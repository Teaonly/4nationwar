#include <stdlib.h>
#include <iostream>
#include "ai4nationwar.h"
#include "router.h"

int Router::initFlag = 0;
Path* Router::pathsCache[8][20];


Path* Router::queryFreePath(unsigned int y, unsigned int x) {
    if ( initFlag == 0) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 20; j++)
                pathsCache[i][j] = NULL;
        }
        initFlag = 1;
    }

    if ( pathsCache[y][x] != NULL)
        return pathsCache[y][x];

    if ( y >= 2) {      // main board
        buildMainBoardPath(y, x);
    } else {            // center board
        buildCenterBoardPath(y, x);
    }

    return pathsCache[y][x];
}

void Router::buildMainBoardPath(unsigned int y, unsigned int x) {
    AI4::POS_TYPE pt = AI4::POSTABLE[y - 2][x%5];

    Path *newPath = new Path();
    switch( pt ) {
        case AI4::POS_CAMP_LU:
        case AI4::POS_CAMP_RD:
        case AI4::POS_CAMP_CENTER:
        case AI4::POS_CAMP_RU:
        case AI4::POS_CAMP_LD: 
            {            
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_NORMAL_UP:
        case AI4::POS_NORMAL_DOWN:
        case AI4::POS_NORMAL_RIGHT:
        case AI4::POS_NORMAL_LEFT:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                break;
            } 

        case AI4::POS_DOWN_LEFT:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                break;
            } 

        case AI4::POS_DOWN_RIGHT:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                break;
            } 

        case AI4::POS_DOWN_CENTER:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_BASE_LEFT:
        case AI4::POS_BASE_RIGHT:
            {
                Road *road;
                road = new Road();
                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_RAIL_D1:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                road->steps.push_back(y);
                road->steps.push_back(x+2);
                road->steps.push_back(y);
                road->steps.push_back(x+3);
                newPath->roads.push_back(road);
                break;
            } 
        case AI4::POS_RAIL_U1:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                road->steps.push_back(y);
                road->steps.push_back(x+2);
                road->steps.push_back(y);
                road->steps.push_back(x+3);
                newPath->roads.push_back(road);
                break;
            } 

        case AI4::POS_RAIL_D2:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                road->steps.push_back(y);
                road->steps.push_back(x-2);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                road->steps.push_back(y);
                road->steps.push_back(x+2);
                newPath->roads.push_back(road);
                break;
            } 
        case AI4::POS_RAIL_U2:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                road->steps.push_back(y);
                road->steps.push_back(x-2);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                road->steps.push_back(y);
                road->steps.push_back(x+2);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(1);
                road->steps.push_back( AI4::L1_TABLE[x] );
                road->steps.push_back(0);
                road->steps.push_back(0);
                road->steps.push_back(1);
                road->steps.push_back( AI4::L1_TABLE[(x+10)%20] );
                road->steps.push_back(2);
                road->steps.push_back( (x+10)%20 );
                newPath->roads.push_back(road);
                break;
            } 

        case AI4::POS_RAIL_D3:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                road->steps.push_back(y);
                road->steps.push_back(x-2);
                road->steps.push_back(y);
                road->steps.push_back(x-3);
                newPath->roads.push_back(road);
                break;
            } 
        case AI4::POS_RAIL_U3:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                road->steps.push_back(y);
                road->steps.push_back(x-2);
                road->steps.push_back(y);
                road->steps.push_back(x-3);
                newPath->roads.push_back(road);
                break;
            } 

        default: 
            {
                return buildMainBoardPath2(y,x);
            }
    }

    pathsCache[y][x] = newPath;
    return;
}

