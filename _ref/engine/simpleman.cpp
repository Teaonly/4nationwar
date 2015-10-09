#include <iostream>
#include <vector>
#include <stdlib.h>

#include "simpleman.h"
#include "boardcontext.h"
#include "firetable.h"

static float simpleBoardValue(AI4::PIECE_COLOR color, BoardContext* ctx) {
    AI4::PIECE_COLOR friendColor = (AI4::PIECE_COLOR)((color + 2)%4);

    float value = 0.0;
    for (int i = 0; i < 100; i++) {
        Piece* p = ctx->getPiece(i);
        if ( p->isAlive ) {
            if( p->color == color) {
                value += AI4::VALUE_TABLE[ p->role ];
            } else if ( p->color == friendColor) {
                if ( p->role == AI4::PR_UNKNOW ) {
                    value += 5;
                } else {
                    value += AI4::VALUE_TABLE[ p->role ];
                }
            } else {
                if ( p->role == AI4::PR_UNKNOW) {
                    value -= 15;
                } else {
                    value -= AI4::VALUE_TABLE[ p->role ];
                }
            }
        }
    }

    return value;
}

static void simpleProbValue(BoardContext* ctx, Move& move, std::vector<AI4::ACTION_RESULT>& results ) {
    Piece* pFrom = ctx->getPiece(move.fromY, move.fromX);
    Piece* pTo = ctx->getPiece(move.toY, move.toX);

    if ( pTo->role != AI4::PR_UNKNOW ) {
        AI4::ACTION_RESULT ar = fireTable[pFrom->role][pTo->role];
        results.push_back(ar);
    } else { 
        ProbInfo probTo = ctx->getProbInfo(pTo);

		for ( int i = 0; i < 32; i++) {
            if ( probTo.valueTable & (1<<i) ) {
                AI4::PIECE_ROLE r = AI4::SEQ_TABLE2[i];
                AI4::ACTION_RESULT ar = fireTable[pFrom->role][r];
                results.push_back(ar);
            } 
        }
    }
}

SimpleMan::SimpleMan() {

}

SimpleMan::~SimpleMan() {
    
}

AI4::PLAYER_ACTION SimpleMan::doAction(AI4::PIECE_COLOR color, Move &newMove, BoardContext *board) {
    std::vector<Move> myMoves;
    board->generateMoves(color, myMoves);
    if ( myMoves.size() == 0) {
        return AI4::AC_SURRENDER;
    }
    
    Move bestMove = myMoves[0];
    float maxValue = -100000;

    Move move;
    for (unsigned int i = 0; i < myMoves.size(); i++) {
        move = myMoves[i];

        if ( board->getPiece(move.toY, move.toX) == NULL ) {
            BoardContext* newBoard = board->clone();
            newBoard->updateWithMove(move);
            float newValue = simpleBoardValue(color, newBoard);
            if ( newValue >= maxValue && (rand()%10 == 1) ) {
                maxValue = newValue;
                bestMove = move;
            } 
            delete newBoard;
        } else {
            // we apply the expectation 
            std::vector<AI4::ACTION_RESULT> results;
            simpleProbValue(board, move, results);
            
            float value = 0.0; 
            for (unsigned int i = 0; i < results.size(); i++) {
                BoardContext* newBoard = board->clone();
                AI4::ACTION_RESULT ar = results[i];

                if ( ar == AI4::AR_FLAG ) {
                    newBoard->updateWithWin(move);    
                } else if ( ar == AI4::AR_MOVED ) {
                    newBoard->updateWithMove(move);
                } else if ( ar == AI4::AR_CAPTURE ) {
                    newBoard->updateWithCapture(move);
                } else if ( ar == AI4::AR_LOST ) {
                    newBoard->updateWithLost(move);
                } else if ( ar == AI4::AR_FIRE ) {
                    newBoard->updateWithFire(move);
                } else if ( ar == AI4::AR_LING_BOMP ) {
                    newBoard->updateWithLingBomp(move);
                } else if ( ar == AI4::AR_LING_LING) {
                    newBoard->updateWithLingBomp(move);
                } else if ( ar == AI4::AR_BOMP_LING) {
                    newBoard->updateWithBompLing(move);
                } else if ( ar == AI4::AR_LING_MINE ) {
                    newBoard->updateWithLingMine(move);
                }

                value += simpleBoardValue(color, newBoard);

                delete newBoard;
            } 

            value /= results.size();

            if ( value >= maxValue) {
                maxValue = value;
                bestMove = move;
            } 
        }
    }

    newMove = bestMove;
    return AI4::AC_MOVE;
}


