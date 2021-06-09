#ifndef BOARD_H
#define BOARD_H
#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

// creates a new board

// array filled with EMPTY
board* board_new(unsigned int width, unsigned int height, enum type type);

// frees the matrix for the board and then frees itself
void board_free(board* b);

// prints appropriate indexing (A-Z past 10, a-z past 36) and padding 
// prints out the board by iterating through the array for board
// prints '.' for EMPTY, 'o' for WHITE, '*' for BLACK
void board_show(board* b);

// helper function that prints corresponding board index of a integer
// used in board_show
void board_index(int i);

// retrieves the cell in a position on a board using the matrix
cell board_get(board* b, pos p);

// updating a position on a board using the matrix
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
