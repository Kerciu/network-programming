cmake_minimum_required(VERSION 3.10)
project(udp_sockets C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(udp-server-c udp-server.c)

add_executable(udp-client-c udp-client.c)

if(WIN32)
    target_link_libraries(udp-server-c PRIVATE Ws2_32)
    target_link_libraries(udp-client-c PRIVATE Ws2_32)
endif()