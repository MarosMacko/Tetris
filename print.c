//
// Created by marosm on 3/9/19.
//

#include "print.h"

inline void gotoXY(const int x, const int y)
{
    #if defined(__linux__)
        printf("%c[%d;%df",0x1B,y,x);
    #endif
    #ifdef WIN32
        COORD coord;
        coord.X = (short) x;
        coord.Y = (short) y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #endif
}

inline void clearScreen(void)
{
    #if defined(__linux__)
    (void)system("clear");
    #endif
    #if defined(_WIN32) || defined(WIN32)
        system("cls");
    #endif
}

void drawBoard(char ** const board, const struct t_stats *g)
{
    static char borderColor = 'g';
    static unsigned long frame = 0;
    static unsigned frames=0;
    static double fps=0;
    static unsigned long long lastDraw_us=0, now_us=0;
    short X = g->mainWindowStartX;
    short Y = g->mainWindowStartY;

    #if defined(WIN32) || defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    static CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(hConsole, &info);
    #elif defined(__linux__)
    HANDLE hConsole = 0; ///Unused
    #endif

    ///FPS measure
    long long old_now = now_us;
    usElapsed(&now_us);
    if (now_us > lastDraw_us+50)
    {
        fps = (double)frames*1000000 / (double)(now_us-lastDraw_us);
        frames = 0;
        lastDraw_us = now_us;
    }
    frames++;
    if ((frame++)==0) clearScreen();

    gotoXY(0, 0);
    printf("%3.1f FPS (%5.2fms)       ", fps, (now_us-old_now)/1000.0);
    printf("\nScore: %4u", g->score);

    #if !defined(NDEBUG)
    printf("\nCycles per frame: %4u", g->cyclesPerFrame+1); //Game logic cycles
    #endif

    //firstline
    gotoXY(X, Y - 2);
    pprint("╔", &borderColor, hConsole);
    for (short i=0; i < g->playAreaX * g->scaling*2 +2; i++)
        pprint("═", &borderColor, hConsole);
    pprint("╗", &borderColor, hConsole);

    //doubled firstline
    gotoXY(X, Y - 1);
    pprint("║╔", &borderColor, hConsole);
    for (short i=0; i < g->playAreaX * g->scaling*2; i++)
        pprint("═", &borderColor, hConsole);
    pprint("╗║", &borderColor, hConsole);

    ///BOARD
    for (short i=0; i < g->playAreaY * g->scaling; i++)
    {
        gotoXY(X, Y++);
        pprint("║║", &borderColor, hConsole);

        for (short j=0; j < g->playAreaX * g->scaling*2; j++)
        {
            pprint(board[i/g->scaling][j/(g->scaling*2)]==' ' ? " " : "█" , &board[i/g->scaling][j/(g->scaling*2)], hConsole);
        }

        pprint("║║", &borderColor, hConsole);//end of row
    }

    //endl
    gotoXY(X, Y);
    pprint("║╚", &borderColor, hConsole);
    for (short i=0; i < g->playAreaX * g->scaling*2; i++)
        pprint("═", &borderColor, hConsole);
    pprint("╝║\n", &borderColor, hConsole);

    //doubled end
    gotoXY(X, Y + 1);
    pprint("╚", &borderColor, hConsole);
    for (short i=0; i < (g->playAreaX * g->scaling*2 +2); i++)
        pprint("═", &borderColor, hConsole);
    pprint("╝\n", &borderColor, hConsole);
}


///pprint stands for pretty print. Heh.
void pprint(const char *toPrint, const char *color, HANDLE hConsole)
{
    static char msg[10];
    static char lastColor = 'w'; //Call WINAPI only when needed

    #if defined(_WIN32) || defined(WIN32)

    #define BLUE	1
    #define GREEN	2
    #define RED		4

    if(lastColor != color[0])
    {
        lastColor = color[0];
        switch (color[0])
        {
            case 'n':
                SetConsoleTextAttribute(hConsole, 0x0);
                break;
            case 'r':
            case 1 :
                SetConsoleTextAttribute(hConsole, RED);
                break;
            case 'g':
            case 2 :
                SetConsoleTextAttribute(hConsole, GREEN);
                break;
            case 'b':
            case 3 :
                SetConsoleTextAttribute(hConsole, BLUE);
                break;
            case 'c':
            case 4 :
                SetConsoleTextAttribute(hConsole, BLUE | GREEN);
                break;
            case 'm':
            case 5 :
                SetConsoleTextAttribute(hConsole, BLUE | RED);
                break;
            case 'y':
            case 6 :
                SetConsoleTextAttribute(hConsole, GREEN | RED);
                break;
            case 'w':
            case 7 :
                SetConsoleTextAttribute(hConsole, RED | GREEN | BLUE);
                break;
            default:
                break;//SetConsoleTextAttribute(hConsole, 0x07);
        }
    }

    strcpy(msg, toPrint);
    printf("%s", msg);

    #endif

    #if defined(__linux__)

    (void)hConsole;
    static char cPrefix[8] = {0};

    if(lastColor != color[0])
    {
        lastColor = color[0];
        switch (color[0])
        {
            case 'n':
                strcpy(cPrefix, "\x1B[0m");
                break;

            case 'r':
            case 1 :
                strcpy(cPrefix, "\x1B[31m");
                break;

            case 'g':
            case 2 :
                strcpy(cPrefix, "\x1B[32m");
                break;

            case 'b':
            case 3 :
                strcpy(cPrefix, "\x1B[34m");
                break;

            case 'c':
            case 4 :
                strcpy(cPrefix, "\x1B[36m");
                break;

            case 'm':
            case 5 :
                strcpy(cPrefix, "\x1B[35m");
                break;

            case 'y':
            case 6 :
                strcpy(cPrefix, "\x1B[33m");
                break;

            case 'w':
            case 7 :
                strcpy(cPrefix, "\x1B[37m");
                break;
            default:
                break;
        }
    }
    strcpy(msg, cPrefix);
    strcat(msg, toPrint);
    printf("%s", msg);

    #endif
}
