#include <stdlib.h>
#include <iostream>

#include "firetable.h"
#include "boardcontext.h"
#include "router.h"

/*******************************************************
p(flag , base) = 50.000%;
P(others, downtwo) = p(8,3) * P(2,1) * p(15,2) * p(18,18) / p(2,1) * p(9,3) * P(16,2) * p(19,19) 
P(mine, downtwo) = 33.3333%
P(bomp, downtwo) = P(8,3)*P(15,1)*P(19,19) / (P(9,3)*P(16,2)*P(19,19)) =  4.1667%
P(all, upRailway) = P(9,3)*P(16,2)*p(18,18) / (P(9,3)*P(16,2)*P(19,19)) = 5.0251%
P(others, normal) = P(9,3)*P(15,2)*P(18,18) / (P(9,3)*P(16,2)*P(19,19)) = 4.6053%
P(bomp, normal) = P(9,3)*P(15,1)*P(19,19) / (P(9,3)*P(16,2)*P(19,19)) = 6.25% 
********************************************************/

void BoardContext::reset() {
 
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 20; j++) {
            mainBoard[i][j] = NULL;                //NULL empty, or point to the real piece
        }
    }

    for ( int i = 0; i < 4; i++) {
        unsigned n = i * 25;

        AI4::PIECE_COLOR co = (AI4::PIECE_COLOR)i;

        // down two lines 5x2
        for (int y = 6; y <= 7; y++) {
            for ( int x = 0; x < 5; x++) {
                allPieces[n].setRole(co, AI4::PR_UNKNOW, y, x+i*5);
                mainBoard[y][x + i*5] = &allPieces[n];
                n++; 
            }
        } 
        
        // up line 5x1 
        for ( int x = 0; x < 5; x++) {
            allPieces[n].setRole(co, AI4::PR_UNKNOW, 2, x+i*5); 
            mainBoard[2][x + i*5] = &allPieces[n];
            n++;
        }
        // left and right 3x2
        for ( int y = 3; y <= 5; y++){
            allPieces[n].setRole(co, AI4::PR_UNKNOW, y, 0+i*5); 
            mainBoard[y][0 + i*5] = &allPieces[n];
            n++;
            
            allPieces[n].setRole(co, AI4::PR_UNKNOW, y, 4+i*5); 
            mainBoard[y][4 + i*5] = &allPieces[n];
            n++;
        }    
    
        // center 4x1
        {
            allPieces[n].setRole(co, AI4::PR_UNKNOW, 3, 2+i*5); 
            mainBoard[3][2 + i*5] = &allPieces[n];
            n++;
            
            allPieces[n].setRole(co, AI4::PR_UNKNOW, 4, 1+i*5); 
            mainBoard[4][1 + i*5] = &allPieces[n];
            n++;
            
            allPieces[n].setRole(co, AI4::PR_UNKNOW, 4, 3+i*5); 
            mainBoard[4][3 + i*5] = &allPieces[n];
            n++;

            allPieces[n].setRole(co, AI4::PR_UNKNOW, 5, 2+i*5); 
            mainBoard[5][2 + i*5] = &allPieces[n];
            n++;
        }
    }
   

    for(int n = 0; n < 100; n++) {
        
        probMatrix[n].moved = 0;
        probMatrix[n].capturedBy = -1;
        probMatrix[n].firedBy = -1;
        probMatrix[n].leftCapture = 0;
        probMatrix[n].rightCapture = 0;

        if ( allPieces[n].role == AI4::PR_UNKNOW) {

            probMatrix[n].valueTable = AI4::EPR_MOVEABLE;

            int y = allPieces[n].y;
            int x = allPieces[n].x;
            AI4::POS_TYPE pos = AI4::POSTABLE[y-2][x%5];
            if ( pos == AI4::POS_RAIL_RU ||
                 pos == AI4::POS_RAIL_LU ||
                 pos == AI4::POS_RAIL_U1 ||
                 pos == AI4::POS_RAIL_U2 ||
                 pos == AI4::POS_RAIL_U3 ) {
                probMatrix[n].valueTable = AI4::EPR_FIRST_ROW;
            } else if ( (pos == AI4::POS_RAIL_RD ||
                       pos == AI4::POS_RAIL_LD ||
                       pos == AI4::POS_RAIL_RD ||
                       pos == AI4::POS_RAIL_D1 ||
                       pos == AI4::POS_RAIL_D2 ||
                       pos == AI4::POS_RAIL_D3 ) || 
                       (pos & AI4::POS_DOWN_MASK) ) {
                probMatrix[n].valueTable = AI4::EPR_LAST_TWO_ROWS;
            } else if ( pos & AI4::POS_BASE_MASK ) {
                probMatrix[n].valueTable = AI4::EPR_ALL;
            }
        }
    }
}


