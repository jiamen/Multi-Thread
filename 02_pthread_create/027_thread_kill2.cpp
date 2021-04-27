//
// Created by zlc on 2021/4/27.
//
/* Description：正确处理信号
 *
 * int pthread_kill(pthread_t thread, int sig);
 *    向线程thread发送sig信号，成功返回0，失败返回错误码。
 *
 * int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact);
 *      为信号signum设置一个处理函数，处理函数在act_sigaction中指定。
 *      act.sa_mask信号屏蔽字。
 *      act.sa_handler信号集处理程序。
 *
 * 多线程屏蔽函数：
 * int pthread_sigmask(int how, const sigset_t* set, sigset_t* oldset);
 *    how = SIG_BLOCK：向当前的信号掩码中添加set，其中set表示要阻塞的信号组。
 *          SIG_UNBLOCK：向当前的信号掩码中添加set，其中set表示要取消阻塞的信号组。
 *          SIG_SETMASK：将当前的信号掩码替换为set，其中set表示新的信号掩码。
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>         // 信号
#include <errno.h>          // 包含错误码所有类型

/*
// 写好两个信号处理函数
void sig_handler1(int arg)
{
    printf("thread1 get signal\n");
    return;
}

void sig_handler2(int arg)
{
    printf("thread2 get signal\n");
    return;
}


void* thread_fun1(void* arg)
{
    printf("new thread 1\n");

    // 配置自己的信号处理函数
    struct sigaction act;
    memset(&act, 0,sizeof(act));
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_handler = sig_handler1;
    sigaction(SIGQUIT, &act, NULL);

    // 屏蔽信号
    pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(2);
}


void* thread_fun2(void* arg)
{
    printf("new thread 2\n");

    // 配置自己的信号处理函数
    struct sigaction act;
    memset(&act, 0,sizeof(act));
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_handler = sig_handler2;
    sigaction(SIGQUIT, &act, NULL);

    pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(2);
}


int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;
    int err;
    int s;

    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    if ( 0!=err )
    {
        printf("create new thread 1 failed!\n");
        return 0;
    }
    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if ( 0!=err )
    {
        printf("create new thread 2 failed!\n");
        return 0;
    }

    sleep(1);

    s = pthread_kill(tid1, SIGQUIT);
    if ( s!=0 )
    {
        printf("send signal to thread1 failed!\n");
    }
    s = pthread_kill(tid2, SIGQUIT);
    if ( s!=0 )
    {
        printf("send signal to thread2 failed!\n");
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    return 0;
}
*/

void sig_handler1(int arg)
{
    printf("thread1 get signal\n");

    return;
}

void sig_handler2(int arg)
{
    printf("thread2 get signal\n");

    return;
}

void *thread_fun1(void *arg)
{
    printf("new thread 1\n");

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_handler = sig_handler1;
    sigaction(SIGQUIT, &act, NULL);

    pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(2);
}


void *thread_fun2(void *arg)
{
    printf("new thread 2\n");

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_handler = sig_handler2;
    sigaction(SIGQUIT, &act, NULL);

    // pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(2);
}

int main()
{
    pthread_t tid1, tid2;
    int err;
    int s;

    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    if(err != 0)
    {
        printf("create new thread 1 failed\n");
        return -1;
    }
    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if(err != 0)
    {
        printf("create new thread 2 failed\n");
        return -1;
    }

    sleep(1);

    s = pthread_kill(tid1, SIGQUIT);
    if(s != 0)
    {
        printf("send signal to thread1 failed\n");
    }

    s = pthread_kill(tid2, SIGQUIT);
    if(s != 0)
    {
        printf("send signal to thread2 failed\n");
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
