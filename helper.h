//
// Created by marosm on 2/21/19.
//

#ifndef TETRIS_HELPER_H
#define TETRIS_HELPER_H

///<summary>
/// Pretty print any string with desired color
/// \param toPrint
/// \param color

void pprint(char *toPrint, char *color);
void msDelay(int time);
void gotoXY(int x, int y);
void drawBoard(char **board, short maxX, short maxY, short X, short Y, short scale);

unsigned short getConsoleHeight();
unsigned short getConsoleWidth();

#endif //TETRIS_HELPER_H
