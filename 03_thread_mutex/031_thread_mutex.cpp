//
// Created by zlc on 2021/4/27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数


struct student
{
    int id;
    int age;
    int name;
}stu;

// 定义两个全局变量
int i;
pthread_mutex_t mutex;


// 线程1启动函数
void* thread_fun1(void* arg)
{
    while (1)
    {
        // 加锁，对整个结构体访问进行加锁，防止产生错乱
        pthread_mutex_lock(&mutex);
        stu.id = i;
        stu.age = i;
        stu.name = i;
        i ++;
        // 因为上面的三个值都相同，在读写过程中如果出现3个值的读写不同步进行的话，值的大小应该会出现互不相等的情况
        if ( stu.id!=stu.age || stu.id!=stu.name || stu.age!=stu.name )
        {
            printf("%d, %d, %d\n", stu.id, stu.age, stu.name);
            break;
        }
        // 访问变量完成需要解锁，只有这样其他线程才能访问
        pthread_mutex_unlock(&mutex);
    }

    return (void*)0;
}

// 线程2启动函数
void* thread_fun2(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        stu.id = i;
        stu.age = i;
        stu.name = i;

        // 因为上面的三个值都相同，在读写过程中如果出现3个值的读写不同步进行的话，值的大小应该会出现互不相等的情况
        if ( stu.id!=stu.age || stu.id!=stu.name || stu.age!=stu.name )
        {
            printf("%d, %d, %d\n", stu.id, stu.age, stu.name);
            break;
        }
        i ++;
        pthread_mutex_unlock(&mutex);
    }

    return (void*)0;
}


int main(int argc, char* *argv)
{
    pthread_t tid1, tid2;
    int err;

    // 对互斥量进行初始化，只有初始化过后互斥量才能使用
    err = pthread_mutex_init(&mutex, NULL);
    if ( 0 != err )
    {
        printf("create new pthread_mutex_init failed!\n");
        return 0;
    }

    // 创造新线程
    err = pthread_create(&tid1, NULL, thread_fun1, NULL);
    if ( 0!=err )
    {
        printf("create new thread failed!\n");
        return 0;
    }

    // 创造新线程
    err = pthread_create(&tid2, NULL, thread_fun2, NULL);
    if ( 0!=err )
    {
        printf("create new thread failed!\n");
        return 0;
    }

    // sleep(2);

    // 等待新线程运行结束，这俩函数执行完主线程main才能结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}


