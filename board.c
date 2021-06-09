// In this file is code that defines the game board for Hangtime, prints it, and deal with its relevant operations
#include <stdlib.h>
#include <stdio.h>
#include "board.h"

board* board_new(unsigned int width, unsigned int height, enum type type){
    board* ret = (board*)malloc(sizeof(board));
    ret->height = height;
    ret->width = width;
    ret->type = type; 
    // the case of BITS type
    // determines the size of the array for bits needed
    // number of cells on the board, divided by 16 
    // (number of cells) that can be stored by each int
    if(type){
        unsigned int size = 1+(width*height/16);
        unsigned int* b = (unsigned int*)malloc(size*sizeof(unsigned int*));
        for(int i = 0; i<size; i++){
          b[i] = 0;
        }
        ret->u.bits = b;
    } else {
        cell** b = (cell**)malloc(height*sizeof(cell*));
        for(int r = 0; r<height; r++){
            b[r] = (cell*)malloc(width*(sizeof(cell)));
            for(int c = 0; c<width; c++){
                b[r][c] = EMPTY;
            }
        }
        ret->u.matrix = b;
    }
    return ret;
}

void board_free(board* b){
    unsigned int rows = b->height;
    if(b->type){
        // freeing a BITS type board just involves
        // freeing an array and the board itself
        free(b->u.bits);
        free(b);
    } else {
        // freeing a MATRIX type board needs to 
        // free a 2D array and the board itself
        free(b->u.bits);
        for(int i=0; i<rows; i++){
            free(b->u.matrix[i]);
        }
        free(b->u.matrix);
        free(b);
    }
}

void board_index(int i){
    if(i<10){
        printf("%d",i);
    } else if(i<36){
        printf("%c", i+55);
    } else if(i<62){
        printf("%c", i+61);
    } else{
        printf("%c", '?');
    }
}

// prints out ints as chars
void board_show(board* b){
    unsigned int c = b->width;
    unsigned int r  = b->height;
    printf("  ");
    for(int i = 0; i<c; i++){
        board_index(i);
    }
    printf("\n");
    printf("\n");
    for(int i = 0; i<r; i++){
        board_index(i);
        printf(" ");
        if(b->type==BITS){
            for(int j = 0; j<c; j++){
                unsigned int index = j+(i*c);
                // bin finds which entry of the BITS array the cell is in
                unsigned int bin = index/16;
                // loc finds the location of the desired cell in the number in BITS
                unsigned int loc = index%16;
                // 3 in binary is 0000...0011
                // using it as filter takes the last two digits in bits
                unsigned int filter = 3;
                if(((b->u.bits[bin]>>(2*loc))&filter)==0){
                    printf("%c", '.');
                } else if(((b->u.bits[bin]>>(2*loc))&filter)==1){
                    printf("%c", '*');
                } else if(((b->u.bits[bin]>>(2*loc))&filter)==2){
                    printf("%c", 'o');
                }
            }
        }else{
            // finding the loc in MATRIX just involves iteration through each row
            for(int j = 0; j<c; j++){
                if(b->u.matrix[i][j]==BLACK){
                    printf("%c", '*');
                }else if(b->u.matrix[i][j]==WHITE){
                    printf("%c", 'o');
                } else {
                    printf("%c", '.');
                }
            }
        }
        printf("\n");
    }
}

cell board_get(board* b, pos p){
    if((p.c>=b->width)||(p.r>=b->height)){
        fprintf(stderr, "board_get: position not supported");
        exit(1);
    }
    // in the BITS format
    if(b->type == BITS){
        int index = (p.r)*b->width+(p.c);
        // once again using bin and loc to find the location of the cell in BITS
        int bin = index/16;
        int loc = index%16;
        int filter = 3;
        // and then using BITS operation to move those two digits to the last
        // and filter it
        return((b->u.bits[bin]>>(2*loc))&filter);
    } else {
        // in MATRIX directly access the relevant location in the array
        return b->u.matrix[p.r][p.c];
    }
}
 
 void board_set(board* b, pos p, cell c){
    if((p.c>b->width)||(p.r>b->height)){
        fprintf(stderr, "board_set: out of bounds location");
        exit(1);
    }
    if(b->type == BITS){
        // in BITS form
        // once again using bin and loc to find the location of the cell in BITS
        int index = (p.r)*b->width+(p.c);
        int bin = index/16;
        int loc = index%16;
        int filter = 3;
        if(c==EMPTY){
            // when we want to set a non-empty cell to empty
            // we use BITS operation to first move the two digits to the back
            // filter it
            // and moves if forward
            // what this does is it finds what is that number times 2 to the 2*loc power
            // and subtract it from the original
            // reaching the number representing the number with that loc set as 0
            b->u.bits[bin] -= (((b->u.bits[bin]>>(2*loc))&filter)<<(2*loc));
        }else{
            // if setting a non-empty cell
            // we just add the number representation of the cell multiplied by 2 to the 2*loc
            b->u.bits[bin]+=(c<<(2*loc));
        }
    }else{
        // directly access in MATRIX form
        b->u.matrix[p.r][p.c] = c;
    }
 }
