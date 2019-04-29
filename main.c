#include "main.h"

int main (void)
{
    signal(SIGINT, exit_handler);
    game.consoleHeight = getConsoleHeight();
    game.consoleWidth  = getConsoleWidth();
    unsigned long long t_now=0, t_lastRender=0;

    //random init
    pcg32_srandom((uint64_t) time(NULL), (uint64_t) time(NULL)/2);

    //GAME INIT
    game.playAreaX = 10;
    game.playAreaY = 21; //Actual game size in Tetris blocks
    game.state = 1; //Main game loop state
    game.blockFalling = 0; //Block is falling (if not, drop a new one!)
    game.fallTick=0; //We can rotate faster, but drop only so fast!
    game.blockNum=0; //Score tracking and color rotation
    game.blockSequence=0; //Number defining next 7 blocks
    game.fastDrop=false; //press s for the block to fall down fast
    game.instantDrop=false; //space for instant drop
    game.score=0;
    game.fallRate=700; //delay length
    game.level=0;
    game.targetFPS=50;
    game.cyclesPerFrame=0;
    #if defined(__linux__)
        initTermios(0);
    #endif
    #if defined(_WIN32) || defined(WIN32)
        setvbuf(stdout, NULL, _IOLBF, 1); //attemt to fix Windows slow render...
        SetConsoleOutputCP(CP_UTF8);
    #endif
    (void)setlocale(LC_ALL, "en_GB");

    //BOARD INIT
    char **board, **staticBoard;
    board = malloc(game.playAreaY * sizeof(*board)); //allocate board in memory
    staticBoard = malloc(game.playAreaY * sizeof(*staticBoard));
    for (short i=0; i<game.playAreaY; i++)
    {
        board[i] = malloc(game.playAreaX * sizeof *board[i]);
        staticBoard[i] = malloc(game.playAreaX * sizeof *staticBoard[i]);
    }

    for (short i=0; i<game.playAreaY; i++)
    {
        for (short j=0; j<game.playAreaX; j++)
        {
            board[i][j] = ' ';
            staticBoard[i][j] = ' ';
        }
    }

    ///Main menu
    #if defined(_WIN32) || defined(WIN32)
    pprint(" ", "g", GetStdHandle(STD_OUTPUT_HANDLE));
    #elif defined(__linux__)
    pprint(" ", "g", 0);
    #endif
    game.level = mainMenu(game.consoleWidth, game.consoleHeight);
    clearScreen();

    //Default to biggest scaling
    game.scaling=1;
    game.consoleHeight = getConsoleHeight(); //Read them again
    game.consoleWidth  = getConsoleWidth();

    //Windows is too slow to render it bigger
    #if defined(__linux__)
        while(((game.scaling+1)*game.playAreaY + 6 < game.consoleHeight) && ((game.scaling+1)*game.playAreaX + 6 < game.consoleWidth))
            game.scaling++;
    #endif
    //Point[x,y] in console where to start drawing
    game.mainWindowStartX = (unsigned short)((game.consoleWidth /2) - (game.playAreaX*game.scaling));
    game.mainWindowStartY = (unsigned short)((game.consoleHeight/2) - (game.playAreaY*game.scaling/2));

    //GAME LOOP
    while(game.state==1)
    {
        if (!game.blockFalling)
        {
            //Drop new block
            game.blockFalling = true;
            game.blockNum++;
            game.fastDrop = false;
            game.instantDrop = false;
            game.blockRotation = (unsigned short) pcg32_random();
            blockShape = advanceSequence(&game);
            switch (blockShape)
            {
                case shape_I: game.color='w'; break;
                case shape_J: game.color='y'; break;
                case shape_L: game.color='m'; break;
                case shape_O: game.color='b'; break;
                case shape_S: game.color='c'; break;
                case shape_T: game.color='g'; break;
                case shape_Z: game.color='r'; break;
                default: break;
            }
            game.blockX = (short) ((game.playAreaX/2) - 1);
            game.blockY = 0;

            for (short i=0; i<game.playAreaY; i++)
                for (short j=0; j<game.playAreaX; j++)
                    staticBoard[i][j] = board[i][j];

            //Check full rows
            int clearedRows = 0;
            for(int y=game.playAreaY-1; y>0; y--)
            {
                int x=0;
                while ((staticBoard[y][x] != ' ') && (x<game.playAreaX)) x++;
                if (x==game.playAreaX) //Full row found
                {
                    for(int Y=y; Y>0; Y--)
                    {
                        for (int X=0; X<game.playAreaX; X++)
                            staticBoard[Y][X]=staticBoard[Y-1][X];

                    }
                    for (int X=0; X<game.playAreaX; X++) staticBoard[0][X]=' ';
                    y++;
                    clearedRows++;
                }
            }
            switch (clearedRows)
            {
                case 0: break;
                case 1: game.score+=(game.level+1)*40;   break;
                case 2: game.score+=(game.level+1)*100;  break;
                case 3: game.score+=(game.level+1)*300;  break;
                case 4: game.score+=(game.level+1)*1200; break;
                default: break;
            }
        }//new block

        //Key press
        if(!kbhit()) game.fastDrop = false;
        else
        {
            game.pressedKey = parseKey();
            switch(game.pressedKey)
            {
                case 'p': if (game.scaling<3) game.scaling++; goto reScale;
                case 'm': if (game.scaling>1) game.scaling--; goto reScale;
                case 's': game.fastDrop=true; break;
                case ' ': game.instantDrop=true; break;
                case 'w': //Rotate
                {
                    bool rotate=true;
                    game.blockRotation++; //assume legal move

                    for(short y=0; y<4; y++)
                    {
                        for(short x=0; x<4; x++)
                        {
                            //check for collisions
                            if (blockPosition(&game, y, x) && ((x + game.blockX) < 0))
                            {
                                rotate = false;
                                break;
                            }
                            if(x+game.blockX>=game.playAreaX || y+game.blockY>=game.playAreaY)
                            {
                                rotate = false;
                                break;
                            }
                            if(staticBoard[game.blockY+y][game.blockX+x] != ' ')
                            {
                                rotate = false;
                                break;
                            }

                        }
                    }
                    if (!rotate) game.blockRotation--;//revert
                } break;

                case 'a': //Move left
                {
                    bool moveX=true;
                    game.blockX--; //assume legal move
                    for(short y=0; y<4; y++)
                    {
                        for (short x = 0; x < 4; x++)
                        {
                            //check for collisions
                            if (blockPosition(&game, y, x) && (((x + game.blockX) < 0) || (x + game.blockX >= game.playAreaX) || (staticBoard[game.blockY+y][game.blockX+x] != ' ')))
                                moveX = false;
                        }
                    }
                    if (!moveX) game.blockX++;//revert
                } break;

                case 'd': //Move right
                {
                    bool moveX=true;
                    game.blockX++; //assume legal move
                    for(short y=0; y<4; y++)
                    {
                        for (short x = 0; x < 4; x++)
                        {
                            //check for collisions
                            if (blockPosition(&game, y, x) && (((x + game.blockX) < 0) || (x + game.blockX >= game.playAreaX) || (staticBoard[game.blockY+y][game.blockX+x] != ' ')))
                                moveX = false;
                        }
                    }
                    if (!moveX) game.blockX--;//revert
                } break;

                #if defined(NDEBUG)
                default: break;
                #else
                default: printf("%c", game.pressedKey); break;
                #endif

                //Non-reachable code unless you press rescale keys p/m
                reScale:
                    game.consoleHeight = getConsoleHeight();
                    game.consoleWidth  = getConsoleWidth();
                    game.mainWindowStartX = ((game.consoleWidth>>1) - (game.playAreaX*game.scaling)); //Point[x,y] in console where to start drawing
                    game.mainWindowStartY = ((game.consoleHeight>>1) - (game.playAreaY*game.scaling>>1));
                    clearScreen();
            }
        }//keypress

        //Calculate dynamic block
        if (game.fallTick>=game.fallRate || (game.fastDrop && game.fallTick>=25) || game.instantDrop)
        {
            //drop 1 down
            game.blockY++;
            game.fallTick=0;
        }
        
        //Making Board matrix from static and dynamic parts
        ///
        drawBlocks:
        ///
        for (short i=0; i<game.playAreaY; i++)
            for (short j=0; j<game.playAreaX; j++)
                board[i][j] = staticBoard[i][j];

        for (short y=0; y<=game.playAreaY; y++)
        {
            for (short x=0; x<game.playAreaX; x++)
            {
                //radius of block
                if (((x >= game.blockX) && (x < game.blockX+4)) && ((y >= game.blockY) && (y < game.blockY+4)))
                {
                    //Check if there should be a block based on shape and position
                    if (blockPosition(&game, y-game.blockY, x-game.blockX))
                    {
                        //place the block
                        if ((staticBoard[y%game.playAreaY][x] == ' ') && (y != game.playAreaY))
                        {
                            board[y][x] = game.color;
                        }
                        else
                        {
                            //We've hit something
                            if(game.blockY==0) game.state=0;
                            else
                            {
                                if(game.instantDrop) game.fastDrop = false; //Only one bonus is applied
                                game.score+=(game.fastDrop+1)*(2*game.instantDrop+1)*(game.level+1)*4;
                                game.blockY--;
                                game.blockFalling=false;
                                goto drawBlocks; //re-draw
                            }
                        }
                    }
                }
            }
        }

        if(game.instantDrop && game.blockFalling)
        {
            game.blockY++;
            goto drawBlocks; //continue dropping
        }

        if (game.state) ///Still not game over?
        {
            usElapsed(&t_now);
            if(t_now >= t_lastRender+(1e6/(game.targetFPS*1.0))) ///Draw :)
            {
                drawBoard(board, &game);

                game.fallTick += (unsigned)((t_now - t_lastRender)*( 0.9*pow(1.2, 1.4*game.level+1) )/1000.0);
                t_lastRender = t_now;
                game.cyclesPerFrame=0;
            }
            else game.cyclesPerFrame++;
            #if defined(__linux__) //Windows too slow
                msDelay(2); ///Let the CPU sleep for a bit
            #endif
        }

    }//main game loop

    ///Game over
    #if defined(__linux__)
    exitTermios();
    #endif
    clearScreen();
    
    printf("Game over! Thanks for playing.\n\nScore: %u\n", game.score);
    printf("Enter your name: ");
    char name[20];
    fflush(stdin);
    while(!scanf("%19[^\n]", name))
    {
        printf("Incorrect input\n");
        printf("Enter your name: ");
        fflush(stdin);
    }

    if(addScore(game.score, game.level, name)) printf("Error saving your score :(\n");
    displayScoreBoard(15);
    
    printf("\nPress 'q' to exit or 'r' to restart the game\n");
    #if defined(__linux__)
    initTermios(0); kbhit();
    #endif

    do game.pressedKey = getch();
    while (!(game.pressedKey=='q' || game.pressedKey=='r'));

    if(game.pressedKey=='r') main();

    exit_handler(0);
}
