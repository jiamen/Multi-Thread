//
// Created by zlc on 2021/4/29.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <fcntl.h>          // O_RDWR等宏定义

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_fun(void* arg)
{
    sleep(1);
    printf("new thread\n");

    pid_t pid;
    pthread_mutex_lock(&mutex);

    pid = fork();
    if (0==pid)
    {
        pthread_mutex_unlock(&mutex);
        printf("child\n");
    }
    if (pid>0)
    {
        pthread_mutex_unlock(&mutex);
        printf("parent\n");
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

