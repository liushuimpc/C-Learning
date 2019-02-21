cmake_minimum_required(VERSION 3.8)
project(fifo_sample)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c fifo_sample.c fifo_sample.h)
add_executable(fifo_sample ${SOURCE_FILES})