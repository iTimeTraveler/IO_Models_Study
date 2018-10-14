//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#include <mm_malloc.h>
#include <zconf.h>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "aio_server.h"

#ifdef __ANDROID__
#elif __linux__
#include <libaio.h>
#endif

extern const int BUF_SIZE;
#define MAX_COUNT 1024

#ifndef O_DIRECT
#define O_DIRECT         040000 /* direct disk access hint */
#endif

int aio_serv(int argc, char *argv[]) {
#ifdef __ANDROID__
    perror("`libaio` just support Linux platform");
#elif __linux__

    int fd;

    void * buf = NULL;

    //获取页面大小
    int pagesize = sysconf(_SC_PAGESIZE);
    //处理对齐
    posix_memalign(&buf, pagesize, BUF_SIZE);

    memset(buf,'A',BUF_SIZE);

    io_context_t ctx;
    struct iocb io,*p=&io;
    struct io_event e[10];
    struct timespec timeout;

    memset(&ctx,0,sizeof(ctx));
    //创建并初始化context
    if(io_setup(MAX_COUNT,&ctx)!=0){
        printf("io_setup error\n");
        return -1;
    }

    if((fd = open("./test.txt", O_WRONLY | O_CREAT | O_APPEND | O_DIRECT, 0644))<0) {
        perror("open error");
        io_destroy(ctx);
        return -1;
    }

    int n = MAX_COUNT;

    while(n > 0) {
        //写数据
        io_prep_pwrite(&io, fd, buf, BUF_SIZE, 0);
        //提交请求
        if(io_submit(ctx, 1, &p)!=1) {
            io_destroy(ctx);
            printf("io_submit error\n");
            return -1;
        }
        //获取完成事件
        int ret = io_getevents(ctx, 1, 10, e, NULL);
        if (ret != 1) {
            perror("ret != 1");
            break;
        }
        n--;
    }

    close(fd);
    //销毁context
    io_destroy(ctx);
#else
    perror("`libaio` just support Linux platform");
#endif
    return 0;
}