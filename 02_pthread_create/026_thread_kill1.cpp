//
// Created by zlc on 2021/4/27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include <signal.h>         // 信号
#include <errno.h>          // 包含错误码所有类型


void* thread_fun(void *arg)
{
    sleep(1);
    printf("I'm new thread!\n");

    return (void*)0;
}


int main(int argc, char* *argv)
{
    pthread_t tid;
    int err;
    int s;
    void* rval;

    err = pthread_create(&tid, NULL, thread_fun, NULL);
    if ( 0!=err )
    {
        printf("create new thread failed!\n");
        return 0;
    }
    // sleep(1);

    // s = pthread_kill(tid, 0);       // 在signal.h头文件中
    s = pthread_kill(tid, SIGQUIT);
    if ( s==ESRCH )
    {
        printf("thread tid is not found!\n");
    }

    pthread_join(tid, &rval);

    printf("I'm main thread!\n");

    return 0;
}

