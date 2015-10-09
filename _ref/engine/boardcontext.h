#ifndef _BOARDCONTEXT_H_
#define _BOARDCONTEXT_H_

#include <vector>
#include "ai4nationwar.h"
#include "piece.h"

class Move {
public:
    unsigned int fromY;
    unsigned int fromX;
    unsigned int toY;
    unsigned int toX; 
};

class ProbInfo {
public:
    AI4::PIECE_ROLE pseudoValue;
    unsigned int valueTable;    

    unsigned int leftCapture;
    unsigned int rightCapture;
    signed int capturedBy;
    signed int firedBy;
    unsigned int moved;
};

class BoardContext {
public:
    void reset();
    void setupPieces(Piece myPieces[]);

    BoardContext *clone();
    Piece *getPiece(int y, int x) {
        return mainBoard[y][x];
    }
    Piece *getPiece(int index) {
        return &allPieces[index];
    }
    ProbInfo getProbInfo(Piece *p) {
        return probMatrix[ p - &allPieces[0]]; 
    } 

    bool checkMove(AI4::PIECE_COLOR color, Move move);
    void generateMoves(AI4::PIECE_COLOR color, Piece *piece, std::vector<Move> &allMoves);
    void generateMoves(AI4::PIECE_COLOR color, std::vector<Move> &allMoves);
    
    void updateWithSurrender(AI4::PIECE_COLOR co);
    void updateWithWin(Move& move);
    void updateWithFlag(unsigned int x);

    void updateWithMove(Move& move);
    void updateWithFire(Move& move);
    void updateWithCapture(Move& move);
    void updateWithLost(Move& move);

    void updateWithLingBomp(Move& move);
    void updateWithBompLing(Move& move);
    void updateWithLingLing(Move& move);
    void updateWithLingMine(Move& move);

private:
    unsigned int queryCaptureValue(unsigned int value);
    unsigned int queryCaptureByValue(unsigned int value);
    void checkProbValue(int who);
    void setupProbValue(int who, int value);
    void updateProbTreeWithTopUpdate(int top);

protected:
    Piece *mainBoard[8][20];
    Piece allPieces[100];
    ProbInfo probMatrix[100]; 
};

#endif
