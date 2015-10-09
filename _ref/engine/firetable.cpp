#include "firetable.h"

AI4::ACTION_RESULT fireTable[13][13];

void initFireTable() {
    static bool initFlag = false;

    if ( initFlag == true)
        return;

    for(int i = 0; i <= 8; i++) {
        for ( int j = 0; j <=8; j++) {
            if ( i < j)
                fireTable[i][j] = AI4::AR_LOST;
            if ( i == j)
                fireTable[i][j] = AI4::AR_FIRE;
            if ( i > j)
                fireTable[i][j] = AI4::AR_CAPTURE; 
        }
    }        
 

    // for MINE
    for (int i = 0; i < 13; i++) {
        fireTable[AI4::PR_MINE][i] = AI4::AR_ILLEGAL;
        fireTable[i][AI4::PR_MINE] = AI4::AR_LOST;
    }

    // for FLAG
    for (int i = 0; i < 13; i++) {
        fireTable[AI4::PR_FLAG][i] = AI4::AR_ILLEGAL;
        fireTable[i][AI4::PR_FLAG] = AI4::AR_CAPTURE;
    }

    // for BOMB
    for (int i = 0; i < 13; i++) {
        fireTable[AI4::PR_BOMB][i] = AI4::AR_FIRE;
        fireTable[i][AI4::PR_BOMB] = AI4::AR_FIRE;
    }
 
    // exceptions
    fireTable[AI4::PR_MINER][AI4::PR_MINE] = AI4::AR_CAPTURE;
    fireTable[AI4::PR_MINE][AI4::PR_FLAG] = AI4::AR_ILLEGAL;
    fireTable[AI4::PR_MINE][AI4::PR_BOMB] = AI4::AR_ILLEGAL;

    initFlag = true;
}


