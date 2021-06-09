// A player interface that plays the game using ./play
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"
#include "board.h"
#include "pos.h"

// prints the next player in the game
void show_cur_player(game* g){
    if(g->player == BLACKS_TURN){
        printf("It is Black's turn.\n");
    } else {
        printf("It is White's turn.\n");
    }
}

// takes the input char and turn that into rows and column index
// called inverse because it is the reverse process of my function in board.c
// i do not throw an error and instead return an error_indicator,
// because I don't want this to stop main
unsigned int inverse_board_index(char c){
    unsigned int error_indicator = 999;
    if((48<=c)&&(c<=57)){
        return (unsigned int)(c-48);
    }
    if((65<=c)&&(c<=90)){
        return (unsigned int)(c-55);
    }
    if((97<=c)&&(c<=122)){
        return(unsigned int)(c-61);
    }
    return error_indicator;
}

void print_separator(int l){
    printf("\n");
    for(int i=0; i<l; i++){
        printf("- -");
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    // sets up a reasonable default board when nothing is specified
    unsigned int width = 6;
    unsigned int height = 6;
    unsigned int run = 3;
    unsigned int hang_time = 2;
    enum type type = BITS;
    unsigned int type_chosen = 0;

    // allows each field of the game to be customizable
    for(int i = 1; i<argc-1; i++){
        if(!(strcmp("-w", argv[i]))){
            width = atof(argv[i+1]);
        }
        if(!(strcmp("-h", argv[i]))){
            height = atof(argv[i+1]);
        }
        if(!(strcmp("-r", argv[i]))){
            run = atof(argv[i+1]);
        }
        if(!(strcmp("-t", argv[i]))){
            hang_time = atof(argv[i+1]);
        }
        if(!(strcmp("-m", argv[i]))){
            if(type_chosen == 1){
                fprintf(stderr, "type: game type cannot be both MATRIX AND BITS");
                exit(1);
            }
            type = MATRIX;
            type_chosen = 1;
        }
        if(!(strcmp("-b", argv[i]))){
            if(type_chosen == 1){
                fprintf(stderr, "type: game type cannot be both MATRIX AND BITS");
                exit(1);
            }
            type = BITS;
            type_chosen = 1;
        }
    }
    
    // raising errors when inputs are invalid
    if(width > 62){
        fprintf(stderr, "invalid width: cannot exceed 62");
        exit(1);
    }   
    
    if(height > 62){
        fprintf(stderr, "invalid height: cannot exceed 62");
        exit(1);
    }
    
    if((run > height)&&(run > width)){
        fprintf(stderr, "invalid run: such a long run would always result in DRAW");
        exit(1);
    }
    
    // initializes a new game
    game* g = new_game(run, hang_time, width, height, type);
    outcome cur_state = IN_PROGRESS;
    char r,c;
    unsigned int ri, ci;

    // puts all of the processes of the game in a while loop
    // that executes when the state of the game is IN_PROGRESS
    while(cur_state==IN_PROGRESS){
        // prints the board
        board_show(g->b);
        print_separator(width);
        // indicates current player
        show_cur_player(g);

        // asks player to enter row and col
        printf("Enter row and column: ");
        scanf(" %c %c*c", &r, &c);
        printf("\n");

        // use inverse_board_index to find index of the position
        ri = inverse_board_index(r);
        ci = inverse_board_index(c);

        // if either is equal to the error indicator, 
        // or if either is out of bounds
        // we printf to ask the player to try again
        // and uses continue to skip this iteration of the loop
        if((ri==999)||(ci==999)||(ri>=height)||(ci>=width)){
            printf("Invalid Board Location, please try again!\n");
            continue;
        }
        // if we attempt to place piece and receives a FALSE
        // nothing happens and we remind the player to try again
        // and indicates that the location is already occupied
        // and then use continue to skip this iteration of the loop
        if(!place_piece(g, make_pos(ri, ci))){
            printf("Board location occupied, please try again!\n");
            continue;
        }
        // if everything goes as planned we update the cur_state
        // to the new state of the game after the piece is placed
        // using game_outcome
        cur_state = game_outcome(g);
    }
    // shows the final board
    board_show(g->b);
    print_separator(width);
    // prints the final outcome
    if(cur_state == BLACK_WIN){
        printf("*** GAME OVER: Black wins! ***\n");
    } else if(cur_state == WHITE_WIN){
        printf("*** GAME OVER: White wins! ***\n");
    } else if(cur_state == DRAW){
        printf("GAME OVER: (An anticlimactic) Draw\n");
    }
    // frees everything
    game_free(g);
    return 0; 
}
