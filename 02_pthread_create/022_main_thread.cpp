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
 * 第三个参数，新线程到启动函数
 * 第四个参数，传递给新线程
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

// 结构体
struct student
{
    int age;
    char name[20];
    char id[5];
};      // 身份证号

void* thread_fun(void* stu)
{
    printf("student age is %d, name is %s, id=%s\n", ((struct student*)stu)->age, ((struct student*)stu)->name, ((struct student*)stu)->id);
    //printf("student age is %d, name is %s, id=%s\n", ((struct student*)stu)->age, ((struct student*)stu)->name);

    return (void*)0;
}


int main(int argc, char* *argv)
{
    pthread_t tid;
    int err;
    int* rval;      // pthread_exit()函数自动回填

    struct student stu;
    stu.age = 20;
    memcpy(stu.name, "zhangsan", 20);
    // 注意这里不是C++，不要直接使用“=”赋值
    memcpy(stu.id, "0007", strlen("0007")+1);
    printf("strlen(\"0007\") = %d\n", strlen("0007"));


    err = pthread_create(&tid, NULL, thread_fun, (void*)(&stu));
    if (0 != err)
    {
        printf("create new thread failed!\n");
        return 0;
    }

    printf("main thread have %d args\n", argc);
    for(int i=0; i<argc; i ++)
    {
        printf("main thread args is %s\n", argv[i]);
    }

    // sleep(1);   // 防止直接使用return 0结束进程，可以删掉这句，就会发现无法创建std新线程
    // return 0;

    pthread_exit(rval);
}
