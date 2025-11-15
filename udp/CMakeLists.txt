cmake_minimum_required(VERSION 4.0)
project(udp C)

set(CMAKE_C_STANDARD 11)

add_executable(udp main.c
        datagram.h
        common.h
        udp-server.c)
