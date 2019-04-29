//
// Created by marosm on 3/15/19.
//

#include "eventHandler.h"


void exit_handler(const int sig)
{
    msDelay(10);

    #if defined(__linux__)
    (void)system("clear");
    exitTermios();
    #elif defined(WIN32)
    (void)system("cls");
    #endif

    printf("\nTetris has exited with code %d ", sig);
    switch (sig)
    {
        case 2:     printf("(SIGINT)\n"); break;
        case 0:     printf("(Successful exit)\n"); break;
        default:    printf("(Unknown error)\n"); break;
    }
    msDelay(1000);
    exit(sig);
}
