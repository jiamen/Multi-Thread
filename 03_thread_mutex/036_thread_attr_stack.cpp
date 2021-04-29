//
// Created by zlc on 2021/4/28.
// DESCRIPTION: 设置线程到分离属性
/*
 * 修改栈属性
 * int pthread_attr_setstack(pthread_attr *attr, void* stackaddr, size_t stacksize);
 *
 * 获取栈属性
 * int pthread_attr_getstack(pthread_attr *attr, void* stackaddr, size_t stacksize);
 *
 * 当然也可以单独获取或者修改栈的大小，而不会修改栈的地址。对于栈大小设置，不能小于PTHREAD_STACK_MIN
 * int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
 * int pthread_attr_getstacksize(pthread_attr_t *attr, size_t stacksize);
 *
 * 对于栈大小的设置，在创建线程之后，还可以修改。
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <limits.h>         // PTHREAD_STACK_SIZE 最小栈大小

pthread_attr_t attr;        // 线程属性变量


void* thread_fun(void* arg)
{
    size_t stack_size;

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    pthread_attr_getstacksize(&attr, &stack_size);      // 16384
    printf("new thread stack size is %d\n", stack_size);

    pthread_attr_setstacksize(&attr, 16389);    // 设置的比16384小会失败。
    pthread_attr_getstacksize(&attr, &stack_size);      // 16389
    printf("new thread stack size is %d\n", stack_size);
#endif

    return (void*)1;
}


int main(int argc, char* *argv)
{
    pthread_t tid;
    int err;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
#endif

    err = pthread_create(&tid, &attr, thread_fun, NULL);
    if ( err )
    {
        printf("create new thread failed!\n");
        return 0;
    }

    pthread_join(tid, NULL);

    return 0;
}



