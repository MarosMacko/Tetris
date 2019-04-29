//
// Created by marosm on 3/16/19.
//

#ifndef TETRIS_MENUS_H
#define TETRIS_MENUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined(WIN32) || defined(_WIN32)
#include <conio.h>
#elif defined(__linux__)
#include "linuxSpecific.h"
#endif

/**
 *
 * @param maxX
 * @param maxY
 * @return level select
 */
unsigned short mainMenu(int maxX, int maxY);

#endif //TETRIS_MENUS_H
