//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "main.h"
#include "block_server.h"
#include "nonblock_server.h"

using namespace std;

int main(int argc, char *argv[]) {
    char str[30];

    cout << "五种I/O模型：" << endl;
    cout << "- 1. blocking I/O" << endl;
    cout << "- 2. non-blocking I/O" << endl;
    cout << "- 3. I/O multiplexing (`select` and `poll`)" << endl;
    cout << "- 4. signal driven I/O (`SIGIO`)" << endl;
    cout << "- 5. asynchronous I/O (the POSIX `aio_` functions)" << endl;
    cout << "请选择一个编号： ";
    cin >> str;

    int i = std::stoi(str);
    switch (i) {
        case 1:
            return block_serv(argc, argv);
        case 2:
            return nonblock_serv(argc, argv);
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            break;
    }
}