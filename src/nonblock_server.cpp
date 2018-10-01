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
                usleep(20 * 1000);
                printf("waiting for client connection.\n");
            }
            continue;
        }

        log_conn(client_sock);

        //Receive a message from client
        int read_size;
        char message[BUF_SIZE];
        while((read_size = static_cast<int>(recv(client_sock, message, BUF_SIZE, 0))) > 0 ) {
            log_recv(client_sock, message);

            //Send the message back to client
            send(client_sock, message, strlen(message)+1, 0);
            log_send(client_sock, message);
        }

        log_dconn(client_sock);
        close(client_sock);
    }

}
