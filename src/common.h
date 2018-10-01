//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#ifndef IO_MODELS_STUDY_COMMON_H
#define IO_MODELS_STUDY_COMMON_H

#include <netinet/in.h>

int create_socket();
void set_reuse_addr(int sockfd);
void bind_socket(int sockfd);
void listen_socket(int sockfd);
int accept_socket(int sockfd);
sockaddr_in get_client_addr(int sockfd);

void log_conn(int client_sock);
void log_recv(int client_sock, char* message);
void log_send(int client_sock, char* message);
void log_dconn(int client_sock);

#ifdef DEBUG
#define print(fmt, arg...) printf(fmt, ##arg)
#else
#define print(fmt, arg...)
#endif

#endif //IO_MODELS_STUDY_COMMON_H
