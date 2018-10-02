//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <pthread.h>
#include "block_server.h"
#include "common.h"

using namespace std;

extern const int BUF_SIZE;

int block_serv(int argc, char *argv[]) {

    int threadCount = 0;

    // create socket
    int listen_sockfd = create_socket();

    // set reuse
    set_reuse_addr(listen_sockfd);

    // bind
    bind_socket(listen_sockfd);

    // listen
    listen_socket(listen_sockfd);

    while (1) {
        //accept connection from an incoming client
        int client_sock = accept_socket(listen_sockfd);
        if (client_sock == -1) {
            perror("ERROR : accept failed");
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
            printf("create new thread, count: %d\n", ++threadCount);
        }
    }

    return 0;
}