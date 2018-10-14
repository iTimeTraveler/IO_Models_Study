cmake_minimum_required(VERSION 3.10)
project(IO_Models_Study)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

MESSAGE(STATUS "###################################")

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")



IF (WIN32)
    MESSAGE(STATUS "Now is windows")
ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple system.")
ELSEIF (LINUX)
    MESSAGE(STATUS "Now is Linux system.")
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS's. Including APPLE os x and CygWin")
ENDIF()

MESSAGE(STATUS "###################################")

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
        src/kqueue.h
        src/multithread_server.cpp
        src/multithread_server.h
        src/aio_server.cpp
        src/aio_server.h)