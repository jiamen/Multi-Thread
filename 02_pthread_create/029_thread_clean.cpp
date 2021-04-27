//
// Created by zlc on 2021/4/27.
//
/*
 * pthread_cleanup_push(void (*rtn) (void*), void* args);    // 注册处理程序
 * pthread_cleanup_pop(int excute);    // 清除处理程序
    这两个函数要成对的出现，否则编译无法通过。
    当执行以下操作时调用清理函数，清理函数的参数由args传入：
        1.调用pthread_exit
        2.响应取消请求
        3.用非零参数调用pthread_cleanup_pop
 * */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
#include <unistd.h>         // sleep函数


void* first_clean(void* arg)
{
    printf("%s first clean\n", arg);
    return (void *)0;
}

void* second_clean(void* arg)
{
    printf("%s second clean\n", arg);
    return (void*)0;
}

void* thread_fun1(void* arg)
{
    printf("new thread 1\n");

    // 注册两个清理函数
    pthread_cleanup_push(reinterpret_cast<void (*)(void *)>(first_clean), (void *) "thread1");
    pthread_cleanup_push(reinterpret_cast<void (*)(void *)>(second_clean), (void *) "thread1");

    // 清除处理程序
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);

    return (void*)1;
}

void* thread_fun2(void* arg)
{
    printf("new thread 2\n");

    // 注册两个清理函数
    pthread_cleanup_push(reinterpret_cast<void (*)(void *)>(first_clean), (void *) "thread2");
    pthread_cleanup_push(reinterpret_cast<void (*)(void *)>(second_clean), (void *) "thread2");

    pthread_exit((void*)2);

    // 清除处理程序
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    // pthread_exit((void*)2); 在后时必须要写1，pthread_exit((void*)2);在前时0、1无所谓

    pthread_exit((void*)2);         // 这里可以把这个函数前提，这样就自动调用自己写的已经注册好的清理函数，且可以任性一点把上面的两个参数1都改为无效移动0
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
        printf("create new thread 1 failed!\n");
        return 0;
    }

    sleep(2);

    return 0;
}




