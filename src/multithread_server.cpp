//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include "multithread_server.h"
#include "common.h"

extern const int BUF_SIZE;
extern const unsigned int MAX_CLIENT_NUM;
unsigned int threadCount = 0;
pthread_mutex_t threadCount_lock;

int multithread_serv(int argc, char *argv[]) {

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

        if (threadCount < MAX_CLIENT_NUM) {
            // 单独申请一块新地址
            int cli_sock = client_sock;
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, client_handle, &cli_sock);
            if (ret != 0) {
                perror("Error : create thread failed.");
            } else {
                pthread_mutex_lock(&threadCount_lock);
                threadCount++;
                pthread_mutex_unlock(&threadCount_lock);
                printf("create new thread, count: %d\n", threadCount);
            }
        } else {
            too_many_clients(client_sock);
        }
    }
}

void* client_handle(void* client_pointer) {
    int client_sock = *((int *)client_pointer);

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

    pthread_mutex_lock(&threadCount_lock);
    threadCount--;
    pthread_mutex_unlock(&threadCount_lock);
    printf("delete thread, count: %d\n", threadCount);
    return NULL;
}