BoardContext * BoardContext::clone() {
    BoardContext *newBoard = new BoardContext();
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 20; j++) {
            newBoard->mainBoard[i][j] = NULL;
        }
    }

    for(int i = 0; i < 100; i++) {
        newBoard->allPieces[i] = allPieces[i];
        if ( allPieces[i].isAlive ) {
            int y = allPieces[i].y;
            int x = allPieces[i].x;
            newBoard->mainBoard[y][x] = &(newBoard->allPieces[i]);
        }
        newBoard->probMatrix[i] = probMatrix[i];
    }
    
    return newBoard;
}

void BoardContext::setupPieces(Piece myPieces[]) {
    for ( int i = 0; i < 25; i++) {
        allPieces[i + myPieces[i].color * 25] = myPieces[i];
        mainBoard[ myPieces[i].y ][ myPieces[i].x ] = &allPieces[i + myPieces[i].color * 25];
        
        int n = i + myPieces[i].color * 25;
        probMatrix[n].valueTable = AI4::SEQ_TABLE[ allPieces[n].role ];
    }
}

// the important function to generating all the movement of current board 
// this function need more optimazation to imrove performance
void BoardContext::generateMoves(AI4::PIECE_COLOR color, Piece *fromPiece, std::vector<Move> &allMoves) {
    Move oneMove;
    oneMove.fromY = fromPiece->y;
    oneMove.fromX = fromPiece->x; 

    AI4::PIECE_COLOR friendColor = (AI4::PIECE_COLOR)((color + 2)%4);

    Path *freePath = Router::queryFreePath(fromPiece->y, fromPiece->x);
    for ( int i = 0; i < (int)freePath->roads.size() ; i++) {
        Road *road = freePath->roads[i];

        bool doBranch = false;
        for(int j = 0; j < (int)road->steps.size(); j+=2) {
            int new_y = road->steps[j];
            int new_x = road->steps[j+1];

            if ( (mainBoard[ new_y ][ new_x ]== NULL)
                 || ( (mainBoard[new_y][new_x]->color != color)
                    && (mainBoard[new_y][new_x]->color != friendColor) )) {
                
                if ( new_y >= 3 
                        && mainBoard[new_y][new_x] != NULL 
                        && AI4::POSTABLE[new_y-2][new_x%5]&AI4::POS_CAMP_MASK ) {
                    break;
                }

                oneMove.toY = new_y;
                oneMove.toX = new_x;
                allMoves.push_back(oneMove);
                if ( mainBoard[new_y][new_x] != NULL)
                    break;
                if ( (j+2) == (int)road->steps.size() )
                   doBranch = true; 
            } else {
                break;
            }
        }

        if ( doBranch == false || road->branches.size() == 0) {
            continue;
        }
        
        for ( int k = 0; k < (int)road->branches.size(); k++) {
            Road *branch = road->branches[k];
            
            for(int j = 0; j < (int)branch->steps.size(); j+=2) {
                int new_y = branch->steps[j];
                int new_x = branch->steps[j+1];
    
                if ( (mainBoard[ new_y ][ new_x ]== NULL)
                     || (mainBoard[new_y][new_x]->color != color  
                        && mainBoard[new_y][new_x]->color != friendColor)) {
                    oneMove.toY = new_y;
                    oneMove.toX = new_x;
                    allMoves.push_back(oneMove);
                    if ( mainBoard[new_y][new_x] != NULL)
                        break;
                } else {
                    break;
                }
            }
        }
    }
   
}
void BoardContext::generateMoves(AI4::PIECE_COLOR color, std::vector<Move> &allMoves) {
    Piece  *myPieces = &allPieces[color*25];
    
    for(int i = 0; i < 25; i++) {
        if ( myPieces[i].isAlive == false)
            continue;
       
        if ( myPieces[i].role == AI4::PR_FLAG 
             || myPieces[i].role == AI4::PR_MINE ) {
            continue;
        }   
        
        // Pieces in base location, can't move
        if ( myPieces[i].y == 7 && ( (myPieces[i].x%5) == 1 || (myPieces[i].x%5) == 3 ) ) {
            continue;
        }
        
        generateMoves(color, &myPieces[i], allMoves);
    }  
}

