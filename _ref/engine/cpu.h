#ifndef _CPU_H_
#define _CPU_H_

#include <string>
#include "ai4nationwar.h"

class BoardContext;
class Move;

class CPU {
public:
    virtual AI4::PLAYER_ACTION  doAction(AI4::PIECE_COLOR co, Move &newMove, BoardContext *board) = 0;    
    virtual ~CPU() {};
    
    static CPU* createCPU(std::string &name, void *parameter);
};


#endif
