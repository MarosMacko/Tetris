//
// Created by marosm on 2/21/19.
//
#include "linuxSpecific.h"

#if defined(__linux__)
static struct termios termios_old, termios_new;
static bool initialized = false, wasRun = false;

int kbhit(void)
{
    static const int STDIN = 0;

    if (!initialized)
    {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void initTermios(int echo)
{
    if(system("tput civis")) return;
    if (wasRun) return;
    tcgetattr(0, &termios_old); /* grab old terminal i/o settings */
    termios_new = termios_old; /* make new settings same as old settings */
    termios_new.c_lflag &= ~ICANON; /* disable buffered i/o */
    if (echo)
        termios_new.c_lflag |= ECHO; /* set echo mode */
    else
        termios_new.c_lflag &= ~ECHO; /* set no echo mode */

    tcsetattr(0, TCSANOW, &termios_new); /* use these new terminal i/o settings now */

    wasRun=1;
}

inline void exitTermios(void)
{
    tcsetattr(0, TCSANOW, &termios_old);
    initialized=false;
    wasRun=false;
}

inline char getch(void)
{
    char ch;
    ch = (char) getchar();
    return ch;
}

#endif
