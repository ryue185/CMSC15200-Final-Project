#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "board.h"


enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


struct game {
    unsigned int run, hangtime;
    board* b;
    posqueue* hanging;
    turn player;
};

typedef struct game game;

// creates a new game where the board is a new board
// sets the posqueue as a new posqueue
// sets the turn as BLACK_TURN
game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type);

// frees a game
// free its associated board
// and frees its posqueue
void game_free(game* g);

// places a piece at the appropriate location on the board
// enqueues the new move to hanging
// then dequeues a move from hanging
// and drops the dequeued move down
bool place_piece(game* g, pos p);

// iterate through each cell of the board
// when encountering empty cells, mark the game as possibly ongoing
// and then moves on
// when encounting non-empty cells,
// checks if it is part of a run with the functions below
// if so return win info as appropriate
outcome game_outcome(game* g);

// helper, drops down a cell to the next possible place
//i.e. either on top of another non-empty cell, or to the bottom
int drop_down(game* g, pos todrop);

// helper, checks if the pos in some run
bool in_four(game* g, pos p);
#endif /* LOGIC_H */
