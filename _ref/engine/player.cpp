#include <stdlib.h>
#include "player.h"
#include "boardcontext.h"
#include "cpu.h"

Player::Player(AI4::PIECE_COLOR co, CPU *cpu) {
    color = co;
    cpu_ = cpu;
    type = AI4::PT_MAN;
    status = AI4::PS_OFFLINE;

    if ( cpu != NULL) {
        type = AI4::PT_AI;
    } else {
        type = AI4::PT_MAN;    
    }

    board_ = new BoardContext();
}

Player::~Player() {
    if ( board_ != NULL)
        delete board_;
    if ( cpu_ != NULL)
        delete cpu_;
}

void Player::setupPieces(Piece pieces[]) {
    board_->reset();
    board_->setupPieces(pieces);
    status = AI4::PS_ONLINE;
}

AI4::PLAYER_ACTION Player::doAI(Move &newMove) {
    if ( status == AI4::PS_OFFLINE )
        return AI4::AC_SKIP;

    return cpu_->doAction( color, newMove, board_); 
}

void Player::doUpdate(AI4::PIECE_COLOR co, AI4::PLAYER_ACTION ac) {
    if ( status == AI4::PS_OFFLINE)
        return;

    if ( ac == AI4::AC_SURRENDER) {
        board_->updateWithSurrender(co);
        if ( co == color ) 
            status = AI4::PS_OFFLINE;    
    }

}

void Player::doUpdate(AI4::ACTION_RESULT ar, Move newMove) {
    if ( ar == AI4::AR_ILLEGAL) 
        return;
    
    if ( status == AI4::PS_OFFLINE )
        return;

    if ( ar == AI4::AR_FLAG ) {
        int deadColor = board_->getPiece(newMove.toY, newMove.toX)->color;
        board_->updateWithWin(newMove);

        if ( deadColor == color) {
            status = AI4::PS_OFFLINE;
        }
        return;
    }

    if ( ar == AI4::AR_MOVED ) {
        board_->updateWithMove(newMove);
        return;
    }
    
    if ( ar == AI4::AR_CAPTURE ) {
        board_->updateWithCapture(newMove);
        return;
    }
    
    if ( ar == AI4::AR_LOST ) {
        board_->updateWithLost(newMove);
        return;
    }
   
    if ( ar == AI4::AR_FIRE ) {
        board_->updateWithFire(newMove);
        return;
    }
    
    if ( ar == AI4::AR_LING_BOMP ) {
        board_->updateWithLingBomp(newMove);
        return;
    } 

    if ( ar == AI4::AR_LING_LING) {
        board_->updateWithLingBomp(newMove);
        return;
    }

    if ( ar == AI4::AR_BOMP_LING) {
        board_->updateWithBompLing(newMove);
        return;
    }

    if ( ar == AI4::AR_LING_MINE ) {
        board_->updateWithLingMine(newMove);
        return;
    }
}

void Player::openFlag(unsigned int x) {
    board_->updateWithFlag(x);
}

