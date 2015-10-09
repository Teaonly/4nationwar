#include <stdlib.h>
#include "piece.h"

void Piece::setRole(AI4::PIECE_COLOR co, AI4::PIECE_ROLE ro, unsigned int newY, unsigned int newX) {
    color = co;
    role = ro;
    isAlive = true;

    y = newY;
    x = newX;
}

