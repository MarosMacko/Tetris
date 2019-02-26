//
// Created by marosm on 2/21/19.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef WIN32
    #include <windows.h>
    #include <conio.h>
#endif
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <time.h>
#endif

#include "helper.h"

HANDLE console;

void pprint(char *toPrint, char *color)
{

    #ifdef WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &info);
        #define BLUE			1
        #define GREEN			2
        #define RED				4
    #endif
    char cPrefix[8] = {0}, msg[200];

    #ifdef WIN32
    switch(color[0])
    {
        case 'n':
            SetConsoleTextAttribute(hConsole, 0x0); break;
        case 'r': case  1 :
            SetConsoleTextAttribute(hConsole, RED); break;
        case 'g': case  2 :
            SetConsoleTextAttribute(hConsole, GREEN); break;
        case 'b': case  3 :
            SetConsoleTextAttribute(hConsole, BLUE); break;
        case 'c': case  4 :
            SetConsoleTextAttribute(hConsole, BLUE | GREEN); break;
        case 'm': case  5 :
            SetConsoleTextAttribute(hConsole, BLUE | RED); break;
        case 'y': case  6 :
            SetConsoleTextAttribute(hConsole, GREEN | RED); break;
        case 'w': case  7 :
            SetConsoleTextAttribute(hConsole, RED | GREEN | BLUE); break;
        default:
            SetConsoleTextAttribute(hConsole, 0x07);
    }
    #endif

    #ifdef __linux__
    switch(color[0])
    {
        case 'n':
            strcpy(cPrefix, "\x1B[0m"); break;

        case 'r': case  1 :
            strcpy(cPrefix, "\x1B[31m"); break;
        case 'g': case  2 :
            strcpy(cPrefix, "\x1B[32m"); break;
        case 'b': case  3 :
            strcpy(cPrefix, "\x1B[34m"); break;

        case 'c': case  4 :
            strcpy(cPrefix, "\x1B[36m"); break;

        case 'm': case  5 :
            strcpy(cPrefix, "\x1B[35m"); break;

        case 'y': case  6 :
            strcpy(cPrefix, "\x1B[33m"); break;

        case 'w': case  7 :
            strcpy(cPrefix, "\x1B[37m"); break;
        default:
            strcpy(cPrefix, "\x1B[0m"); break;
    }
    #endif

    strcpy(msg, cPrefix);
    strcat(msg, toPrint);
    //strcat(msg, "\x1B[0m");

    for (int c = 0; msg[c] != '\0'; c++) printf("%c", msg[c]);

}

void msDelay(int time) {
#ifdef WIN32
    sleep((unsigned int) (time / 1000));
#endif
#ifdef __linux__
    usleep((__useconds_t) time * 1000);
#endif
}

void gotoXY(int x, int y)
{
    #ifdef __linux__
        printf("%c[%d;%df",0x1B,y,x);
    #endif
    #ifdef WIN32
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #endif

}

void drawBoard(char **board, short maxX, short maxY, short X, short Y, short scale)
{
    static char borderColor = 'g';
    static unsigned long frame = 0;

    #ifdef __linux__
        system("clear");
    #endif
    #ifdef WIN32
        COORD pos = { 0, 0};
        DWORD written;
        unsigned size;

        size = maxX * maxY;

        FillConsoleOutputCharacter(console, ' ', size, pos, &written);
        FillConsoleOutputAttribute(console, 0x71, size, pos, &written);
        //SetConsoleCursorPosition(console, pos);
    #endif

    gotoXY(0, 0);
    printf("%ld\n", ++frame);

    //firstline
    gotoXY(X, Y - 2);
    pprint("╔", &borderColor);
    for (short i=0; i<maxX*scale+2; i++)
        pprint("═", &borderColor);
    pprint("╗", &borderColor);

    //doubled firstline
    gotoXY(X, Y - 1);
    pprint("║╔", &borderColor);
    for (short i=0; i<maxX*scale; i++)
        pprint("═", &borderColor);
    pprint("╗║", &borderColor);

    //BOARD
    for (short i=0; i<maxY*scale; i++)
    {
        gotoXY(X, Y++);
        pprint("║║", &borderColor);

        for (short j=0; j<maxX*scale; j++)
        {
            pprint(board[i/scale][j/scale]==' ' ? " " : "█" , &board[i/scale][j/scale]);
        }

        pprint("║║\n", &borderColor);
    }

    //endl
    gotoXY(X, Y);
    pprint("║╚", &borderColor);
    for (short i=0; i<maxX*scale; i++)
        pprint("═", &borderColor);
    pprint("╝║\n", &borderColor);

    //doubled
    gotoXY(X, Y + 1);
    pprint("╚", &borderColor);
    for (short i=0; i<maxX*scale+2; i++)
        pprint("═", &borderColor);
    pprint("╝\n", &borderColor);
}

unsigned short getConsoleHeight()
{
    #ifdef __linux__
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
    #endif
    #ifdef WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #endif
}

unsigned short getConsoleWidth()
{
    #ifdef __linux__
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    #endif
    #ifdef WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #endif
}