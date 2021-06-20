cmake_minimum_required (VERSION 3.16)
project(crawler)

set(SAMPLE_NAME "crawler")

set(SAMPLE_SRCS "crawler.cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")
# depend libraries
#set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

add_executable(${SAMPLE_NAME} ${SAMPLE_SRCS} linkqueue.cpp repo_in_memory.cpp indexer.cpp server.cpp indexer.cpp) 
target_link_libraries(${SAMPLE_NAME} -pthread -lcurl)