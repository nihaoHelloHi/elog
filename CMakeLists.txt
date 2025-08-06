cmake_minimum_required(VERSION 3.16)
project(elog C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(elog
        elog.c
        elog.h)
