//
// Created by zlc on 2021/4/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数


pthread_once_t once = PTHREAD_ONCE_INIT;    // PTHREAD_ONCE_INIT=0
// pthread_once_t once = 1;     // 等于1的情况下就会阻塞，等待多久都不会有退出
// pthread_once_t once = 2;     // once打印输出全部是2，初始化也不会执行
pthread_t tid;

void thread_init()
{
    printf("I'm in thread 0x%x\n", tid);
}

void* thread_fun1(void* arg)
{
    tid = pthread_self();                       // 自身的线程号
    printf("I'm thread1 0x%x\n", tid);
    printf("before once is %d.\n", once);   // 开始是0
    pthread_once(&once, thread_init);       // 一次性初始化
    printf("after once is %d.\n", once);    // 结束是2，代表已初始化

    return NULL;
}


void* thread_fun2(void* arg)
{
    sleep(2);   // 这会导致线程1先执行
    tid = pthread_self();                   // 自身的线程号
    printf("I'm thread2 0x%x\n", tid);
    pthread_once(&once, thread_init);       //线程1先执行，导致这里不会再进行初始化

    return NULL;
}


int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;
    int err;

    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
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

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

