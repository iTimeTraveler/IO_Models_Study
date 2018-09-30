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
#include "block_server.h"
#include "common.h"

using namespace std;

extern const int BUF_SIZE;
extern const char SERVER_IP[];
extern const int SERVER_PORT;
extern const int BACKLOG;

int block_serv(int argc, char *argv[]) {

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
            perror("accept failed");
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