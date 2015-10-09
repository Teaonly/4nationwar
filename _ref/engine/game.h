#ifndef _GAME_H_
#define _GAME_H_

#include "ai4nationwar.h"

class Move;
class Player;
class Piece;

class Game {
public:
    Game();
    ~Game();
    void setPlayer(Player * play);
    void setupPieces(Piece* []);

    AI4::PLAYER_ACTION doAI(AI4::PIECE_COLOR color, Move &newMove);
    AI4::ACTION_RESULT doAction(AI4::PIECE_COLOR color, Move move);
    void doUpdate(AI4::ACTION_RESULT ar, Move move);
    void doUpdate(AI4::PIECE_COLOR color, AI4::PLAYER_ACTION ac);
    
    int checkBoard();
protected:
    Player *players[4];
};

#endif
