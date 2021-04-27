
cmake_minimum_required(VERSION 2.8)

project(pthread_create)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall" )
set( CMAKE_BUILD_TYPE "Release" )

add_executable(pthread_create 021_pthread_create.cpp)
target_link_libraries(pthread_create -lpthread)

# 主线程的特殊性
add_executable(main_thread 022_main_thread.cpp)
target_link_libraries(main_thread -lpthread)

# 线程退出
add_executable(thread_exit 023_thread_exit.cpp)
target_link_libraries(thread_exit -lpthread)

# 线程连接和分离
add_executable(thread_join 024_thread_join.cpp)
target_link_libraries(thread_join -lpthread)

# 线程取消
add_executable(thread_cancel 025_thread_cancel.cpp)
target_link_libraries(thread_cancel -lpthread)


# 向线程发送信号
add_executable(thread_kill1 026_thread_kill1.cpp)
target_link_libraries(thread_kill1 -lpthread)
add_executable(thread_kill2 027_thread_kill2.cpp)
target_link_libraries(thread_kill2 -lpthread)



