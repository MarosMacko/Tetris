//
// Created by marosm on 2/21/19.
//

#ifndef TETRIS_LINUXSPECIFIC_H
#define TETRIS_LINUXSPECIFIC_H

#ifdef __linux__

int kbhit();
void initTermios(int echo);
char getch(int echo);

#endif

#endif //TETRIS_LINUXSPECIFIC_H
