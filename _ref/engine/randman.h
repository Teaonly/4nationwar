#ifndef _RANDMAN_H_
#define _RANDMAN_H_

#include "cpu.h"

class Move;

class RandomMan : public CPU {
public:
    RandomMan();
    virtual ~RandomMan();
    virtual AI4::PLAYER_ACTION doAction(AI4::PIECE_COLOR color, Move &newMove, BoardContext *board); 
};


#endif
