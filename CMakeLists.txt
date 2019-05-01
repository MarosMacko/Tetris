cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(.)

add_executable(Tetris
    CMakeLists.txt
    main.c
    main.h
    eventHandler.c
    eventHandler.h
    helper.c
    helper.h
    linuxSpecific.c
    linuxSpecific.h
    menus.c
    menus.h
    pcg_basic.c
    pcg_basic.h
    print.c
    print.h
    tetrominos.c
    tetrominos.h
    scoreboard.c
    scoreboard.h)

target_link_libraries(Tetris m)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra -Werror -O0 -g3")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O2 -g0")
endif()
