//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <cstdio>
#include <sys/select.h>
#include <vector>
#include <zconf.h>
#include <poll.h>
#include "poll_server.h"
#include "common.h"

extern const int BUF_SIZE;
extern const unsigned int MAX_CLIENT_NUM;

std::vector<pollfd> poll_sets;

int poll_serv(int argc, char *argv[]) {

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    pollfd listen_pollfd;
    listen_pollfd.fd = serv_sockfd;
    listen_pollfd.events = POLLIN;
    poll_sets.push_back(listen_pollfd);

    while (true) {

        int ret = poll(&poll_sets[0], (unsigned int)poll_sets.size(), -1);
        if (ret <= 0) {
            // timeout (ret = 0) is error
            perror("Error : select error.");
            exit(1);
        }

        print_pollsets(poll_sets);

        // check every pool in the set
        std::vector<pollfd>::iterator it;
        std::vector<pollfd> temp_sets(poll_sets);
        for (it = temp_sets.begin(); it != temp_sets.end(); it++) {

            if (!(it->revents & POLLIN)) {
                if (it->revents & POLLERR) {
                    if (it->fd == serv_sockfd) {
                        // listen socket error
                        perror("listen socket error");
                        exit(1);
                    } else {
                        // client error
                        poll_sets.erase(it);
                        log_dconn(it->fd);
                        close(it->fd);
                    }
                }
                continue;
            }

            // new client
            if (it->fd == serv_sockfd) {

                // accept connection from an incoming client
                int client_sock = accept_socket(serv_sockfd);
                if (client_sock == -1) {
                    perror("ERROR : accept failed");
                    continue;
                }

                // add to fd set
                if (poll_sets.size() - 1 < MAX_CLIENT_NUM) {
                    pollfd client_pollfd;
                    client_pollfd.fd = client_sock;
                    client_pollfd.events = POLLIN;
                    poll_sets.push_back(client_pollfd);
                    log_conn(client_sock);
                } else {
                    too_many_clients(client_sock);
                }
            } else {

                // client echo message
                printf("client echo message\n");
                char message[BUF_SIZE];
                int cli = it->fd;
                int read_size;

                if ((read_size = recv(cli, message, BUF_SIZE, 0)) > 0 ) {
                    log_recv(cli, message);

                    //Send the message back to client
                    send(cli, message, strlen(message)+1, 0);
                    log_send(cli, message);
                } else {
                    // client close
                    poll_sets.erase(it);
                    log_dconn(it->fd);
                    close(it->fd);
                }
            }
        }
        printf("now_client_num: %u\n", (unsigned int)poll_sets.size() - 1);
    }
}

/**
 * show every fd's status after `poll` function return，like this:
 *  [3] [4] [5] [6]
 *  *** OK.	***	***
 */
void print_pollsets(std::vector<pollfd> poll_sets) {
    printf("[poll_sets.size] = %lu\n", poll_sets.size());

    std::vector<pollfd>::iterator it;
    for (it = poll_sets.begin(); it != poll_sets.end(); it++) {
        printf("[%d]\t", it->fd);
    }
    printf("\n");
    for (it = poll_sets.begin(); it != poll_sets.end(); it++) {
        printf("%s\t", (it->revents & POLLIN) ? "OK." : "***");
    }
    printf("\n");
}