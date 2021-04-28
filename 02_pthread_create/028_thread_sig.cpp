//
// Created by zlc on 2021/4/27.
//

/* threadsig.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数

void sighandler(int signo);

// 线程1启动函数
void* thr1_fn(void* arg)
{
    struct sigaction action;
    action.sa_flags=0;
    action.sa_handler = sighandler;

    sigaction(SIGINT, &action, NULL);

    pthread_t tid=pthread_self();
    int rc;

    printf("thread1 with tid : %lu\n", tid);
    rc = sleep(60);
    if ( rc != 0 )
        printf("thread1 ... interrupted at %d seconds\n",60-rc);

    printf("thread1 ends\n");

    return NULL;
}

// 线程2启动函数
void* thr2_fn(void* arg)
{
    struct sigaction action;
    pthread_t tid = pthread_self();
    int rc,err;

    printf("thread2 with tid : %lu\n", tid);

    action.sa_flags = 0;
    action.sa_handler = sighandler;

    err = sigaction(SIGALRM, &action, NULL);

    rc = sleep(60);
    if ( rc != 0 )
        printf("thread2 ... interrupted at %d second\n", 60-rc);

    printf("thread2 ends\n");

    return NULL;
}

// 线程3启动函数
void* thr3_fn(void* arg)
{
    pthread_t tid = pthread_self();
    sigset_t mask;
    int rc, err;

    printf("thread3 with tid %lu\n",tid);


    sigemptyset(&mask);     /* 初始化mask信号集 */

    sigaddset(&mask, SIGALRM);
    err = pthread_sigmask(SIG_BLOCK,&mask,NULL);
    if ( err != 0 )
    {
        printf("%d, %s/n", rc, strerror(rc));
        return NULL;
    }

    rc = sleep(10);
    if ( rc != 0 )
        printf("thread3 ... interrupted at %d second\n", 60-rc);

    err=pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
    if ( err != 0 )
    {
        printf("unblock %d, %s/n", rc, strerror(rc));
        return NULL;
    }

    rc = sleep(10);
    if ( rc != 0 )
        printf("thread3 ... interrupted at %d second after unblock\n", 60-rc);
    printf("thread3 ends\n");

    return NULL;

}

int main(int argc, char* *argv)
{
    int rc, err;
    pthread_t thr1, thr2, thr3, thrm = pthread_self();
    
    printf("thread main with pid %lu\n", thrm);                 // 先打印进程号
    err = pthread_create(&thr1, NULL, thr1_fn, NULL);       // 创造线程1
    if ( err != 0 )
    {
        printf("error in creating pthread : %d\t%s\n", err, strerror(rc));
        exit(1);
    }
    
    
    /* pthread_kill(thr1,SIGALRM);
     * send a SIGARLM signal to thr1 before thr2 set the signal handler,then the whole process will be terminated*/
    err = pthread_create(&thr2, NULL, thr2_fn, NULL);       // 创造线程2
    if ( err != 0 )
    {
        printf("error in creating pthread : %d\t%s\n", err, strerror(rc));
        exit(1);
    }
    
    err = pthread_create(&thr3, NULL, thr3_fn, NULL);       // 创造线程3
    if ( err != 0 )
    {
        printf("error in creating pthread : %d\t%s\n", err, strerror(rc));
        exit(1);
    }
    
    sleep(10);
    // 内部产生的信号，只有指定的线程能收到，因此要向所有线程发送
    pthread_kill(thr1, SIGALRM);
    pthread_kill(thr2, SIGALRM);
    pthread_kill(thr3, SIGALRM);
    pthread_kill(thr3, SIGALRM);
    pthread_kill(thr3, SIGALRM);

    sleep(5);

    pthread_join(thr1, NULL);   /* wait for the threads to complete.*/
    pthread_join(thr2, NULL);
    pthread_join(thr3, NULL);

    printf("main ends\n");

    return 0;
}

void sighandler(int signo)
{
    pthread_t tid = pthread_self();

    printf("thread with pid: %lu receive signo : %d\n", tid, signo);
    return;
}
