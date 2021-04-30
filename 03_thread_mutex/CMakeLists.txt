
cmake_minimum_required(VERSION 2.8)

project(pthread_mutex)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall" )
set( CMAKE_BUILD_TYPE "Release" )

add_executable(mutex mutex.cpp)
target_link_libraries(mutex -lpthread)


# 互斥锁实例
add_executable(thread_mutex 031_thread_mutex.cpp)
target_link_libraries(thread_mutex -lpthread)

# 读写锁实例
add_executable(thread_rwlock 032_thread_rwlock.cpp)
target_link_libraries(thread_rwlock -lpthread)


# 条件变量：生产者、消费者模式
add_executable(thread_cond 033_thread_cond.cpp)
target_link_libraries(thread_cond -lpthread)


# 一次性初始化
add_executable(thread_once 034_thread_once.cpp)
target_link_libraries(thread_once -lpthread)

# 分离属性实例：setdetachstate
add_executable(thread_attr_detach 035_thread_attr_detach.cpp)
target_link_libraries(thread_attr_detach -lpthread)

# 线程栈属性
add_executable(thread_attr_stack 036_thread_attr_stack.cpp)
target_link_libraries(thread_attr_stack -lpthread)

add_executable(thread_mutexattr 037_thread_mutexattr.cpp)
target_link_libraries(thread_mutexattr -lpthread -lrt)

# 线程私有数据
add_executable(thread_private 038_thread_private.cpp)
target_link_libraries(thread_private -lpthread -lrt)
# 线程的私有数据，一个像errno一样的数据
add_executable(thread_specific 039_thread_specific.cpp)
target_link_libraries(thread_specific -lpthread -lrt)


# 安全的fork进程
# 1.主进程中加锁，子进程中想要加锁，会陷入死锁
add_executable(thread_fork1 040_thread_fork1.cpp)
target_link_libraries(thread_fork1 -lpthread -lrt)
# 2.主进程中加锁，子进程顺利解锁执行
add_executable(thread_fork2 041_thread_fork2.cpp)
target_link_libraries(thread_fork2 -lpthread -lrt)
# 3.pthread_atfork()函数
add_executable(thread_fork3 042_thread_fork3.cpp)
target_link_libraries(thread_fork3 -pthread -lrt)



