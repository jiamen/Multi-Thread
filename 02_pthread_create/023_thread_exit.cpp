// Description: 验证几种线程到退出方式
// Created by zlc on 2021/4/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void* thread_fun(void* arg)
{
    // 第一种方式：返回值是线程的退出码,return+退出码
    if (0 == strcmp("1", (char*)arg))
    {
        printf("new thread return!\n");
        return (void*)1;
    }
    // 第二种方式：线程调用pthread_exit(void* rval)函数，rval是退出码。
    if (0 == strcmp("2", (char*)arg))
    {
        printf("new thread pthread_exit!\n");
        pthread_exit((void*)2);
    }
    // exit()的退出是最危险的，直接导致进程退出，后面的主线程打印也无法执行
    if (0 == strcmp("3", (char*)arg))
    {
        printf("new thread exit!\n");
        exit(3);
    }
}


int main(int argc, char* *argv)
{
    int err;
    pthread_t  tid;

    err = pthread_create(&tid, NULL, thread_fun, (void*)argv[1]);
    if (0 != err)
    {
        printf("create new thread failed!\n");
        return 0;
    }

    sleep(1);
    printf("main thread!\n");   // 主线程打印

    return 0;
}

