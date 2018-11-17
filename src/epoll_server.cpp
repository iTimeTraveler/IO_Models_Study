//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "epoll_server.h"
#include "common.h"

#if ((defined __ANDROID__) || (defined __linux__))
#include <sys/epoll.h>
#endif

extern const int BUF_SIZE;
extern const unsigned int MAX_CLIENT_NUM;
int MAX_EVENTS = 100;

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

int epoll_serv(int argc, char *argv[]) {
#if ((defined __ANDROID__) || (defined __linux__))

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // set non-block
    // set_socket_non_block(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    // create epoll
    int epollfd = epoll_create(MAX_EVENTS);
    if (epollfd == -1) {
        perror("Error : epoll_create error");
        exit(1);
    }

    // add serv_sockfd to epoll
    struct epoll_event ev, events[MAX_EVENTS];
    ev.data.fd = serv_sockfd;
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;      //设置要处理的事件类型
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_sockfd, &ev) == -1) {
        perror("Error : epoll_ctl add serv_sockfd error");
        exit(1);
    }

    while (true) {
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);    //等待epoll事件的发生
        if (nfds == -1) {
            printf("epoll_wait failed %d [%d]\n", errno, nfds);
            break;
        }

        for (int i = 0; i < nfds; i++) {
            int evtfd = events[i].data.fd;

            if (evtfd < 0)
                continue;

            // socket错误
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                /* An error has occured on this fd, or the socket is not
                 * ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll events error\n");
                close (evtfd);
                continue;
            }

            // new client
            if (evtfd == serv_sockfd) {   //监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。

                // accept connection from an incoming client
                int client_sock = accept_socket(serv_sockfd);
                if (client_sock == -1) {
                    perror("ERROR : accept failed");
                    continue;
                }

                ev.data.fd = client_sock;
                ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;  //设置要处理的事件类型
                epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sock, &ev);
                continue;
            }

            if (events[i].events & EPOLLIN) {   //socket收到数据

                // client echo message
                printf("client echo message\n");
                char buf[BUF_SIZE];
                int cli = evtfd;
                int read_size;

                if ((read_size = recv(cli, buf, BUF_SIZE, 0)) > 0 ) {
                    log_recv(cli, buf);

                    //Send the message back to client
                    send(cli, buf, strlen(buf)+1, 0);
                    log_send(cli, buf);
                } else {
                    // client close & delete
                    // updateEvents(kqfd, cli, kReadEvent | kWriteEvent, true);
                    log_dconn(cli);
                    close(cli);
                }
            }
        }
    }
#else
    perror("`epoll` just support Linux platform");
#endif
    return 0;
}