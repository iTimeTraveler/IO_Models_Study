//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"


extern const int BUF_SIZE = 1024;
extern const char SERVER_IP[] = "0.0.0.0";
extern const int SERVER_PORT = 8999;
extern const int BACKLOG = 3;

extern const unsigned int MAX_CLIENT_NUM = 100*1024 + 10;
extern const char ERR_MESSAGE[] = "[ERR]too many clients!!!";
extern const int CLIENT_INTERVAL = 1;


int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("create_socket error");
        exit(1);
    }
    return sockfd;
}

void set_reuse_addr(int sockfd) {
    int ret;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret)) == -1) {
        perror("set_reuse_addr error");
        exit(1);
    }
}

void bind_socket(int sockfd) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if(bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind_socket error");
        exit(1);
    }
}

void listen_socket(int sockfd) {
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen_socket error");
        exit(1);
    }

    // start success
    print("Server started success, listen at %s:%d\n", SERVER_IP, SERVER_PORT);
}

int accept_socket(int sockfd) {
    sockaddr_in client;
    socklen_t addr_size = sizeof(sockaddr_in);
    return accept(sockfd, (sockaddr *)&client, &addr_size);
}

sockaddr_in get_client_addr(int sockfd) {
    sockaddr_in client;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr*)&client, &addr_size) != 0) {
        perror("get_client_addr error");
        exit(1);
    }
    return client;
}


void log_conn(int client_sock) {
    #ifdef DEBUG
    sockaddr_in client = get_client_addr(client_sock);
    #endif

    print("[Conn %d] %s:%d\n", client_sock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
}

void log_recv(int client_sock, char* message) {
    #ifdef DEBUG
    sockaddr_in client = get_client_addr(client_sock);
    #endif

    print("[Recv %d] %s:%d %s\n", client_sock, inet_ntoa(client.sin_addr), ntohs(client.sin_port), message);
}

void log_send(int client_sock, char* message) {
    #ifdef DEBUG
    sockaddr_in client = get_client_addr(client_sock);
    #endif

    print("[Send %d] %s:%d %s\n", client_sock, inet_ntoa(client.sin_addr), ntohs(client.sin_port), message);
}

void log_dconn(int client_sock) {
    #ifdef DEBUG
    sockaddr_in client = get_client_addr(client_sock);
    #endif

    print("[DConn %d] %s:%d\n", client_sock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
}