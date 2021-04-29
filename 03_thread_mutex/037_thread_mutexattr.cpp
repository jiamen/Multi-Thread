//
// Created by zlc on 2021/4/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数
#include <fcntl.h>

#include <sys/mman.h>       // 使用shm_open来操作共享内存


int main ()
{
    pid_t fpid;     // fpid表示fork函数返回的值
    int count_data = 0;
    int* count = &count_data;
    *count = 0;

    char* shm = "my_shm";
    char* shm1 = "my_shm1";
    char* shm_count = "my_shm_count";
    int shm_count_id, shm_id, shm_id1;
    char* buf;


    pthread_mutex_t* mutex;                 // 互斥量
    pthread_mutexattr_t mutexattr;          // 互斥量属性

    // 打开共享内存
    // shm_id = shm_open(shm, O_RDWR|O_CREAT, 0644);
    shm_id1       = shm_open(shm1, O_RDWR|O_CREAT, 0644);
    shm_count_id  = shm_open(shm_count, O_RDWR|O_CREAT, 0644);          // 用来修改count
    // 调整共享内存大小
    ftruncate(shm_id1, 100);
    ftruncate(shm_count_id,  100);
    // 映射共享内存，MAP_SHARED属性表明，对共享内存的任何修改都会影响到其他进程
    // buf = (char *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);
    mutex = (pthread_mutex_t *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id1, 0);
    count = (int *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_count_id, 0);
    *count = 0;         // 这里不置0会出现打印地址的情况

    // 打开共享内存
    shm_id = shm_open(shm, O_RDWR|O_CREAT, 0644);
    // 调整共享内存大小
    ftruncate(shm_id, 100);
    // 映射共享内存，MAP_SHARED属性表明，对共享内存的任何修改都会影响到其他进程
    buf = (char *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);

    pthread_mutexattr_init(&mutexattr);     // 互斥量属性初始化
#ifdef _POSIX_THREAD_PROCESS_SHARED
    // ，这个属性可以使互斥量在多个进程中进行同步
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
#endif
    pthread_mutex_init(mutex, &mutexattr);



    fpid = fork();
    if (fpid < 0)
        printf("error in fork!");
    else if (fpid == 0)
    {
        // 休眠1s，让父进程先运行
        sleep(1);
        printf("i am the child process, my process id is %d\n", getpid());
        printf("我是爹的儿子\n");         // 对某些人来说中文看着更直白。

        pthread_mutex_lock(mutex);
        memcpy(buf, "hello", 6);    // 将共享内存修改为hello
        *count = *count + 1;

        printf("子线程buff：%s\n", buf);
        pthread_mutex_unlock(mutex);
    }
    else
    {
        printf("i am the parent process, my process id is %d\n", getpid());
        printf("我是孩子他爹\n");
        sleep(2);

        pthread_mutex_lock(mutex);
        memcpy(buf, "world", 6);    // 将共享内存的内容，改为world
        *count = *count + 1;
        printf("父线程buff：%s\n", buf);
        pthread_mutex_unlock(mutex);
    }

    printf("统计结果是: %d\n", *count);

    // 释放互斥量属性变量和互斥量变量
    pthread_mutexattr_destroy(&mutexattr);
    pthread_mutex_destroy(mutex);

    // 解除映射
    munmap(count, 100);
    munmap(buf, 100);
    munmap(mutex, 100);

    // 消除共享内存
    shm_unlink(shm);
    shm_unlink(shm1);
    shm_unlink(shm_count);

    return 0;
}



// fork（）函数通过系统调用创建一个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事，
// 但如果初始参数或者传入的变量不同，两个进程也可以做不同的事。


