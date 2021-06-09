play: pos.h pos.c board.h board.c logic.h logic.c play.c
	clang -Wall -g -O0 -o play pos.c board.c logic.c play.c
evidence: pos.h pos.c board.h board.c logic.h logic.c evidence.c
	clang -Wall -g -O0 -o evidence pos.c board.c logic.c evidence.c
clean:
	rm -rf evidence *.o *~ *dSYM
// this is the code used to compile the various files used in this game
// use "make play"   or   "make evidence" to compile
