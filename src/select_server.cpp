//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <cstdio>
#include <sys/select.h>
#include <vector>
#include <zconf.h>
#include <unistd.h>
#include "select_server.h"
#include "common.h"

extern const int BUF_SIZE;
extern const unsigned int MAX_CLIENT_NUM;

std::set<int> fd_sets;

int select_serv(int argc, char *argv[]) {

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    fd_sets.insert(serv_sockfd);
    fd_set readfds;
    while (true) {

        // initialize file descriptor set
        FD_ZERO(&readfds);

        // add active connection to read fd set.
        std::set<int>::iterator it;
        for (it = fd_sets.begin(); it != fd_sets.end(); it++)
            FD_SET(*it, &readfds);

        int ret = select(*fd_sets.rbegin() + 1, &readfds, nullptr, nullptr, nullptr);
        if (ret <= 0) {
            // timeout (ret = 0) is error
            perror("Error : select error.");
            exit(1);
        }

        print_fdsets(fd_sets, readfds);

        // check every fd in the set
        std::set<int> temp_sets(fd_sets);
        it = temp_sets.begin();
        for (; it != temp_sets.end() && ret != 0; it++) {

            if (!FD_ISSET(*it, &readfds)) {
                continue;
            }

            ret--;

            // new client
            if (*it == serv_sockfd) {

                // accept connection from an incoming client
                int client_sock = accept_socket(serv_sockfd);
                if (client_sock == -1) {
                    perror("ERROR : accept failed");
                    continue;
                }

                // add to fd set
                if (fd_sets.size() - 1 < MAX_CLIENT_NUM) {
                    fd_sets.insert(client_sock);
                    log_conn(client_sock);
                } else {
                    too_many_clients(client_sock);
                }
            } else {

                // client echo message
                printf("client echo message\n");
                char message[BUF_SIZE];
                int cli = *it;
                int read_size;

                if ((read_size = recv(cli, message, BUF_SIZE, 0)) > 0 ) {
                    log_recv(cli, message);

                    //Send the message back to client
                    send(cli, message, strlen(message)+1, 0);
                    log_send(cli, message);
                } else {
                    // client close
                    fd_sets.erase(*it);
                    log_dconn(*it);
                    close(*it);
                }
            }
        }
        printf("now_client_num: %u\n", (unsigned int)fd_sets.size() - 1);
    }
}

void print_fdsets(std::set<int> fd_sets, fd_set readfds) {
    printf("[fd_sets.size] = %u\n", fd_sets.size());

    std::set<int>::iterator it;
    for (it = fd_sets.begin(); it != fd_sets.end(); it++) {
        printf("[%d]\t", *it);
    }
    printf("\n");
    for (it = fd_sets.begin(); it != fd_sets.end(); it++) {
        printf("%s\t", FD_ISSET(*it, &readfds) ? "OK." : "***");
    }
    printf("\n");
}