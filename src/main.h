//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#ifndef IO_MODELS_STUDY_MAIN_H
#define IO_MODELS_STUDY_MAIN_H


int mapPut(char target);


#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
    #ifdef _WIN64
        //define something for Windows (64-bit only)
    #else
        //define something for Windows (32-bit only)
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
        #include "kqueue.h"
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // android
    #include "epoll_server.h"
#elif __linux__
    // linux
    #include "epoll_server.h"
    #include "aio_server.h"
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif



void show_current_system() {
#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    #ifdef _WIN64
        //define something for Windows (64-bit only)
        printf("current platform: Windows (64-bit only) system.\n");
    #else
        //define something for Windows (32-bit only)
        printf("current platform: Windows (32-bit only) system.\n");
    #endif
#elif __APPLE__
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        printf("current platform: iOS Simulator system.\n");
    #elif TARGET_OS_IPHONE
        // iOS device
        printf("current platform: iOS device system.\n");
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
        printf("current platform: Mac system.\n");
    #else
    #   error "Unknown Apple platform"
        printf("current platform: Unknown Apple platform.\n");
    #endif
#elif __ANDROID__
    // android
    printf("current platform: Android system.\n");
#elif __linux__
    // linux
    printf("current platform: Linux system.\n");
#elif __unix__ // all unices not caught above
    // Unix
    printf("current platform: UNIX-like OS system.\n");
#elif defined(_POSIX_VERSION)
    // POSIX
    printf("current platform: POSIX system.\n");
#else
#   error "Unknown compiler"
    printf("current platform: Unknown compiler.\n");
#endif
}


#endif //IO_MODELS_STUDY_MAIN_H
