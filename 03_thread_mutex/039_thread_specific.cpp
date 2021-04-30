//
// Created by zlc on 2021/4/29.
//
// Description： 线程的私有数据，一个像errno一样的数据


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <fcntl.h>          // O_RDWR等宏定义

pthread_key_t key;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_fun1(void* arg)
{
    printf("thread 1 start!\n");
    int a = 1;
    pthread_setspecific(key, (void *)a);
    sleep(2);
    printf("thread 1 key->data is %d\n", pthread_getspecific(key));
}

void* thread_fun2(void* arg)
{
    sleep(1);
    printf("thread 2 start!\n");
    int a = 2;
    pthread_setspecific(key, (void*)a);     // 将线程私有数据与键值关联
    printf("thread 2 key->data is %d\n", pthread_getspecific(key));
}

int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;

    pthread_key_create(&key, NULL);

    if (pthread_create(&tid1, NULL, thread_fun1, NULL))
    {
        printf("create new thread 1 failed!\n");
        return 0;
    }
    if (pthread_create(&tid2, NULL, thread_fun2, NULL))
    {
        printf("create new thread  2 failed!\n");
        return 0;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

