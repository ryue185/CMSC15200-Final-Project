#ifndef POS_H
#define POS_H

#include <stdbool.h>


struct pos {
    unsigned int r, c;
};
typedef struct pos pos;



typedef struct pq_entry pq_entry;
struct pq_entry {
    pos p;
    pq_entry* next;
};


typedef struct posqueue posqueue;
struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};


// makes a new pos instance 
pos make_pos(unsigned int r, unsigned int c);

// makes a new posqueue where head and tail both null, len is 0
posqueue* posqueue_new();

// adds 1 to the len of the posqueue
// appends a new pqe item with pos p and NULL
// as the next of the old posqueue tail
// and then update the tail of the posqueue as this new pqe
void pos_enqueue(posqueue* q, pos p);

// subtracts 1 from the len of posqueue
// makes the head of the new posqueue as 
// the next of the original next
pos pos_dequeue(posqueue* q);

// compare two positions if they are equal
bool equal_pos(pos p1, pos p2);
// iterates through the posqueue and see 
// if there is a pqe with the same pos
bool posqueue_member(posqueue* q, pos p);
// iteratively frees posqueue
void posqueue_free(posqueue* q);

#endif /* POS_H */
