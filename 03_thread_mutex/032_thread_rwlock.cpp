//
// Created by zlc on 2021/4/28.
//
/*
 * Description: 验证可以有多个线程同时拥有读模式下的读写锁
 *
 * 读写锁在使用之前必须初始化：
 *      int pthread_rwlock_init(pthread_rwlock *restrict rwlock,
 *                      const pthread_rwlockattr_t *restrict attr)
 *  成功返回0，失败返回错误码。
 *
 *  使用完需要销毁：
 *      int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
 *  成功返回0，失败返回错误码。
 *
 *  读模式加锁：
 *      int pthread_rwlock(pthread_rwlock_t *rwlock);
 *      int pthread_rwlock_trylock(pthread_rwlock_t *rwlock);
 *  写模式加锁：
 *      int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
 *      int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
 *
 *  解锁：
 *      int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数

pthread_rwlock_t rwlock;    // 定义读写锁
int num = 0;

void* thread_fun1(void* arg)
{
    int err;

    pthread_rwlock_rdlock(&rwlock);     // 读模式加锁
    // pthread_rwlock_wrlock(&rwlock);     // 写模式加锁
    printf("thread 1 print num %d\n", num);

    sleep(3);
    printf("thread 1 over!\n");

    pthread_rwlock_unlock(&rwlock);     // 读模式解锁

    return (void*)1;
}

void* thread_fun2(void* arg)
{
    int err;

    pthread_rwlock_rdlock(&rwlock);     // 读模式加锁
    // pthread_rwlock_wrlock(&rwlock);     // 写模式加锁
    printf("thread 2 print num %d\n", num);

    sleep(3);
    printf("thread 2 over!\n");

    pthread_rwlock_unlock(&rwlock);     // 读模式解锁

    return (void*)1;
}

int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;
    int err;

    pthread_rwlock_init(&rwlock, NULL);             // 属性默认为NULL
    if ( err )
    {
        printf("init rwlock failed\n");
        return 0;
    }
    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    if ( err )
    {
        printf("create new thread 1 failed\n");
        return 0;
    }

    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if ( err )
    {
        printf("create new thread 2 failed\n");
        return 0;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}


