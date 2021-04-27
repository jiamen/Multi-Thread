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
    memset(&act, 0, sizeof(act));
    int err;
    sigemptyset(&act.sa_mask);                  // 初始化mask信号集
    err = sigaddset(&act.sa_mask, SIGALRM);     // 设置信号
    if ( err!=0 )
    {
        printf("add failed!\n");
        return 0;
    }
    // printf("act.sa_mask = %ld\n", act.sa_mask.__val);

    act.sa_flags = 0;
    act.sa_handler = sig_handler1;              // 设置信号处理函数
    sigaction(SIGALRM, &act, NULL);        // 根据上面的设置进行信号处理，设置信号处理函数

    // 屏蔽信号，thread_fun1和下面的thread_fun2中都注释掉信号屏蔽则只打印一个信号处理函数
    pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(3);
}


void* thread_fun2(void* arg)
{
    printf("new thread 2\n");

    // 配置自己的信号处理函数
    struct sigaction act;
    memset(&act, 0,sizeof(act));
    int err;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    err = sigaddset(&act.sa_mask, SIGKILL);
    if ( err!=0 )
    {
        printf("add failed!\n");
        return 0;
    }
    act.sa_handler = sig_handler2;
    sigaction(SIGKILL, &act, NULL);         // 时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号.

    // 注释就是打印两个同样的
    pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
    sleep(3);
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
        exit(1);
    }
    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if ( 0!=err )
    {
        printf("create new thread 2 failed!\n");
        exit(1);
    }

    sleep(3);

    s = pthread_kill(tid1, SIGALRM);            // 经测试，SIGKILL和SIGQUIT只要出现，立马结束‘进’程，而非结束‘线’程
    if ( s!=0 )
    {
        printf("send signal to thread1 failed!\n");
    }
    s = pthread_kill(tid2, SIGALRM);
    if ( s!=0 )
    {
        printf("send signal to thread2 failed!\n");
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    return 0;
}
