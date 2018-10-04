//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <sys/socket.h>
#include <cstring>
#include <zconf.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include "nonblock_server.h"
#include "common.h"

extern const int BUF_SIZE;

/**
 * 设置socket为non-blocking
 */
static int set_socket_non_block(int sfd) {
    int flags, res;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("Error : cannot get socket flags!\n");
        exit(1);
    }

    flags |= O_NONBLOCK;
    res = fcntl(sfd, F_SETFL, flags);
    if (res == -1) {
        perror("Error : cannot set socket flags!\n");
        exit(1);
    }

    return 0;
}

int nonblock_serv(int argc, char *argv[]) {

    int threadCount = 0;

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // set non-block
    set_socket_non_block(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    while (true) {
        //accept connection from an incoming client
        int client_sock = accept_socket(serv_sockfd);
        if (client_sock == -1) {
            if (errno == EWOULDBLOCK) {
                usleep(200 * 1000);
                printf("waiting for client connection.\n");
            }
            continue;
        }

        log_conn(client_sock);

        // 单独申请一个新的地址
        int cli_sock = client_sock;
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, client_handle_infinit, &cli_sock);
        if (ret != 0) {
            perror("Error : create thread failed.");
        } else {
            printf("new thread, count: %d\n", ++threadCount);
        }
    }
}

void* client_handle_infinit(void* client_pointer) {
    int client_sock = *((int *)client_pointer);

    //Receive a message from client
    char message[BUF_SIZE];
    while(true) {
        int read_size = recv(client_sock, message, BUF_SIZE, 0);
        if (read_size <= 0 ) {
            if (errno == EAGAIN) {
                usleep(200 * 1000);  // sleep 1

                std::ostringstream stringStream;
                stringStream << "Error : [" << client_sock << "] recv error.";
                std::string copyOfStr = stringStream.str();
                perror(copyOfStr.c_str());
            }
            continue;
        }

        log_recv(client_sock, message);

        //Send the message back to client
        send(client_sock, message, strlen(message)+1, 0);
        log_send(client_sock, message);
    }

    log_dconn(client_sock);
    close(client_sock);
    return NULL;
}
