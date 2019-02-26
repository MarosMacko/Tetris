cmake_minimum_required(VERSION 3.12)
project(Tetris C)

set(CMAKE_C_STANDARD 11)

add_executable(Tetris
        main.c helper.c helper.h linuxSpecific.c linuxSpecific.h)
