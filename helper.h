//
// Created by marosm on 2/21/19.
//
#ifndef TETRIS_HELPER_H
#define TETRIS_HELPER_H

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <conio.h>
#endif
#if defined(__linux__)
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <termios.h>
    #include <stropts.h>
    #include <time.h>
    #include "linuxSpecific.h"
#endif

void msDelay(unsigned time);
unsigned short getConsoleHeight();
unsigned short getConsoleWidth();
char parseKey();


/**
 * Returns microseconds from the program start (real-time)
 * @param timer* ULL
 */
void usElapsed(unsigned long long *timer);

#endif //TETRIS_HELPER_H
