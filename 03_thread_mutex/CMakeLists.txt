
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

