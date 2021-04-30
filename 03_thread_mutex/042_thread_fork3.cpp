//
// Created by zlc on 2021/4/29.
//
/*
 * Description: 安全的使用fork，保证创建子进程的时候所有互斥量两两配对
 *      prepare  在调用fork之前会被调用
 *      parent   在fork返回父进程之前被调用
 *      child    在fork返回子进程之前被调用
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <fcntl.h>          // O_RDWR等宏定义

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void prepare()
{
    pthread_mutex_lock(&mutex);
    printf("I'm prepare.\n");
}

void parent()
{
    pthread_mutex_unlock(&mutex);
    printf("I'm parent.\n");
}

void child()
{
    pthread_mutex_unlock(&mutex);
    printf("I'm child.\n");
}

void* thread_fun(void* arg)
{
    sleep(1);
    pid_t pid;

    pthread_atfork(prepare, parent, child);

    pid = fork();
    if (0==pid)
    {
        pthread_mutex_lock(&mutex);
        printf("child process.\n");
        pthread_mutex_unlock(&mutex);
    }
    if (pid>0)
    {
        pthread_mutex_unlock(&mutex);
        printf("parent process\n");
    }
}


int main(int argc, char* *argv)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_fun, NULL) )
    {
        printf("create new thread failed\n");
        return 0;
    }

    printf("main\n");
    pthread_join(tid, NULL);
    return 0;
}
