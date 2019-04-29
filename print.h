//
// Created by marosm on 3/9/19.
//
#ifndef TETRIS_PRINT_H
#define TETRIS_PRINT_H

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tetrominos.h"

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

    //No console handle for linux
    #define HANDLE int
#endif

#include "helper.h"

void gotoXY(int x,int y);
void clearScreen(void);

/**
 * Pretty print
 * @param toPrint - string to print
 * @param color - one char representing the color (rgbcmyw,n)
 */
void pprint(const char *toPrint, const char *color, HANDLE hConsole);
void drawBoard(char ** board, const struct t_stats *g);

#endif //TETRIS_PRINT_H