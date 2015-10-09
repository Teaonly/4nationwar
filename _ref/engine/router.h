#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <vector>

class Road {
public:
    std::vector<int> steps;
    std::vector<Road *> branches;
};

class Path {
public:    
    std::vector<Road *> roads;
};


class Router {
public:
    static Path* queryFreePath(unsigned int y, unsigned int x);


private:    
    static void buildMainBoardPath(unsigned int y, unsigned int x);
    static void buildMainBoardPath2(unsigned int y, unsigned int x);
    static void buildCenterBoardPath(unsigned int y, unsigned int x);
    static int initFlag;
    static Path* pathsCache[8][20];
};


#endif
