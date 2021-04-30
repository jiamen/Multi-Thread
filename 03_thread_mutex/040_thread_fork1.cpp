//
// Created by zlc on 2021/4/29.
//
// Description: 安全地使用fork

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <fcntl.h>          // O_RDWR等宏定义

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_fun(void *arg)
{
    sleep(1);
    pid_t pid;
    pid = fork();
    if (0==pid)
    {
        pthread_mutex_lock(&mutex);     // 这里想锁锁不上
        printf("child\n");
        pthread_mutex_unlock(&mutex);
    }
    if (pid>0)
    {
        pthread_mutex_lock(&mutex);     // 这里的话只要等待父进程main函数中解锁，这就可以照常加锁解锁
        printf("parent\n");
        pthread_mutex_unlock(&mutex);
    }
}


int main(int argc, char* *argv)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_fun, NULL))
    {
        printf("create new thread failed!\n");
        return 0;
    }

    pthread_mutex_lock(&mutex);
    sleep(2);
    pthread_mutex_unlock(&mutex);
    printf("main\n");
    pthread_join(tid, NULL);

    return 0;
}
