#include <iostream>

#include "player.h"
#include "piece.h"
#include "boardcontext.h"
#include "firetable.h"
#include "game.h"


Game::Game() {
    for ( int i = 0; i < 4; i++) {
        players[i] = NULL;
    }
    
    initFireTable();
}

Game::~Game() {
    
}

void Game::setPlayer( Player *player ) {
    players[player->color] = player;
}

void Game::setupPieces( Piece* pieces[]) {
    for(int i = 0; i < 4; i++) {
        players[i]->setupPieces(pieces[i]);
    }
}

AI4::ACTION_RESULT Game::doAction(AI4::PIECE_COLOR color, Move move) {
    if ( players[color]->board_->checkMove(color, move) == false) {
        std::cout << "Error this is illegal move " << std::endl;
        return AI4::AR_ILLEGAL;
    }
    
    AI4::ACTION_RESULT ar;
    Piece *a = players[color]->board_->getPiece(move.fromY, move.fromX);
    Piece *b = players[color]->board_->getPiece(move.toY, move.toX);
    if ( b == NULL) {
        ar = AI4::AR_MOVED;
    } else {
        AI4::PIECE_COLOR fireColor = b->color;
        b = players[fireColor]->board_->getPiece(move.toY, move.toX);
        ar = fireTable[a->role][b->role];
        if ( ar == AI4::AR_FIRE) {
            if ( a->role == AI4::PR_LING  && b->role == AI4::PR_LING) {
                ar = AI4::AR_LING_LING; 
            } else if ( a->role == AI4::PR_LING && b->role == AI4::PR_BOMB) {
                ar = AI4::AR_LING_BOMP;
            } else if ( b->role == AI4::PR_LING && a->role == AI4::PR_BOMB) {
                ar = AI4::AR_BOMP_LING;
            }
        } else if ( ar == AI4::AR_LOST) {
            if ( a->role == AI4::PR_LING ) {
                ar = AI4::AR_LING_MINE;
            }
        } else if ( ar == AI4::AR_CAPTURE) {
            if ( b->role == AI4::PR_FLAG) {
                ar = AI4::AR_FLAG;
            }
        }
    }
    
    return ar;
}

void Game::doUpdate(AI4::PIECE_COLOR color, AI4::PLAYER_ACTION ac) {
    for (int i = 0; i < 4; i++) {
        players[i]->doUpdate(color, ac);
    }    
}

void Game::doUpdate(AI4::ACTION_RESULT ar, Move move) {
    Piece *a = players[0]->board_->getPiece(move.fromY, move.fromX);
    Piece *b = players[0]->board_->getPiece(move.toY, move.toX);
 
    for(int i = 0; i < 4; i++) {
        players[i]->doUpdate(ar, move);
    }        

    bool openAflag = false;
    bool openBflag = false;
    if ( ar == AI4::AR_LING_LING) {
        openAflag = true;
        openBflag = true;
    } else if ( ar == AI4::AR_LING_BOMP || ar == AI4::AR_LING_MINE) {
        openAflag = true;    
    } else if ( ar == AI4::AR_BOMP_LING) {
        openBflag = true;
    }

    if ( a != NULL && openAflag ) {
        AI4::PIECE_COLOR color = a->color;
        int flagX = -1;
        for(int i = 0; i < 25; i++) {
            if ( players[color]->board_->getPiece(color*25+i)->role == AI4::PR_FLAG) {
                flagX = players[color]->board_->getPiece(color*25+i)->x;    
                break;
            }
        }
        if (flagX != -1) {
            for(int i = 0; i < 4; i++) {
                players[i]->openFlag(flagX);
            } 
        }
    }

    if ( b != NULL && openBflag ) {
        AI4::PIECE_COLOR color = b->color;
        int flagX = -1;
        for(int i = 0; i < 25; i++) {
            if ( players[color]->board_->getPiece(color*25+i)->role == AI4::PR_FLAG) {
                flagX = players[color]->board_->getPiece(color*25+i)->x;    
                break;
            }
        }
        if (flagX != -1) {
            for(int i = 0; i < 4; i++) {
                players[i]->openFlag(flagX);
            } 
        }
    }
}

AI4::PLAYER_ACTION Game::doAI(AI4::PIECE_COLOR color, Move &newMove) {
    return players[color]->doAI(newMove); 
} 

int Game::checkBoard() {
    for (int y = 0; y <= 7; y++) {
        for (int x = 0; x < 20; x++) {
            
            if ( players[0]->board_->getPiece(y, x) == NULL) {
                if ( players[1]->status == AI4::PS_ONLINE && players[1]->board_->getPiece(y, x) != NULL) {
                    return -1;
                }         
                if ( players[2]->status == AI4::PS_ONLINE && players[2]->board_->getPiece(y, x) != NULL) {
                    return -1;
                }
                if ( players[3]->status == AI4::PS_ONLINE && players[3]->board_->getPiece(y, x) != NULL) {
                    return -1;
                }
            } else {
                if ( players[1]->status == AI4::PS_ONLINE && players[1]->board_->getPiece(y, x)->color != players[0]->board_->getPiece(y, x)->color) {
                    return -2;
                }
                if ( players[2]->status == AI4::PS_ONLINE && players[2]->board_->getPiece(y, x)->color != players[0]->board_->getPiece(y, x)->color) {
                    return -2;
                } 
                if ( players[3]->status == AI4::PS_ONLINE && players[3]->board_->getPiece(y, x)->color != players[0]->board_->getPiece(y, x)->color) {
                    return -2;
                } 
            }

            
            for(int n = 0; n < 4; n++) {
                if (players[n]->status != AI4::PS_ONLINE )
                    continue;
                Piece *test = players[n]->board_->getPiece(y, x);
                if ( test != NULL) {
                    if ( (int)test->y != y || (int)test->x != x) {
                        return -3;
                    }
                }
            }
        }
    } 

    return 0;
}