bool BoardContext::checkMove(AI4::PIECE_COLOR color, Move move) {
    std::vector<Move> legalMoves;
    
    Piece *myPiece = mainBoard[move.fromY][move.fromX];
    if ( myPiece == NULL)
        return false;
    generateMoves(color, myPiece, legalMoves); 
    
    bool findFlag = false;
    for(int i = 0; i < (int)legalMoves.size(); i++) {
        if ( legalMoves[i].toY == move.toY 
             && legalMoves[i].toX == move.toX ) {
            findFlag = true;
            break;
        }
    }

    return findFlag;
}


void BoardContext::updateWithSurrender(AI4::PIECE_COLOR co) {
    for(int i = co * 25; i < co * 25 + 25; i++) {
        if ( allPieces[i].isAlive == true ) {
            allPieces[i].isAlive = false;
            mainBoard[allPieces[i].y][allPieces[i].x] = NULL;
        }
    }
}

void BoardContext::updateWithWin(Move& newMove) {
    // update context
    AI4::PIECE_COLOR color = mainBoard[newMove.toY][newMove.toX]->color;
    
    mainBoard[newMove.toY][newMove.toX]->role = AI4::PR_FLAG;
    mainBoard[newMove.toY][newMove.toX]->isAlive = false;

    mainBoard[newMove.toY][newMove.toX] = 
        mainBoard[newMove.fromY][newMove.fromX];
    mainBoard[newMove.toY][newMove.toX]->y = newMove.toY;
    mainBoard[newMove.toY][newMove.toX]->x = newMove.toX;
    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    
    for(int i = 0; i < 25; i++) {
        if ( allPieces[i+color*25].isAlive == true) {
            allPieces[i+color*25].isAlive = false;
            int y  = allPieces[i+color*25].y;
            int x  = allPieces[i+color*25].x;
            mainBoard[y][x] = NULL;
        }
    }    
}

void BoardContext::updateWithMove(Move& newMove) {
    mainBoard[newMove.toY][newMove.toX] =  
        mainBoard[newMove.fromY][newMove.fromX];
    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX]->y = newMove.toY;
    mainBoard[newMove.toY][newMove.toX]->x = newMove.toX;
    
    // update probMatrix info 
    int n = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;
    probMatrix[n].moved ++;
    if ( allPieces[n].role == AI4::PR_UNKNOW ) {
        unsigned int vOld = probMatrix[n].valueTable;
        probMatrix[n].valueTable &= AI4::EPR_MOVEABLE;
        if ( vOld != probMatrix[n].valueTable ) {
            updateProbTreeWithTopUpdate(n);     
        }
    }
}

void BoardContext::updateWithCapture(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]);
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]);
    
    mainBoard[newMove.toY][newMove.toX]->isAlive = false;
    mainBoard[newMove.toY][newMove.toX] =  
        mainBoard[newMove.fromY][newMove.fromX];
    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX]->y = newMove.toY;
    mainBoard[newMove.toY][newMove.toX]->x = newMove.toX;
    
    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[from].moved++;
    probMatrix[from].valueTable &= AI4::EPR_MOVEABLE;
    probMatrix[from].valueTable &= queryCaptureByValue( probMatrix[to].valueTable );
    probMatrix[to].valueTable &= queryCaptureValue( oldValue1 );

    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
    if ( (allPieces[to].color + 4 - allPieces[from].color) % 4 == 1 ) {
        probMatrix[from].leftCapture |= 1 << (to%25);     
    } else if ( (allPieces[to].color + 4 - allPieces[from].color) % 4 == 3) {
        probMatrix[from].rightCapture |= 1 << (to%25);
    } else {
        // FATAL
    }
    
}

