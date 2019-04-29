//
// Created by marosm on 2/21/19.
//

#include "helper.h"


char parseKey()
{
    #if defined(__linux__)
    char c = 0;
    while(kbhit()) c = getch(); //Skip escape characters

    switch(c)
    {
        case 'A': //arrow up
            return 'w';
        case 'B': //arrow down
            return 's';
        case 'C': //arrow right
            return 'd';
        case 'D': //arrow left
            return 'a';
        default:
            return c;
    }

    #endif

    #if defined(WIN32) || defined(_WIN32)
    char c = getch();
    switch(c)
    {
        case 'H': //arrow up
            return 'w';
        case 'P': //arrow down
            return 's';
        case 'M': //arrow right
            return 'd';
        case 'K': //arrow left
            return 'a';
        default: return c;
    }
    #endif
}

void msDelay(const unsigned time)
{
    #if defined(_WIN32) || defined(WIN32)
        Sleep(time);
    #endif
    #if defined(__linux__)
        usleep((__useconds_t) time * 1000);
    #endif
}


void usElapsed(unsigned long long *timer)
{
    #if defined(_WIN32) || defined(WIN32)
    LARGE_INTEGER frequency, spec;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&spec);

    *timer = (unsigned long long) spec.QuadPart*1000000 / frequency.QuadPart; //sec + ns to us
    #endif

    #if defined(__linux__)
        struct timespec spec;
        clock_gettime(CLOCK_MONOTONIC, &spec);

        *timer = (unsigned long long) (spec.tv_sec * 1e6) + (spec.tv_nsec / 1000); //sec + ns to us
    #endif
}


inline unsigned short getConsoleHeight()
{
    #if defined(__linux__)
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

inline unsigned short getConsoleWidth()
{
    #if defined(__linux__)
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