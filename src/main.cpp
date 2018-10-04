//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "main.h"
#include "block_server.h"
#include "nonblock_server.h"
#include "select_server.h"
#include "poll_server.h"
#include "epoll_server.h"
#include "kqueue.h"
#include "multithread_server.h"

using namespace std;

int main(int argc, char *argv[]) {
    char str[30];

    cout << "为Server端选择一种I/O模型：" << endl;
    cout << "- 1. blocking I/O" << endl;
    cout << "- 2. non-blocking I/O" << endl;
    cout << "- 3. I/O multiplexing (`select`)" << endl;
    cout << "- 4. I/O multiplexing (`poll`)" << endl;
    cout << "- 5. I/O multiplexing (`epoll`)" << endl;
    cout << "- 6. I/O multiplexing (`kqueue`)" << endl;
    cout << "- 7. multithread I/O" << endl;
    cout << "- 8. asynchronous I/O (the POSIX `aio_` functions)" << endl;
    cout << "请输入一个编号： ";
    cin >> str;

    int i = std::stoi(str);
    switch (i) {
        case 1:
            return block_serv(argc, argv);
        case 2:
            return nonblock_serv(argc, argv);
        case 3:
            return select_serv(argc, argv);
        case 4:
            return poll_serv(argc, argv);
        case 5:
            return epoll_serv(argc, argv);
        case 6:
            return kqueue_serv(argc, argv);
        case 7:
            return multithread_serv(argc, argv);
        default:
            break;
    }
}