void BoardContext::updateWithLost(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    mainBoard[newMove.fromY][newMove.fromX]->isAlive = false;
    mainBoard[newMove.fromY][newMove.fromX] = NULL;


    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[from].moved++;
    probMatrix[from].valueTable &= AI4::EPR_MOVEABLE;
    probMatrix[from].valueTable &= queryCaptureValue( probMatrix[to].valueTable );
    probMatrix[to].valueTable &= queryCaptureByValue( oldValue1 );
    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
    if ( (allPieces[from].color + 4 - allPieces[to].color) % 4 == 1 ) {
        probMatrix[to].leftCapture |= 1 << (from%25);     
    } else if ( (allPieces[from].color + 4 - allPieces[to].color) % 4 == 3) {
        probMatrix[to].rightCapture |= 1 << (from%25);
    } else {
        // FATAL
    }
}

void BoardContext::updateWithFire(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    mainBoard[newMove.fromY][newMove.fromX]->isAlive = false;
    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX]->isAlive = false;
    mainBoard[newMove.toY][newMove.toX] = NULL;

    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[from].moved++;
    probMatrix[from].valueTable &= AI4::EPR_MOVEABLE;
    if ( probMatrix[from].valueTable & AI4::EPR_BOMBS || probMatrix[to].valueTable & AI4::EPR_BOMBS) {        
        probMatrix[from].valueTable &= (~AI4::EPR_LING);
        probMatrix[to].valueTable &= (~AI4::EPR_LING);
    } else {
        probMatrix[from].valueTable &= oldValue2;
        probMatrix[to].valueTable &= (oldValue1 & AI4::EPR_MOVEABLE);
    }
    probMatrix[from].firedBy = to;
    probMatrix[to].firedBy = from;

    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
 
}

void BoardContext::updateWithLingBomp(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    Piece *p1 = mainBoard[newMove.fromY][newMove.fromX];
    Piece *p2 = mainBoard[newMove.toY][newMove.toX];
    p1->isAlive = false;
    p1->role = AI4::PR_LING;
    p2->isAlive = false;
    p2->role = AI4::PR_BOMB;

    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX] = NULL;

    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[from].valueTable = AI4::EPR_LING;
    probMatrix[to].valueTable &= AI4::EPR_BOMBS;
    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }

}

void BoardContext::updateWithLingLing(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    Piece *p1 = mainBoard[newMove.fromY][newMove.fromX];
    Piece *p2 = mainBoard[newMove.toY][newMove.toX];
    p1->isAlive = false;
    p1->role = AI4::PR_LING;
    p2->isAlive = false;
    p2->role = AI4::PR_LING;

    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX] = NULL;
    
    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[from].valueTable = AI4::EPR_LING;
    probMatrix[to].valueTable = AI4::EPR_LING;
    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
}

void BoardContext::updateWithBompLing(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    Piece *p1 = mainBoard[newMove.fromY][newMove.fromX];
    Piece *p2 = mainBoard[newMove.toY][newMove.toX];
    p1->isAlive = false;
    p1->role = AI4::PR_BOMB;
    p2->isAlive = false;
    p2->role = AI4::PR_LING;

    mainBoard[newMove.fromY][newMove.fromX] = NULL;
    mainBoard[newMove.toY][newMove.toX] = NULL;

    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[to].valueTable = AI4::EPR_LING;
    probMatrix[from].valueTable &= AI4::EPR_BOMBS;
    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
}

void BoardContext::updateWithLingMine(Move& newMove) {
    int from = ( mainBoard[newMove.fromY][newMove.fromX] - &allPieces[0]) ;
    int to = ( mainBoard[newMove.toY][newMove.toX] - &allPieces[0]) ;

    Piece *p1 = mainBoard[newMove.fromY][newMove.fromX];
    Piece *p2 = mainBoard[newMove.toY][newMove.toX];
    p1->isAlive = false;
    p1->role = AI4::PR_LING;
    p2->role = AI4::PR_MINE;

    mainBoard[newMove.fromY][newMove.fromX] = NULL;

    unsigned int oldValue1 = probMatrix[from].valueTable;
    unsigned int oldValue2 = probMatrix[to].valueTable;
    probMatrix[to].valueTable = AI4::EPR_LING;
    probMatrix[from].valueTable &= AI4::EPR_MINES;
    if ( oldValue1 != probMatrix[from].valueTable ) {
        updateProbTreeWithTopUpdate(from);
    }
    if ( oldValue2 != probMatrix[to].valueTable ) {
        updateProbTreeWithTopUpdate(to);
    }
}

