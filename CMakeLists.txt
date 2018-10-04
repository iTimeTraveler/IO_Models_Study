cmake_minimum_required(VERSION 3.10)
project(IO_Models_Study)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

# DEBUG模式
ADD_DEFINITIONS(-DDEBUG)

add_executable(client
        src/client.cpp
        src/common.cpp
        src/common.h)

add_executable(server
        src/main.cpp
        src/main.h
        src/common.cpp
        src/common.h
        src/block_server.cpp
        src/block_server.h
        src/nonblock_server.cpp
        src/nonblock_server.h
        src/select_server.cpp
        src/select_server.h
        src/poll_server.cpp
        src/poll_server.h
        src/epoll_server.cpp
        src/epoll_server.h
        src/kqueue.cpp
        src/kqueue.h)