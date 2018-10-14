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
#include <unistd.h>
#include "block_server.h"
#include "common.h"

extern const int BUF_SIZE;

int block_serv(int argc, char *argv[]) {

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    while (true) {
        //accept connection from an incoming client
        int client_sock = accept_socket(serv_sockfd);
        if (client_sock == -1) {
            perror("ERROR : accept failed");
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