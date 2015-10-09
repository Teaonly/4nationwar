#ifndef _SIMPLEMAN_H_
#define _SIMPLEMAN_H_

#include "cpu.h"

class Move;

class SimpleMan : public CPU {
public:
    SimpleMan();
    virtual ~SimpleMan();
    virtual AI4::PLAYER_ACTION doAction(AI4::PIECE_COLOR color, Move &newMove, BoardContext *board); 
};

#endif
