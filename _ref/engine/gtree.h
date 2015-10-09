#ifndef _GTREE_H_
#define _GTREE_H_

#include <vector>
#include "ai4nationwar.h"
#include "boardcontext.h"

class GameTree {
public:
    GameTree(AI4::PIECE_COLOR c, BoardContext* b);
    ~GameTree();

    void addMove(Move& m);

public:
    struct GameBranch {
        GameTree*  captured_;
        GameTree*  losted_;
        GameTree*  fired_;
    };

    AI4::PIECE_COLOR color_;
    float value_;
    Move move_;                 
    float   weight_;            // only for triple branch
    BoardContext* board_;       // only for node
     
    std::vector<GameTree*>      schild_;
    std::vector<GameBranch>     mchild_;
    GameTree*                   parent_;
};


#endif
