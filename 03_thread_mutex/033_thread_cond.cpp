//
// Created by zlc on 2021/4/28.
//
// Description: 生产者与消费者问题

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>         // sleep函数


#define BUFFER_SIZE  5      // 产品库存大小
#define PRODUCT_CNT  30     // 产品生产总数


struct product_cons
{
    int buffer[BUFFER_SIZE];    // 生产产品值
    pthread_mutex_t lock;       // 互斥锁  volatile int
    int read_pos, write_pos;      // 读写位置
    pthread_cond_t not_empty;    // 条件变量，非空
    pthread_cond_t not_full;     // 非满
} buffer;


void init(struct product_cons* p)
{
    pthread_mutex_init(&p->lock, NULL);         // 互斥锁初始化
    pthread_cond_init(&p->not_empty, NULL);     // 条件变量初始化
    pthread_cond_init(&p->not_full, NULL);      // 条件变量初始化
    p->read_pos = 0;                                    // 读写变量
    p->write_pos = 0;
}

void finish(struct product_cons* p)
{
    pthread_mutex_destroy(&p->lock);        // 互斥锁
    pthread_cond_destroy(&p->not_empty);    // 条件变量
    pthread_cond_destroy(&p->not_full);     // 条件变量
    p->read_pos = 0;                        // 读写位置
    p->write_pos = 0;
}

// 生产者写，存储一个数据到buffer
void put(struct product_cons* p, int data)  // 输入产品子函数
{
    pthread_mutex_lock(&p->lock);
    // 读的位置和写的位置始终要差一个
    if ( (p->write_pos+1)%BUFFER_SIZE == p->read_pos )
    {
        printf("producer wait for not full!\n");    // 生产者等待不满的时候再生产
        pthread_cond_wait(&p->not_full, &p->lock);
    }

    p->buffer[p->write_pos] = data;
    p->write_pos ++;

    if ( p->write_pos >= BUFFER_SIZE )
        p->write_pos = 0;

    pthread_cond_signal(&p->not_empty);     // 不空，发送信号通知读操作来读取数据
    pthread_mutex_unlock(&p->lock);
}

// 消费者读，从 buffer 移除一个数据
int get(struct product_cons* p)
{
    int data;
    pthread_mutex_lock(&p->lock);

    if (p->read_pos == p->write_pos)
    {
        printf("consumer wait for not empty");      // 消费者等待不空的时候再消耗
        pthread_cond_wait(&p->not_empty, &p->lock);
    }

    data = p->buffer[p->read_pos];
    p->read_pos ++;

    if ( p->read_pos >= BUFFER_SIZE )
        p->read_pos = 0;
    pthread_cond_signal(&p->not_full);

    pthread_mutex_unlock(&p->lock);

    return data;
}

// 子线程，生产者每隔1秒产生一个数据
void* producer(void* data)
{
    int n;
    for ( n=1; n<=50; n ++ )  // 生产50个商品
    {
        sleep(1);
        printf("put the %d product ...\n", n);
        put(&buffer, n);
        printf("put the %d product success\n", n);
    }
    printf("producer stopped\n");

    return NULL;
}

// 子线程，消费者每隔2s取出一个数据
void* consumer(void* data)
{
    static int cnt = 0;
    int num;        // 每次取得的数据

    while (1)
    {
        sleep(2);
        printf("get product ...\n");
        num = get(&buffer);
        printf("get the %d product success\n", num);
        if ( ++cnt == PRODUCT_CNT )     // 产品生产总数
            break;
    }

    printf("consumer stopped\n");
    return NULL;
}


int main(int argc, char* *argv)
{
    pthread_t th_a, th_b;
    void* ret_val;

    init(&buffer);

    pthread_create(&th_a, NULL, producer, 0);
    pthread_create(&th_b, NULL, consumer, 0);

    pthread_join(th_a, &ret_val);
    pthread_join(th_b, &ret_val);

    finish(&buffer);

    return 0;
}

