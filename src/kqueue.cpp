//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <sys/event.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <zconf.h>
#include <errno.h>
#include "kqueue.h"
#include "common.h"

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("error no: %d error msg %s\n", errno, strerror(errno)); exit(1);}

extern const int BUF_SIZE;
extern const unsigned int MAX_CLIENT_NUM;
const int kReadEvent    = 1;
const int kWriteEvent   = 2;

int kqueue_serv(int argc, char *argv[]) {

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    // create kqueue, like epoll_create
    int kqfd = kqueue();

    updateEvents(kqfd, serv_sockfd, kReadEvent, false);

    struct kevent events[MAX_CLIENT_NUM];
    while (true) {

        int ret = kevent(kqfd, NULL, 0, events, MAX_CLIENT_NUM, NULL);
        if (ret <= 0) {
            perror("Error : kevent error.");
            exit(1);
        }

        for (int i = 0; i < ret; i++) {
            int sock = events[i].ident;
            int data = events[i].data;

            // new client
            if (sock == serv_sockfd) {

                // accept connection from an incoming client
                int client_sock = accept_socket(serv_sockfd);
                if (client_sock == -1) {
                    perror("ERROR : accept failed");
                    continue;
                }

                // add to fd set
                updateEvents(kqfd, client_sock, kReadEvent|kWriteEvent, false);
            } else {
                
                // client echo message
                printf("client echo message\n");
                char message[BUF_SIZE];
                int cli = sock;
                int read_size;

                if ((read_size = recv(cli, message, BUF_SIZE, 0)) > 0 ) {
                    log_recv(cli, message);

                    //Send the message back to client
                    send(cli, message, strlen(message)+1, 0);
                    log_send(cli, message);
                } else {
                    // client close & delete
                    updateEvents(kqfd, cli, kReadEvent | kWriteEvent, true);
                    log_dconn(cli);
                    close(cli);
                }
            }
        }
    }
}

void updateEvents(int kqfd, int fd, int events, bool is_del) {
    struct kevent ev[2];
    int n = 0;
    if (events & kReadEvent) {
        EV_SET(&ev[n++], fd, EVFILT_READ, is_del ? EV_DELETE : EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    }
    if (events & kWriteEvent) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, is_del ? EV_DELETE : EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    }
    printf("%s fd %d events read %d write %d\n",
           is_del ? "mod" : "add", fd, events & kReadEvent, events & kWriteEvent);
    int r = kevent(kqfd, ev, n, NULL, 0, NULL);
    exit_if(r, "Error : kevent failed ");
}