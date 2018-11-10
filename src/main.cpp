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

map<int, char> mapping;
int x = 0;

int main(int argc, char *argv[]) {
    show_current_system();

    char str[30];

    cout << "为Server端选择一种I/O模型：" << endl;
    cout << "- " << mapPut('A') << ". blocking I/O" << endl;
    cout << "- " << mapPut('B') << ". non-blocking I/O" << endl;
    cout << "- " << mapPut('C') << ". I/O multiplexing (`select`)" << endl;
    cout << "- " << mapPut('D') << ". I/O multiplexing (`poll`)" << endl;
#if ((defined __ANDROID__) || (defined __linux__))
    cout << "- " << mapPut('E') << ". I/O multiplexing (`epoll`)" << endl;
#elif __APPLE__
    cout << "- " << mapPut('F') << ". I/O multiplexing (`kqueue`)" << endl;
#endif
    cout << "- " << mapPut('G') << ". multithread I/O" << endl;
#ifdef __linux__
    cout << "- " << mapPut('H') << ". asynchronous I/O (the POSIX `aio_` functions)" << endl;
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

int mapPut(char target) {
    mapping[++x] = target;
    return x;
}