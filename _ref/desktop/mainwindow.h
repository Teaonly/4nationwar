#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui>
#include "../engine/ai4nationwar.h"

class BoardScene;
class BoardView;
class PieceItem;
class Game;
class Player;
class Piece;
class Move;
class AIAgent;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
	MainWindow();
    ~MainWindow();

private:
	void InitGui();
    void createActions();
	void createMenus();
	void setupSignal();

    void InitGame();
    void updateBoard(AI4::ACTION_RESULT, Move);

protected:
	void closeEvent(QCloseEvent *event);
	void timerEvent(QTimerEvent *event);

private slots:
	void restart();
	void quit();
    void test();
    void onPieceMove(PieceItem *, QPointF);
    void onAIFinish();

private:
	QAction *restartAct;
    QAction *quitAct;
    QAction *testAct;

private:
	BoardScene *pScene;		    
	BoardView *pView;			
    PieceItem *pItems[100];          
    
    Player*   players[4];
    Game*     gameControler;   
    AIAgent*  aiAgent;

    AI4::GAME_STATE state;
};

#endif
