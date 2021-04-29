//
// Created by zlc on 2021/4/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数


void* thread_fun1(void* arg)
{
    printf("I'm new thread 1\n");

    return NULL;
}
// 如果线程具有分离属性，线程终止时会立刻回收，
// 回收将释放掉所有在线程终止时未释放的系统资源和进程资源，
// 包括保存线程返回值的内存空间、堆栈、保存寄存器的内存空间等。


void* thread_fun2(void* arg)
{
    printf("I'm new thread 2\n");

    return NULL;
}


int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;
    int err;

    pthread_attr_t attr;        // 定义线程属性变量
    pthread_attr_init(&attr);   // 线程初始化
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    // 设置分离状态
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);           // 使用这个参数后就可以连接成功

    // err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    err = pthread_create(&tid1, &attr, thread_fun1, NULL);
    if ( 0 != err )
    {
        printf("create new thread 1 failed!\n");
        return 0;
    }

    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if ( 0 != err )
    {
        printf("create new thread 2 failed!\n");
        return 0;
    }

    err = pthread_join(tid1, NULL);
    if ( !err )
        printf("join thread 1 success!\n");
    else
        printf("join thread 1 failed!\n");

    err = pthread_join(tid2, NULL);
    if ( !err )
        printf("join thread 2 success!\n");
    else
        printf("join thread 2 failed!\n");

    pthread_attr_destroy(&attr);

    return 0;
}

