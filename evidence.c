// Evidence file with test cases testing the various mechanisms, functions, and their interactions with defined objects

#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "board.h"
#include "pos.h"
#include <string.h>
#include <stdbool.h>

// each evidence function takes in a type input to account for the different types
// everything else are unchanged
void evidence_board(enum type type){
    printf("--- Testing board and its associated functions ---\n");
    board* test = board_new(8,5,type);
    board_show(test);
    printf("the board after several calls to board_set\n");
    pos p1 = make_pos(3,2);
    board_set(test, p1,BLACK);
    pos p2 = make_pos(2,1);
    board_set(test,p2,WHITE);
    pos p3 = make_pos(0,7);
    board_set(test,p3,BLACK);
    board_show(test);
    printf("checking board_get\n");
    pos p4 = make_pos(4,4);
    printf("row: %d", 4);
    printf(" col: %d", 4);
    printf(" expecting 0: %d\n", board_get(test, p4));
    printf("row: %d", 2);
    printf(" col: %d", 1);
    printf(" expecting 2: %d\n", board_get(test, p2));
    board_free(test);
    printf("Another test case with larger dimensions\n");
    board* big_test = board_new(12,5,type);
    board_show(big_test);
    board_free(big_test);
}

void evidence_pos(){
    printf("--- Testing pos and its associated functions ---\n");
    printf("Creating a new test posqueue with three elements: (3,2), (4,7), (0,0)\n");
    posqueue* test = posqueue_new();
    pos_enqueue(test, make_pos(3,2));
    pos_enqueue(test, make_pos(4,7));
    pos_enqueue(test, make_pos(0,0));
    printf("testing posqueue_member for (3,2), expecting 1: %u\n", 
                            posqueue_member(test, make_pos(3,2)));
    printf("testing posqueue_member for (2,1), expecting 0: %u\n", 
                            posqueue_member(test, make_pos(2,1)));
    pos_dequeue(test);
    printf("testing again for (3,2), after a call to dequeue, expecting 0: %u\n",
                            posqueue_member(test, make_pos(3,2)));
    posqueue_free(test);
}

void evidence_game(enum type type){
    printf("--- Testing game logic ---\n");
    game* test = new_game(4,2,6,5,type);
    printf("The new game board\n");
    printf("Hangtime: %d", test->hangtime);
    printf("     Run: %d\n", test->run);
    board_show(test->b);
    printf("Blacks places first piece at (0,0)\n");
    printf("Should be successful, expecting 1:%d\n", place_piece(test, make_pos(0,0)));
    board_show(test->b);
    printf("White places next also at (0,0)\n");
    printf("Should fail, expecting 0:%d\n", place_piece(test, make_pos(0,0)));
    printf("White tried again at (1,1)\n");
    printf("Should be successful, expecting 1:%d\n", place_piece(test, make_pos(1,1)));
    board_show(test->b);
    printf("Black places next piece at (0,1);(0,0) falls down:\n");
    place_piece(test, make_pos(0,1));
    board_show(test->b);
    printf("White places next piece at (4,2); (1,1) falls down:\n");
    place_piece(test, make_pos(4,2));
    board_show(test->b);
    printf("Black places next piece at (1,1); (0,1) falls down:\n");
    printf("Should be successful, expecting 1:%d\n", place_piece(test, make_pos(1,1)));
    board_show(test->b);
    printf("Nothing falls; White places next piece at (3,4):\n");
    place_piece(test, make_pos(3,4));
    board_show(test->b);
    printf("The game should still be in progress, expecting 0:%u\n", game_outcome(test));
    printf("Black places next piece at (3,1); (0,1), (1,1) both fall down:\n");
    printf("Should be successful, expecting 1:%d\n", place_piece(test, make_pos(3,1)));
    board_show(test->b);
    printf("White places next piece at (4,3); (3,4) falls:\n");
    place_piece(test, make_pos(4,3));
    board_show(test->b);
    printf("White has a run of 4 and wins, expecting 2:%u\n", game_outcome(test));
    game_free(test);
    printf("--- Checking if a diagonal win is possible---\n");
    game* test1 = new_game(3,10,5,5,type);
    printf("Hangtime: %d", test1->hangtime);
    printf("     Run: %d\n", test1->run);
    place_piece(test1, make_pos(2,1));
    place_piece(test1, make_pos(1,0));
    place_piece(test1, make_pos(3,2));
    place_piece(test1, make_pos(2,0));
    place_piece(test1, make_pos(4,3));
    board_show(test1->b);
    printf("Black has a run of 3 and wins, expecting 1:%u\n", game_outcome(test));
    game_free(test1);
}

int main(int argc, char *argv[])
{
    // adding in the option to choose BITS or MATRIX types
    // the two should produce the same evidence output
    // defaults to BITS
    enum type type = BITS;
    for(int i = 1; i<argc-1; i++){
        if(!(strcmp("-m", argv[i]))){
            type = MATRIX;
        }
        if(!(strcmp("-b", argv[i]))){
            type = BITS;
        }
    }
    evidence_board(type);
    evidence_pos();
    evidence_game(type);
    return 0;
}
