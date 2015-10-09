#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "ai4nationwar.h"

class CPU;
class Move;
class BoardContext;
class Piece;

class Player {
public:
    Player(AI4::PIECE_COLOR co, CPU *);
    ~Player();

    AI4::PLAYER_TYPE type;
    AI4::PLAYER_STATUS status;
    AI4::PIECE_COLOR color;

protected:
    void setupPieces(Piece[]);
    
    AI4::PLAYER_ACTION doAI(Move &newMove);

    void doUpdate(AI4::PIECE_COLOR color, AI4::PLAYER_ACTION ac);
    void doUpdate(AI4::ACTION_RESULT ar, Move newMove);
    void openFlag(unsigned int x);

    BoardContext *board_;
    CPU *cpu_;
    friend class Game; 
};

#endif
