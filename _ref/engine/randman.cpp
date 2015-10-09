#include <vector>
#include <stdlib.h>

#include "randman.h"
#include "boardcontext.h"

static std::vector<Move> myMoves;

RandomMan::RandomMan() {

}

RandomMan::~RandomMan() {
    
}

AI4::PLAYER_ACTION RandomMan::doAction(AI4::PIECE_COLOR color, Move &newMove, BoardContext *board) {
    myMoves.clear();
    board->generateMoves(color, myMoves);
    if ( myMoves.size() == 0) {
        return AI4::AC_SURRENDER;
    }
    int best = rand() % myMoves.size();    
    
    newMove = myMoves[best];
    return AI4::AC_MOVE; 
}


