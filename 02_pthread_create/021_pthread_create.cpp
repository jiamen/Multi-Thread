//
// Created by zlc on 2021/4/26.
//

/*
 * getpid()         获取进程ID
 * pthread_self()   获取线程ID
 *
 * int pthread_create(pthread_t *thread,
 *                    const pthread_attr_t *attr,
 *                    void *(*start_routine)(void*),
 *                    void *arg);
 * 第一个参数，新线程ID，创建成功系统回填
 * 第二个参数，新线程的属性，NULL为默认属性
 * 第三个参数，新线程的启动函数
 * 第四个参数，传递给新线程
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void print_id(char* s)
{
    pid_t pid;      // 无符号整数
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();   // 自己的线程ID号

    printf("%s pid is %u, tid is 0x%x\n", s, pid, tid);     // 进程pid一致，线程tid不一致

}

void* thread_fun(void* arg)
{
    print_id((char*)arg);

    return (void*)0;
}

int main(int argc, char* *argv)
{
    pthread_t ntid;         // 需要创建的线程
    int err;

    err = pthread_create(&ntid, NULL, thread_fun, (void *) "new thread");
    if (err)
    {
        printf("create new thread failed!\n");
        return 0;
    }

    print_id("main_thread: ");      // 第1个线程信息打印
    sleep(2);       // 单位是秒s，等待第2个线程创建成功并打印

    return 0;
}

// 打印的结果：
// main_thread:  pid is 12243, tid is 0xb1d16740
// new thread pid is 12243, tid is 0xb1530700
// 结果表明：进程一致，线程不一致，在一个进程中，所有线程共享进程的ID

