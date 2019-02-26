#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef WIN32
    #include <windows.h>
    #include <conio.h>
    #include <time.h>
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
#include "linuxSpecific.h"

#define k(x) 1000*x
#define M(x) 1000000*x


typedef struct
{
    //TODO: Score, time elapsed,
    unsigned short consoleWidth;
    unsigned short consoleHeight;
    unsigned short mainWindowStartX;
    unsigned short mainWindowStartY;
    unsigned short playAreaX;
    unsigned short playAreaY;
    unsigned short scaling;
    unsigned short blockNum;
    unsigned long fallTick;
    char pressedKey;
    char color;
    enum blockShape {cube, line, lShape1, lShape2, sShape1, sShape2, eShape} blockShape;
    unsigned short blockRotation;
    unsigned short blockX;
    unsigned short blockY;
    bool blockFalling;
    bool fastDrop;
    bool state;

} t_stats;


bool blockPosition(unsigned short type, short rotation, unsigned short x, unsigned short y)
{
    rotation+=100;
    rotation%=4;

    t_stats block;
    block.blockShape=type;

    switch (block.blockShape)
    {
        case cube: //
        {
            return ((y<2) && (x<2));
        }
        case line:
        {
            if (rotation%2) return (y==2);
            else return (x==2);
        }
        case lShape1:
        {

        }
        case lShape2:
        {

        }
        case sShape1:
        {

        }
        case sShape2:
        {

        }
        case eShape:
        {
            switch (rotation)
            {
                case 0:
                {

                }
                case 1:
                {

                }
                case 2:
                {

                }
                case 3:
                {

                }
                default: return false;
            }

        }

        default: return false;
    }
}



///<summary>
/// MAIN
/// \return
int main (int argc, char **argv) //int argc, char *argv[] for Windows?
{
    #ifdef WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    t_stats game;

    game.consoleHeight = getConsoleHeight();
    game.consoleWidth  = getConsoleWidth();

    //GAME INIT
    //TODO: Game main menu
    game.playAreaX = 12;
    game.playAreaY = 15; //Actual game size in Tetris blocks
    game.scaling=1; //Scale whole board
    game.mainWindowStartX = ((game.consoleWidth>>1) - (game.playAreaX*game.scaling>>1)); //Point[x,y] in console where to start drawing
    game.mainWindowStartY = ((game.consoleHeight>>1) - (game.playAreaY*game.scaling>>1));
    game.state = 1; //Main game loop state
    game.blockFalling = 0; //Block is falling (if not, drop a new one)
    game.fallTick=0; //We can rotate faster, but drop only so fast!
    game.blockNum=0; //Score tracking and color rotation
    game.fastDrop=false; //press s for the block to fall down fast
    srand((unsigned) time(NULL));

    //BOARD INIT
    char **board, **staticboard;
    board = malloc(game.playAreaY * sizeof *board); //allocate board in memory
    staticboard = malloc(game.playAreaY * sizeof *staticboard);
    for (short i=0; i<game.playAreaY; i++)
    {
        board[i] = malloc(game.playAreaX * sizeof *board[i]);
        staticboard[i] = malloc(game.playAreaX * sizeof *staticboard[i]);
    }

    for (short i=0; i<game.playAreaY; i++)
    {
        for (short j=0; j<game.playAreaX; j++)
        {
            //board[i][j] = (char) ((i + j) % 2 == 0 ? 'g' : 'y');
            board[i][j] = ' ';
            staticboard[i][j] = ' ';
        }
    }

    //GAME LOOP
    while(game.state==1)
    {

        if (!game.blockFalling) //Drop new block
        {
            //board=staticboard;
            game.blockFalling = true;
            game.fastDrop = false;
            game.color = (char) ((game.blockNum++ % 7) + 1);
            game.blockRotation = 1;
            game.blockShape = cube; //(random()%7)+1;
            game.blockX = game.playAreaX >> 1;
            game.blockY = 0;

            for (short i=0; i<game.playAreaY; i++)
                for (short j=0; j<game.playAreaX; j++)
                    staticboard[i][j] = board[i][j];
        }

        if(kbhit()) //Key was pressed
        {
            game.pressedKey = getch();
            switch(game.pressedKey)
            {
                case 'p': if (game.scaling<4) game.scaling++; goto reScale;
                case 'm': if (game.scaling>1) game.scaling--; goto reScale;
                case 's': game.fastDrop=true; break;
                case 'w': game.blockRotation++; break; //TODO: Game crashing when pressing D while landing
                case 'a': if ((game.blockX > 0) && (game.blockFalling)) game.blockX--; break;
                case 'd':
                {
                    if (game.blockX >= game.playAreaX-1) break;
                    bool moveX=true;
                    for (int y=game.blockY; y<game.blockY+4; y++)
                        if (board[y][game.playAreaX-1] != ' ') moveX=false;
                    if (moveX) game.blockX++;
                } break;

                default: printf("%c", game.pressedKey); break;

                reScale:
                    game.consoleHeight = getConsoleHeight();
                    game.consoleWidth  = getConsoleWidth();
                    game.mainWindowStartX = ((game.consoleWidth>>1) - (game.playAreaX*game.scaling>>1)); //Point[x,y] in console where to start drawing
                    game.mainWindowStartY = ((game.consoleHeight>>1) - (game.playAreaY*game.scaling>>1));
                    gotoXY(0,0);
                    for (int y=game.consoleHeight; y>=0; y--)
                        for (int x=game.consoleWidth; x>0; x--)
                            printf(" ");
            }
        }

        //Calculate dynamic block
        if (game.fallTick>=800 || game.fastDrop)
        {
            //drop 1 down
            game.blockY++;
            game.fallTick=0;
        }

        drawBlocks:
        for (short i=0; i<game.playAreaY; i++)
            for (short j=0; j<game.playAreaX; j++)
                board[i][j] = staticboard[i][j];

        for (short y=0; y<=game.playAreaY; y++)
        {
            for (short x=0; x<game.playAreaX; x++)
            {
                bool block;
                //radius of block
                if (((x >= game.blockX) && (x < game.blockX+4)) && ((y >= game.blockY) && (y < game.blockY+4)))
                {
                    //Check if there should be a block based on shape and position
                    block = blockPosition(game.blockShape, game.blockRotation, x-game.blockX, y-game.blockY);
                    if (block)
                    {
                        if ((staticboard[y%game.playAreaY][x] == ' ') && (y != game.playAreaY))
                        {
                            board[y][x] = game.color;
                        }
                        else
                        {
                            //We've hit something
                            game.blockY--;
                            game.blockFalling=false;
                            goto drawBlocks;
                        }
                    }
                }
            }
        }

        drawBoard(board, game.playAreaX, game.playAreaY, game.mainWindowStartX, game.mainWindowStartY, game.scaling);
        //TODO: make this var, make starting-diff and var-diff
        msDelay(100);
        game.fallTick+=100;

    }//main game loop


    //TODO: Game over screen, score etc
    /* deallocate the array */
    for (short i=0; i<game.consoleHeight; i++) free(board[i]);
    free(board);

    return 0;
}
