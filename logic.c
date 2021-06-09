#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "board.h"
#include "pos.h"
#include <stdbool.h>


game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type){
                   if(run<2){
                       fprintf(stderr, "new_game: run too short, need to be 2 or more");
                       exit(1);
                   }
                   game* ret = (game*)malloc(sizeof(game));
                   ret->run = run;
                   ret->hangtime = hangtime;
                   ret->b = board_new(width, height, type);
                   ret->hanging = posqueue_new();
                   ret->player = BLACKS_TURN;
                   return ret;
               }

void game_free(game* g){
    board_free(g->b);
    posqueue_free(g->hanging);
    free(g);
}

int drop_down(game* g, pos todrop){
    int drop_dis = 0;
    pos below = make_pos(todrop.r+1,todrop.c);
    while((below.r<g->b->height)&&(board_get(g->b,below)==EMPTY)){
        below.r+=1;
        drop_dis+=1;
    }
    pos new_loc = make_pos(below.r-1, todrop.c);
    if(new_loc.r!=todrop.r){
        board_set(g->b,new_loc,board_get(g->b,todrop));
        board_set(g->b,todrop,EMPTY);
    }
    return drop_dis;
}


bool place_piece(game* g, pos p){
    // case where illegal move is requested
    if(board_get(g->b,p)!=EMPTY){
        return false;
    }
    // for out of bounds immediately return false 
    if((p.r>g->b->height)||(p.c>g->b->width)){
        return false;
    }
    // adds in new piece
    // making the move
    // note +1 maps turn to cell perfectly
    // black is 0 in turn but 1 in cell
    // white is 1 in turn but 2 in cell
    board_set(g->b,p,g->player+1);
    // enqueue to hanging
    pos_enqueue(g->hanging,p);
    //changes player to the next one
    g->player = (g->player+1)%2;

    // drops down old piece
    if(g->hanging->len>g->hangtime){
        pos todrop = pos_dequeue(g->hanging);
        while((!posqueue_member(g->hanging,todrop))
             &&(board_get(g->b,todrop)!=EMPTY)){
                    drop_down(g,todrop);
                    if(todrop.r==0){
                        break;
                    }
                    todrop.r-=1;
                }
    }
    return true;
}


// helper, checks if the pos is part of some run
bool in_four(game* g, pos p){
    bool left_end = false;
    bool up_end = false;
    bool up_r_diag = false;
    bool up_l_diag = false;
    pos check;
    if(p.c <= g->b->width-g->run){ 
        left_end = true;
        check.r = p.r;
        check.c = p.c+1;
        for(int i = 1; i<g->run; i++){
            if(board_get(g->b, p)==board_get(g->b, check)){
                check.c+=1;
            } else {
                left_end = false;
                break;
            }
        }
    }

    if(p.r <= g->b->height-g->run){ 
        up_end = true;
        check.c = p.c;
        check.r = p.r+1;
        for(int i = 1; i<g->run; i++){
            if(board_get(g->b, p)==board_get(g->b, check)){
                check.r+=1;
            } else {
                up_end = false;
                break;
            }
        }
    }

    if((p.r <= g->b->height-g->run)&&(p.c <= g->b->width-g->run)){ 
        up_l_diag = true;
        check.c = p.c+1;
        check.r = p.r+1;
        for(int i = 1; i<g->run; i++){
            if(board_get(g->b, p)==board_get(g->b, check)){
                check.c+=1;
                check.r+=1;
            } else {
                up_l_diag = false;
                break;
            }
        }
    }

    if((p.r <= g->b->height-g->run)&&(p.c >= g->run)){ 
        up_r_diag = true;
        check.c = p.c-1;
        check.r = p.r+1;
        for(int i = 1; i<g->run; i++){
            if(board_get(g->b, p)==board_get(g->b, check)){
                check.c-=1;
                check.r+=1;
            } else {
                up_r_diag = false;
                break;
            }
        }
    }
    return (up_l_diag||up_r_diag||left_end||up_end);
}

outcome game_outcome(game* g){
    outcome ret = DRAW;
    pos checker;
    for(int i=0; i<g->b->height; i++){
        for(int j=0; j<g->b->width; j++){
            checker.c=j;
            checker.r=i;
            cell cur = board_get(g->b, checker);
            if(cur==EMPTY){
                ret = IN_PROGRESS;
                continue;
            }
            if(in_four(g,checker)){
                   if(cur==BLACK){
                       return BLACK_WIN;
                   }else{
                       return WHITE_WIN;
                   }
               }
        }
    }
    return ret;
}