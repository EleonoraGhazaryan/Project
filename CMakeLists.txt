cmake_minimum_required (VERSION 3.16)
project(crawler)

set(SAMPLE_NAME "crawler")

set(SAMPLE_SRCS "crawler.cpp")

# depend libraries
find_package(CURL REQUIRED)

add_executable(${SAMPLE_NAME} ${SAMPLE_SRCS} linkqueue.cpp repo_in_memory.cpp indexer.cpp server.cpp) 
target_link_libraries(${SAMPLE_NAME}
                       -lcurl
                      )