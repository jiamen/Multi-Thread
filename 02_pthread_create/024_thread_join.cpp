/*
 *
 * pthread_join(pthread_t tid, void* *rval);
 *      调用该函数ud线程会阻塞，直到线程tid终止，而且会使tid分离
 *      如果tid已经分离，那么调用pthread_join就会失败
 * pthreaddetach(pthreadt, tid);
 *      分离线程  由tid指定待分离线程
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>


void* thread_fun1(void* arg)
{
    printf("I'm thread 1. \n");
    return (void*)1;                  // 线程返回码
}

void* thread_fun2(void* arg)
{
    printf("I'm thread 2. \n");
    pthread_detach(pthread_self());
    pthread_exit((void*)2);     // 线程返回码
}


int main(int argc, char* *argv)
{
    int err1, err2;
    pthread_t tid1, tid2;
    void* rval1, *rval2;

    err1 = pthread_create(&tid1, NULL, thread_fun1, NULL);
    err2 = pthread_create(&tid2, NULL, thread_fun2, NULL);

    if (err1 && err2)
    {
        printf("create new thread failed!\n");
        return 0;
    }

    printf("I'm main thread!\n");
    // 查看有没有连接成功
    printf("join1 rval is %d\n", pthread_join(tid1, &rval1));   // 打印pthread_join函数返回的0，表示连接成功，注意这个不是线程返回码
    printf("join2 rval is %d\n", pthread_join(tid2, &rval2));
    // 打印错误码22，表示连接失败，可以在/usr/include/asm-generic/errno-base.h中查看22代表什么错误，错误宏定义为EINVAL，表示不是一个可连接的线程
    // 使用 man pthread_join查看手册

    // 打印返回码（线程退出码）
    printf("thread 1 exit code is %d\n", (int*)rval1);          // 返回码rval1为1
    printf("thread 2 exit code is %d\n", (int*)rval2);          // 返回码rval2本来应为2，但是由于detach函数存在，返回码变成了不确定数


    return 0;
}