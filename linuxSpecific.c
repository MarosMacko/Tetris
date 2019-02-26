//
// Created by marosm on 2/21/19.
//

#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#endif

#include "linuxSpecific.h"

#ifdef __linux__


int kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
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

void initTermios(int echo) {
    static struct termios old, new;
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    if (echo) {
        new.c_lflag |= ECHO; /* set echo mode */
    } else {
        new.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

char getch()
{
    char ch;
    initTermios(0);
    ch = (char) getchar();
    return ch;
}

#endif
