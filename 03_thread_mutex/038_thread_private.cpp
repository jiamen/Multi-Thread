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

// #include <sys/mman.h>       // 使用shm_open来操作共享内存
#include <errno.h>

void* thread_fun1(void *arg)
{
    printf("thread 1 start!\n");
    open("1.c", O_RDWR);
    sleep(2);
    printf("thread 1 errno is : %d\n", errno);
}

void* thread_fun2(void *arg)
{
    sleep(1);
    printf("thread 2 start!\n");
    open("../t.c", O_RDWR);
    printf("thread 2 errno is : %d\n", errno);  // 线程1与线程2的errno值不同
}

int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;

    if (pthread_create(&tid1, NULL, thread_fun1, NULL))
    {
        printf("create new thread 1 failed!\n");
        return 0;
    }
    if (pthread_create(&tid2, NULL, thread_fun2, NULL))
    {
        printf("create new thread 2 failed!\n");
        return 0;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}


