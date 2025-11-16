cmake_minimum_required(VERSION 3.10)
project(TCP_client C)

add_executable(tcp-client tcp-client.c)

if(WIN32)
    target_link_libraries(tcp-client PRIVATE ws2_32)
endif()