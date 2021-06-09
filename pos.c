#include <stdlib.h>
#include <stdio.h>
#include "pos.h"
#include <stdbool.h>


pos make_pos(unsigned int r, unsigned int c){
    pos ret;
    ret.r = r;
    ret.c = c;
    return ret;
}

pq_entry* new_pqe(pos p, pq_entry* next){
    pq_entry* ret = (pq_entry*)malloc(sizeof(pq_entry));
    ret->p = p;
    ret->next = next;
    return ret;
}

posqueue* posqueue_new(){
    posqueue* ret = (posqueue*)malloc(sizeof(posqueue));
    if(ret==NULL){
        fprintf(stderr, "posqueue_new: cannot create queue");
    }
    ret->head = NULL;
    ret->tail = NULL;
    ret->len = 0;
    return ret;
}

void pos_enqueue(posqueue* q, pos p){
    q->len+=1;
    pq_entry* added = new_pqe(p, NULL);
    if(q->len==1){
        q->head = added;
        q->tail = added;
    } else {
    q->tail->next = added;
    q->tail = added;
    }
}

pos pos_dequeue(posqueue* q){
    if((q->len==0)||q->head==NULL){
        fprintf(stderr, "dequeue: cannot dequeue empty queue");
        exit(1);
    }
    q->len-=1;
    pos ret = q->head->p;
    pq_entry* next = q->head->next;
    free(q->head);
    q->head = next;
    if(q->head == NULL){
        q->tail=NULL;
    }
    return ret;
}


bool equal_pos(pos p1, pos p2){
    return((p1.c==p2.c)&&(p1.r==p2.r));
}

bool posqueue_member(posqueue* q, pos p){
    pq_entry* cur = q->head;
    while(cur!=NULL){
        if(equal_pos(p,cur->p)){
            return true;
        }
        cur=cur->next;
    }
    return false;
}

void posqueue_free(posqueue* q){
    pq_entry* next;
    pq_entry* cur = q->head;
    while(cur!=NULL){
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(q);
}
