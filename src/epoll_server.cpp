//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include "epoll_server.h"
#include "common.h"

extern const unsigned int MAX_CLIENT_NUM;

int epoll_serv(int argc, char *argv[]) {

    // create socket
    int serv_sockfd = create_socket();

    // set reuse
    set_reuse_addr(serv_sockfd);

    // bind
    bind_socket(serv_sockfd);

    // listen
    listen_socket(serv_sockfd);

    // create epoll
    int epollfd = epoll_create(MAX_CLIENT_NUM);
    if (epollfd == -1) {
        perror("Error : epoll_create error");
        exit(1);
    }

    // add serv_sockfd to epoll
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = serv_sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_sockfd, &ev) == -1) {
        perror("Error : epoll_ctl add serv_sockfd error");
        exit(1);
    }

    return 0;
}