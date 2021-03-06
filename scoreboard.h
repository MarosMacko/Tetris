//
// Created by marosm on 3/27/19.
//

#ifndef TETRIS_SCOREBOARD_H
#define TETRIS_SCOREBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dlnBoard
{
    char time[20];
    char name[20];
    unsigned long score;
    unsigned level;
    struct dlnBoard * next;
    struct dlnBoard * prev;
};

/**
 * Create the scoreboard if it does not exist
 * @return success
 */
int initializeScoreboard(void);


/**
 * Print the doubly linked list
 * @param board
 */
void printBoard(struct dlnBoard * board, int n);


/**
 * Display first n people
 * @return
 */
int displayScoreBoard(int size);


/**
 * Appends a person to scoreboard
 * @param score
 * @param name
 * @return success
 */
int addScore(int score, unsigned level, const char* name);

#endif //TETRIS_SCOREBOARD_H
