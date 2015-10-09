#ifndef _PIECE_H_
#define _PIECE_H_

#include "ai4nationwar.h"

class Piece {
public:
    Piece() {}
    ~Piece() {}
    void setRole(AI4::PIECE_COLOR co, AI4::PIECE_ROLE ro, unsigned int y, unsigned int x);

public:
    // static information
    AI4::PIECE_COLOR color;
    AI4::PIECE_ROLE role;
   
public:
    // dynamic information fast access
    bool isAlive;
    unsigned int y;
    unsigned int x;
};

#endif