void Router::buildMainBoardPath2(unsigned int y, unsigned int x) {

    AI4::POS_TYPE pt = AI4::POSTABLE[y-2][x%5];
    Path *newPath = new Path();

    switch( pt ) {
        case AI4::POS_RAIL_L3:
        case AI4::POS_RAIL_L2:
        case AI4::POS_RAIL_L1:
            {   
                Road *road;
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);

                if ( y == 4 ) {
                    road = new Road();
                    road->steps.push_back(y-1);
                    road->steps.push_back(x+1);
                    newPath->roads.push_back(road);
                    road = new Road();
                    road->steps.push_back(y+1);
                    road->steps.push_back(x+1);
                    newPath->roads.push_back(road);
                }

                road = new Road();
                for ( int i = y+1; i <= 6; i++) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                    newPath->roads.push_back(road);
                }     

                road = new Road();
                for ( int i = y-1; i >= 2; i--) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }

                Road * leftRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    leftRoad->steps.push_back(i);
                    leftRoad->steps.push_back( (20+x-1)%20 );
                }
                road->branches.push_back(leftRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] ); 
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+17)%20] );  
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+14)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (14+x)%20 );
                } 
                road->branches.push_back(forwardRoad);

                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_RAIL_R3:
        case AI4::POS_RAIL_R2:
        case AI4::POS_RAIL_R1:
            {   
                Road *road;
                road = new Road();
                road->steps.push_back(y);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);

                if ( y == 4 ) {
                    road = new Road();
                    road->steps.push_back(y-1);
                    road->steps.push_back(x-1);
                    newPath->roads.push_back(road);
                    road = new Road();
                    road->steps.push_back(y+1);
                    road->steps.push_back(x-1);
                    newPath->roads.push_back(road);
                }

                road = new Road();
                for ( int i = y+1; i <= 6; i++) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }     
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = y-1; i >= 2; i--) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }

                Road * rightRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    rightRoad->steps.push_back(i);
                    rightRoad->steps.push_back( (x+1)%20 );
                }
                road->branches.push_back(rightRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] );  
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+3)%20] );
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+6)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (x+6)%20 );
                } 
                road->branches.push_back(forwardRoad);

                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_RAIL_LU:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = y+1; i <= 6; i++) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }     
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = x+1; i <= (int)x+4; i++) {
                    road->steps.push_back(y);
                    road->steps.push_back(i);
                }
                newPath->roads.push_back(road);

                Road * leftRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    leftRoad->steps.push_back(i);
                    leftRoad->steps.push_back( (20+x-1)%20 );
                }
                newPath->roads.push_back(leftRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] );  
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+17)%20] );  
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+14)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (14+x)%20 );
                } 
                newPath->roads.push_back(forwardRoad);

                break;
            }
        case AI4::POS_RAIL_RU:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = y+1; i <= 6; i++) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }     
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = x-1; i >= (int)x-4; i--) {
                    road->steps.push_back(y);
                    road->steps.push_back(i);
                }
                newPath->roads.push_back(road);

                Road * rightRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    rightRoad->steps.push_back(i);
                    rightRoad->steps.push_back( (x+1)%20 );
                }
                newPath->roads.push_back(rightRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] );
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+3)%20] );
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+6)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (x+6)%20 );
                } 
                newPath->roads.push_back(forwardRoad);

                break;
            }

        case AI4::POS_RAIL_LD:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x+1);
                newPath->roads.push_back(road);

                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = x+1; i <= (int)x+4; i++) {
                    road->steps.push_back(y);
                    road->steps.push_back(i);
                }
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = y-1; i >= 2; i--) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }    

                Road * leftRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    leftRoad->steps.push_back(i);
                    leftRoad->steps.push_back( (20+x-1)%20 );
                }
                road->branches.push_back(leftRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] ); 
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+17)%20] );  
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+14)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (14+x)%20 );
                } 
                road->branches.push_back(forwardRoad);

                newPath->roads.push_back(road);
                break;
            }

        case AI4::POS_RAIL_RD:
            {
                Road *road;
                road = new Road();
                road->steps.push_back(y-1);
                road->steps.push_back(x-1);
                newPath->roads.push_back(road);

                road = new Road();
                road->steps.push_back(y+1);
                road->steps.push_back(x);
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = x-1; i >= (int)x-4; i--) {
                    road->steps.push_back(y);
                    road->steps.push_back(i);
                }
                newPath->roads.push_back(road);

                road = new Road();
                for ( int i = y-1; i >= 2; i--) {
                    road->steps.push_back(i);
                    road->steps.push_back(x);
                }    

                Road * rightRoad = new Road();
                for ( int i = 2; i <= 6; i++) {
                    rightRoad->steps.push_back(i);
                    rightRoad->steps.push_back( (x+1)%20 );
                }
                road->branches.push_back(rightRoad);

                Road * forwardRoad = new Road();
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[x] );  
                 forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+3)%20] );
                forwardRoad->steps.push_back(1);
                forwardRoad->steps.push_back( AI4::L1_TABLE[(x+6)%20] );   
                for (int i = 2; i <=6; i++) {
                    forwardRoad->steps.push_back(i);
                    forwardRoad->steps.push_back( (x+6)%20 );
                } 
                road->branches.push_back(forwardRoad);

                newPath->roads.push_back(road);
                break;
            }

        default: 
            {
                return;
            }
    }

    pathsCache[y][x] = newPath;
    return;
}

