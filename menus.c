//
// Created by marosm on 3/16/19.
//

#include "menus.h"

unsigned short mainMenu(const int maxX, const int maxY)
{
    printf("\n\n\n\n\n");

    for(int i=0; i<6; i++)
    {
        for(int w=0; w<(maxX-45)/2; w++) printf(" ");

        switch (i)
        {
            case 0: printf("████████╗███████╗████████╗██████╗ ██╗███████╗"); break;  //45 chars wide
            case 1: printf("╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝"); break;
            case 2: printf("   ██║   █████╗     ██║   ██████╔╝██║███████╗"); break;
            case 3: printf("   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║"); break;
            case 4: printf("   ██║   ███████╗   ██║   ██║  ██║██║███████║"); break;
            case 5: printf("   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝"); break;
            default: break;
        }
        printf("\n");
    }

    const int numText = 7;
    const char *text[numText];

    text[0] = "Welcome to Tetris (v1.0) by Maros Macko\n";
    text[1] = "**Instructions**\n";
    text[2] = "I recommend resizing the console (best experience in fullscreen)";
    text[3] = "Press 'm' to zoom out or 'p' to zoom in the board\n";
    text[4] = "Use keys or WASD (rotate, move, fast drop) and spacebar for instant drop";
    text[5] = "Score per block/line clear scales with game level\n\n";
    text[6] = "Press 0-9 to choose the level and start the game";

    for(int i=0; i<numText; i++)
    {
        for(unsigned w=0; w<(maxX-strlen(text[i]))/2; w++) printf(" ");
        printf("%s\n",text[i]);
    }

    if(maxY<26) printf("!Please enlarge your console, it seems to be too small to render correctly!");

    for(int i=0; i<maxY-20; i++) printf("\n");

    char c=0;
    while(!isdigit(c)) //We expect level select
        c = getch();

    return (c - '0');
}
