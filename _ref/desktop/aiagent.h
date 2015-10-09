#ifndef _AI_AGENT_H_
#define _AI_AGENT_H_

#include <QThread>

#include "../engine/ai4nationwar.h"
#include "../engine/boardcontext.h"

class Game;

class AIAgent : public QThread
{
    Q_OBJECT
public:
    AIAgent() {
        inProcessing = false;
    }
    ~AIAgent() {
    }

signals:
    void signalAIFinish(); 

protected:
    virtual void run();

public:    
    bool inProcessing;
    Move moveResult;
    AI4::PLAYER_ACTION actionResult;

    void ExecuteAI(Game *, AI4::PIECE_COLOR);

private:
    Game *game;
    AI4::PIECE_COLOR color;
};
#endif

