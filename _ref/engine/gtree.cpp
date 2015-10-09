#include "gtree.h"

GameTree::GameTree(AI4::PIECE_COLOR c, BoardContext* b) {
    color_ = c;
    board_ = b;
    parent_ = NULL;
}

GameTree::~GameTree() {

}

void GameTree::addMove(Move& move) {
    if ( board_->getPiece(move.toY, move.toX) == NULL) {
        BoardContext *b = board_->clone();
           
        AI4::PIECE_COLOR c = (AI4::PIECE_COLOR)((color_+1) % 4);
        GameTree* t = new GameTree(c,b);
        t->move_ = move;
        schild_.push_back(t);
    }
} 



