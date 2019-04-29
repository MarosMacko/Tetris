//
// Created by marosm on 2/21/19.
//

#ifndef TETRIS_LINUXSPECIFIC_H
#define TETRIS_LINUXSPECIFIC_H

#if defined(__linux__)
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stropts.h>
#include <time.h>
#include <stdbool.h>

/**
 * @return Keyboard hit (bool)
 */
int kbhit(void);

/**
 * Getch for Linux
 * @return
 */
char getch(void);


void initTermios(int echo);
void exitTermios(void);

#endif
#endif //TETRIS_LINUXSPECIFIC_H
