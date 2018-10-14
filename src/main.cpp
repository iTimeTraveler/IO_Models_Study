//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <map>
#include "main.h"
#include "block_server.h"
#include "nonblock_server.h"
#include "select_server.h"
#include "poll_server.h"
#include "multithread_server.h"

using namespace std;

int main(int argc, char *argv[]) {
    show_current_system();

    char str[30];
    map<int, char> mapping;

    for (char i = 0; i < 8; i++) {
        mapping[i + 1] = ('A' + i);
    }

    int x = 0;
    cout << "为Server端选择一种I/O模型：" << endl;
    cout << "- " << ++x << ". blocking I/O" << endl;
    cout << "- " << ++x << ". non-blocking I/O" << endl;
    cout << "- " << ++x << ". I/O multiplexing (`select`)" << endl;
    cout << "- " << ++x << ". I/O multiplexing (`poll`)" << endl;
#if ((defined __ANDROID__) || (defined __linux__))
    cout << "- " << ++x << ". I/O multiplexing (`epoll`)" << endl;
#elif __APPLE__
    cout << "- " << ++x << ". I/O multiplexing (`kqueue`)" << endl;
#endif
    cout << "- " << ++x << ". multithread I/O" << endl;
#ifdef __linux__
    cout << "- " << ++x << ". asynchronous I/O (the POSIX `aio_` functions)" << endl;
#endif
    cout << "请输入一个编号： ";
    cin >> str;

    int in = std::stoi(str);
    char c = mapping[in];
    printf("str_index: %d_%c\n", in, c);

    switch (c) {
        case 'A':
            cout << "You choose blocking server." << endl;
            return block_serv(argc, argv);
        case 'B':
            cout << "You choose non-blocking server." << endl;
            return nonblock_serv(argc, argv);
        case 'C':
            cout << "You choose `select` server." << endl;
            return select_serv(argc, argv);
        case 'D':
            cout << "You choose `poll` server." << endl;
            return poll_serv(argc, argv);
#if ((defined __ANDROID__) || (defined __linux__))
        case 'E':
            cout << "You choose `epoll` server." << endl;
            return epoll_serv(argc, argv);
#elif __APPLE__
        case 'F':
            cout << "You choose `kqueue` server." << endl;
            return kqueue_serv(argc, argv);
#endif
        case 'G':
            cout << "You choose multithread server." << endl;
            return multithread_serv(argc, argv);
        case 'H':
            printf("Sorry, asynchronous I/O, command [%s] Not support.\n", str);
            return 0;
        default:
            printf("Sorry, command [%s] Not support.\n", str);
            return 0;
    }
}