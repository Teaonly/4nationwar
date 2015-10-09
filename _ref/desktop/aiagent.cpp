#include "aiagent.h"

#include "../engine/game.h"
#include "../engine/player.h"

void AIAgent::ExecuteAI( Game *gameContoler, AI4::PIECE_COLOR whichPlayer) {
    if ( inProcessing == true)
        return;

    game = gameContoler;
    color = whichPlayer;

    inProcessing = false;
    start();
}

void AIAgent::run() {
    
    QThread::msleep(1000);
    actionResult = game->doAI(color, moveResult);

    inProcessing = true;
    emit signalAIFinish();        
}