void BoardContext::updateWithFlag(unsigned int x) {
    mainBoard[7][x]->role = AI4::PR_FLAG;
}

unsigned int BoardContext::queryCaptureValue(unsigned int value) {
    unsigned int ret = 0;
    if ( value & AI4::EPR_MINERS ) {
        ret |= AI4::EPR_MINES;
    }    
    if ( value & AI4::EPR_PAIS ) {
        ret |= AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_LIANS ) {
        ret |= AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_YINGS ) {
        ret |= AI4::EPR_LIANS + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_TUANS ) {
        ret |= AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_LVS) {
        ret |= AI4::EPR_TUANS + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_SHIS) {
        ret |= AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_JUN) {
        ret |= AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_LING) {
        ret |= AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS + AI4::EPR_MINERS;
    }
    if ( value & AI4::EPR_MINES ) {
        ret |= AI4::EPR_LING 
               + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS;
    }

    return ret;
}

unsigned int BoardContext::queryCaptureByValue(unsigned int value) {
    unsigned int ret = 0;
    if ( value & AI4::EPR_MINES ) {
        ret |= AI4::EPR_MINERS;
    }    
    if ( value & AI4::EPR_JUN ) {
        ret |= AI4::EPR_LING;
    }
    if ( value & AI4::EPR_SHIS ) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN;
    }
    if ( value & AI4::EPR_LVS ) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN + AI4::EPR_SHIS;
    }
    if ( value & AI4::EPR_TUANS) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS; 
    }
    if ( value & AI4::EPR_YINGS) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS; 
    }
    if ( value & AI4::EPR_LIANS) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS; 
    }
    if ( value & AI4::EPR_PAIS) {
        ret |= AI4::EPR_LING + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS; 
    }
    if ( value & AI4::EPR_MINERS ) {
        ret |= AI4::EPR_LING 
               + AI4::EPR_JUN + AI4::EPR_SHIS
               + AI4::EPR_LVS + AI4::EPR_TUANS 
               + AI4::EPR_YINGS + AI4::EPR_LIANS 
               + AI4::EPR_PAIS;
    }

    return ret;
}

void BoardContext::updateProbTreeWithTopUpdate(int top) {
    checkProbValue(top);
    
    int leftColor = (allPieces[top].color + 1)%4;
    int rightColor = (allPieces[top].color + 3)%4;
    unsigned int captureValue = queryCaptureValue(probMatrix[top].valueTable);

    for(int i = 0; i < 25; i++) {
        int bitMask = 1 << i;
        if ( bitMask & probMatrix[top].leftCapture ) {
            int left = leftColor*25 + i;
            if ( allPieces[left].role == AI4::PR_UNKNOW) {
                unsigned int oldValue = probMatrix[left].valueTable;
                probMatrix[left].valueTable &= captureValue;
                if ( oldValue != probMatrix[left].valueTable ) {
                    updateProbTreeWithTopUpdate(left);
                } 
            }
        }
        
        if ( bitMask & probMatrix[top].rightCapture ) {
            int right = rightColor*25 + i;
            if ( allPieces[right].role == AI4::PR_UNKNOW) {
                unsigned int oldValue = probMatrix[right].valueTable;
                probMatrix[right].valueTable &= captureValue;
                if ( oldValue != probMatrix[right].valueTable ) {
                    updateProbTreeWithTopUpdate(right);
                } 
            }
        }
    }    
}


void BoardContext::setupProbValue(int who, int value) {
    int color = allPieces[who].color;
    
    probMatrix[who].valueTable = value;
    for(int i = color*25; i < color*25+25; i++) {
        if ( i != who) {
            probMatrix[i].valueTable |= ~(value);
        }
    }
}

