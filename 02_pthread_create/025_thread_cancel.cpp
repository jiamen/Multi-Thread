//
// Created by zlc on 2021/4/27.
//
/*
 * int pthread_cancel(pthread_t tid)
 *   取消tid指定的线程，成功返回0。但是取消只是发送一个请求，并不意味着等待线程终止，而且发送成功也不意味着tid一定会终止
 * int pthread_setcancelstate(int state, int* old_state);
 *   设置本线程对Cancel信号的反应，state有两种值：PTHREAD_CANCEL_ENABLE(缺省)和PTHREAD_CANCEL_DISABLE,
 *                                 分别表示收到信号后设为CANCELED状态和忽略CANCEL信号继续运行；
 *                            old_state如果如果不为NULL则存入原来的Cancel状态以便恢复。
 * int pthread_setcanceltype(int type, int* old_type)
 *   设置本线程取消动作的执行时机，type由两种取值：PTHREAD_CANCEL_DEFFERED和PTHREAD_CANCEL_ASYCHRONOUS,
 *      仅Cancel状态为Enable时有效，分别表示收到信号后继续运行至下一个取消点再退出和立即执行取消动作（退出）；
 *      old_type如果不为NULL则存入原来的取消动作类型值。
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>


void* thread_fun(void* arg)
{
    int state_val;       // stat_eval=0表示设置成功
    // 步骤2：将本线程取消状态设置为不可取消
    state_val = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if ( 0!=state_val )
    {
        printf("set cancel state failed!\n");
    }

    printf("I'm new thread!\n");
    sleep(4);

    printf("about to cancel\n");
    // 还是步骤2：取消状态，设置成为可以取消，这里成为取消点
    state_val = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // state_val = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if ( 0!=state_val )
    {
        printf("set cancel state failed!\n");
    }
    // 步骤3：取消类型，设置为立即响应请求
    int type_val = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if ( 0!=type_val )
    {
        printf("set cancel type failed!\n");
    }
    printf("first cancel point.\n");        // 如果不是立即响应，则执行完这一句之后再取消线程
    printf("second cancel point.\n");

    return (void*)20;
}


int main(int argc, char* *argv)
{
    pthread_t tid;
    int err, cancel_val, join_val;
    void* rval;             // 返回码

    err = pthread_create(&tid, NULL, thread_fun, NULL);
    if ( 0!=err )
    {
        printf("create thread failed!\n");
        return 0;
    }

    sleep(2);       // 等待上面的err线程创建

    cancel_val = pthread_cancel(tid);       // 步骤1：取消函数发送取消请求
    if ( 0!=cancel_val )
    {
        printf("cancel thread failed!\n");
    }

    join_val = pthread_join(tid, &rval);    // 步骤4：取消点

    printf("new thread exit code is %d\n", (int*)rval);
    // 返回码是-1，在/usr/include/pthread.h中查看-1的意思

    return 0;
}



