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
    int listen_sockfd = create_socket();

    // set reuse
    set_reuse_addr(listen_sockfd);

    // set non-block
    set_socket_non_block(listen_sockfd);

    // bind
    bind_socket(listen_sockfd);

    // listen
    listen_socket(listen_sockfd);

    while (1) {
        //accept connection from an incoming client
        int client_sock = accept_socket(listen_sockfd);
        if (client_sock == -1) {
            if (errno == EWOULDBLOCK) {
                usleep(2 * 1000);
                printf("waiting for client connection.\n");
            }
            continue;
        }

        log_conn(client_sock);

        // 单独申请一个新的地址
        int cli_sock = client_sock;
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, client_handler, &cli_sock);
        if (ret != 0) {
            perror("Error : create thread failed.");
        } else {
            printf("new thread, count: %d\n", ++threadCount);
        }
    }

}