void BoardContext::checkProbValue(int who) {
    if ( allPieces[who].role != AI4::PR_UNKNOW) {
        return;
    }

    int color = allPieces[who].color;

    if ( probMatrix[who].valueTable == AI4::EPR_LING) {
        allPieces[who].role = AI4::PR_LING;
        setupProbValue(who, AI4::EPR_LING);
        return;
    } else if ( probMatrix[who].valueTable == AI4::EPR_JUN) {
        allPieces[who].role = AI4::PR_JUN;
        setupProbValue(who, AI4::EPR_JUN);
        return;
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_MINES)) == 0) {
        allPieces[who].role = AI4::PR_MINE;
        if ( probMatrix[who].valueTable & AI4::EPR_MINE1 ) {
            setupProbValue(who, AI4::EPR_MINE1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_MINE2) {
            setupProbValue(who, AI4::EPR_MINE2);
        } else if ( probMatrix[who].valueTable & AI4::EPR_MINE3) {
            setupProbValue(who, AI4::EPR_MINE3);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_BOMBS)) == 0) {
        allPieces[who].role = AI4::PR_BOMB;
        if ( probMatrix[who].valueTable & AI4::EPR_BOMB1 ) {
            setupProbValue(who, AI4::EPR_BOMB1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_BOMB2) {
            setupProbValue(who, AI4::EPR_BOMB2);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_MINERS)) == 0) {
        allPieces[who].role = AI4::PR_MINER;
        if ( probMatrix[who].valueTable & AI4::EPR_MINER1 ) {
            setupProbValue(who, AI4::EPR_MINER1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_MINER2) {
            setupProbValue(who, AI4::EPR_MINER2);
        } else if ( probMatrix[who].valueTable & AI4::EPR_MINER3) {
            setupProbValue(who, AI4::EPR_MINER3);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_PAIS)) == 0) {
        allPieces[who].role = AI4::PR_PAI;
        if ( probMatrix[who].valueTable & AI4::EPR_PAI1 ) {
            setupProbValue(who, AI4::EPR_PAI1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_PAI2) {
            setupProbValue(who, AI4::EPR_PAI2);
        } else if ( probMatrix[who].valueTable & AI4::EPR_PAI3) {
            setupProbValue(who, AI4::EPR_PAI3);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_LIANS)) == 0) {
        allPieces[who].role = AI4::PR_LIAN;
        if ( probMatrix[who].valueTable & AI4::EPR_LIAN1 ) {
            setupProbValue(who, AI4::EPR_LIAN1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_LIAN2) {
            setupProbValue(who, AI4::EPR_LIAN2);
        } else if ( probMatrix[who].valueTable & AI4::EPR_LIAN3) {
            setupProbValue(who, AI4::EPR_LIAN3);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_YINGS)) == 0) {
        allPieces[who].role = AI4::PR_YING;
        if ( probMatrix[who].valueTable & AI4::EPR_YING1 ) {
            setupProbValue(who, AI4::EPR_YING1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_YING2) {
            setupProbValue(who, AI4::EPR_YING2);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_TUANS)) == 0) {
        allPieces[who].role = AI4::PR_TUAN;
        if ( probMatrix[who].valueTable & AI4::EPR_TUAN1 ) {
            setupProbValue(who, AI4::EPR_TUAN1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_TUAN2) {
            setupProbValue(who, AI4::EPR_TUAN2);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_LVS)) == 0) {
        allPieces[who].role = AI4::PR_LV;
        if ( probMatrix[who].valueTable & AI4::EPR_LV1 ) {
            setupProbValue(who, AI4::EPR_LV1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_LV2) {
            setupProbValue(who, AI4::EPR_LV2);
        }
    } else if ( (probMatrix[who].valueTable & (~AI4::EPR_SHIS)) == 0) {
        allPieces[who].role = AI4::PR_SHI;
        if ( probMatrix[who].valueTable & AI4::EPR_SHI1 ) {
            setupProbValue(who, AI4::EPR_SHI1);
        } else if ( probMatrix[who].valueTable & AI4::EPR_SHI2) {
            setupProbValue(who, AI4::EPR_SHI2);
        }
    } 
}
