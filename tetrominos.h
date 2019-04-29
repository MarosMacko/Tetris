//
// Created by marosm on 3/15/19.
//

#ifndef TETRIS_TETROMINOS_H
#define TETRIS_TETROMINOS_H

#include <stdbool.h>
#include <stdint.h>
#include "pcg_basic.h"

//Number of different shapes
#define SHAPES 7

enum {shape_O, shape_I, shape_L, shape_J, shape_S, shape_Z, shape_T} blockShape;

extern struct t_stats
{
    unsigned short consoleWidth;
    unsigned short consoleHeight;
    unsigned short mainWindowStartX;
    unsigned short mainWindowStartY;
    unsigned short playAreaX;
    unsigned short playAreaY;
    unsigned char scaling;
    unsigned int targetFPS;
    unsigned int cyclesPerFrame;
    char pressedKey;
    char color;

    unsigned long fallTick;
    unsigned long blockSequence;
    unsigned short blockRotation;
    unsigned short fallRate;
    unsigned short level;
    unsigned short blockNum;
    unsigned int score;
    short blockX;
    short blockY;

    bool blockFalling;
    bool fastDrop;
    bool instantDrop;
    bool state;

} t_stats;

struct t_stats game;


/**
 * Does the square contain any Tetromino piece?
 * @param g - game struct
 * @param y
 * @param x
 * @return
 */
bool blockPosition(struct t_stats *g, short y, short x);

/**
 * Tetrominos fall randomly, but always in sequence of 7, where each one appears once
 * This guarantees tight pseudo-random distribution - equal distribution on small sample size
 * @param g
 */
void shuffleSequence(struct t_stats *g);
int advanceSequence(struct t_stats *g);

#endif //TETRIS_TETROMINOS_H