void Router::buildCenterBoardPath(unsigned int y, unsigned int x) {
    Path *newPath = new Path();

    if ( y == 0 && x == 0) {
        Road *road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(2);
        road->steps.push_back(2);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(3);
        road->steps.push_back(2);
        road->steps.push_back(7);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(5);
        road->steps.push_back(2);
        road->steps.push_back(12);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(7);
        road->steps.push_back(2);
        road->steps.push_back(17);
        newPath->roads.push_back(road);
    } else if ( y == 1 && x == 1) {
        Road *road = new Road();
        road->steps.push_back(0);
        road->steps.push_back(0);
        road->steps.push_back(1);
        road->steps.push_back(5);
        road->steps.push_back(2);
        road->steps.push_back(12);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(2);
        road->steps.push_back(2);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(0);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(19);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(2);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(5);
        }
        newPath->roads.push_back(road);
    } else if ( y == 1 && x == 3) {
        Road *road = new Road();
        road->steps.push_back(0);
        road->steps.push_back(0);
        road->steps.push_back(1);
        road->steps.push_back(7);
        road->steps.push_back(2);
        road->steps.push_back(17);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(2);
        road->steps.push_back(7);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(2);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(4);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(4);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(10);
        }
        newPath->roads.push_back(road);
    } else if ( y == 1 && x == 5) {
        Road *road = new Road();
        road->steps.push_back(0);
        road->steps.push_back(0);
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(2);
        road->steps.push_back(2);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(2);
        road->steps.push_back(12);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(4);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(9);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(6);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(15);
        }
        newPath->roads.push_back(road);
    } else if ( y == 1 && x == 7) {
        Road *road = new Road();
        road->steps.push_back(0);
        road->steps.push_back(0);
        road->steps.push_back(1);
        road->steps.push_back(3);
        road->steps.push_back(2);
        road->steps.push_back(7);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(2);
        road->steps.push_back(17);
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(0);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(0);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(6);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(14);
        }
        newPath->roads.push_back(road);

    } else if ( y == 1 && x == 0) {
        Road *road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(0);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(2);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(5);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(7);
        road->steps.push_back(1);
        road->steps.push_back(6);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(14);
        }
        newPath->roads.push_back(road);

        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(19);
        }

        newPath->roads.push_back(road);

    } else if ( y == 1 && x == 2) {
        Road *road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(4);
        }
        newPath->roads.push_back(road);

        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(5);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(3);
        road->steps.push_back(1);
        road->steps.push_back(4);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(10);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(1);
        road->steps.push_back(0);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(19);
        }
        newPath->roads.push_back(road);

    } else if ( y == 1 && x == 4) {
        Road *road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(3);
        road->steps.push_back(1);
        road->steps.push_back(2);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(4);
        }
        newPath->roads.push_back(road);

        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(9);
        }
        newPath->roads.push_back(road);
        
        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(10);
        }
        newPath->roads.push_back(road);
       
        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(5);
        road->steps.push_back(1);
        road->steps.push_back(6);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(15);
        }

        newPath->roads.push_back(road);

    } else if ( y == 1 && x == 6) {
        Road *road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(7);
        road->steps.push_back(1);
        road->steps.push_back(0);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(0);
        }
        newPath->roads.push_back(road);

        road = new Road();
        road->steps.push_back(1);
        road->steps.push_back(5);
        road->steps.push_back(1);
        road->steps.push_back(4);
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(9);
        }
        newPath->roads.push_back(road);
        
        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(14);
        }
        newPath->roads.push_back(road);
        
        road = new Road();
        for(int i = 2; i <= 6; i++) {
            road->steps.push_back(i);
            road->steps.push_back(15);
        }

        newPath->roads.push_back(road);
    }

    pathsCache[y][x] = newPath;
    return;
}

