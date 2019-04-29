//
// Created by marosm on 3/14/19.
//
#ifndef TETRIS_MAIN_H
#define TETRIS_MAIN_H

///Standard libraries
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <locale.h>

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <conio.h>
#elif defined(__linux__)
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <termios.h>
    #include <stropts.h>

    #include "linuxSpecific.h"
#endif

///Project files
#include "pcg_basic.h"
#include "helper.h"
#include "tetrominos.h"
#include "print.h"
#include "eventHandler.h"
#include "menus.h"
#include "scoreboard.h"

#endif //TETRIS_MAIN_H
