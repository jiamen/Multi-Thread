
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

