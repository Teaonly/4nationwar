#include <string>
#include <sstream>
#include <iostream>

#include "mainwindow.h"
#include "boardview.h"
#include "boardscene.h"
#include "pieceitem.h"
#include "aiagent.h"

#include "../engine/game.h"
#include "../engine/player.h"
#include "../engine/boardcontext.h"
#include "../engine/piece.h"
#include "../engine/cpu.h"

MainWindow::MainWindow() {
	InitGui();
	setupSignal();

    pScene = new BoardScene();	
	pView = new BoardView(pScene);
    for(int i = 0; i < 100; i++)
        pItems[i] = NULL;
	setCentralWidget(pView);
    QObject::connect(pScene, SIGNAL(pieceMove(PieceItem *, QPointF)), this, SLOT(onPieceMove(PieceItem *, QPointF)) );
    

    InitGame();    
    restart();    
}

MainWindow::~MainWindow() {
         
}

void MainWindow::InitGame() {
    gameControler = new Game(); 
    players[0] = new Player(AI4::PC_SOUTH, NULL);
    gameControler->setPlayer(players[0]);   

    std::string  cpuName = "Random";
    CPU* cpu = CPU::createCPU(cpuName, NULL);
    players[1] = new Player(AI4::PC_WEST, cpu); 
    gameControler->setPlayer(players[1]);   

    cpu = CPU::createCPU(cpuName, NULL);
    players[2] = new Player(AI4::PC_NORTH, cpu); 
    gameControler->setPlayer(players[2]);   
    
    cpu = CPU::createCPU(cpuName, NULL);
    players[3] = new Player(AI4::PC_EAST, cpu); 
    gameControler->setPlayer(players[3]);   
    
    state = AI4::GS_PREPARE;
}

void MainWindow::InitGui() {
	setWindowTitle(tr("AI4"));

	createActions();
	createMenus();

    resize(680, 700);
    setMinimumSize(QSize(680, 700));
    setMaximumSize(QSize(680, 700));
    
    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );
}

void MainWindow::setupSignal() {
	connect(restartAct, SIGNAL(triggered()), this, SLOT(restart()));
	connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
    connect(testAct, SIGNAL(triggered()), this, SLOT(test()));
}

void MainWindow::createActions() {
	restartAct = new QAction(tr("Restart"), this);
	quitAct = new QAction(tr("Quit"), this);
    testAct = new QAction(tr("Test"), this);
}

void MainWindow::createMenus() {
    QMenuBar* menu = menuBar();

	QMenu * fileMenu = new QMenu(tr("Game"), menu);
	fileMenu->addAction(restartAct);
    fileMenu->addAction(testAct);
    fileMenu->addSeparator();
	fileMenu->addAction(quitAct);
    menu->addMenu(fileMenu);
    
    menu->setGeometry(QRect(0, 0, 680, 20));
}


void MainWindow::quit() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event = event;
}

void MainWindow::timerEvent(QTimerEvent *event) {
    event = event;	
}

static const char *sample = "\
627029483051142a095a1a3b6\
6071340569289123a2a45ba10\
405239960027126835a411aba\
367011289401235a59406b2aa";

static void board2Scene(int *y, int *x) {
    const int offsetX = 5;
    const int offsetY = 5;

    if ( *y >= 2) {
        if ( *x >=0 && *x <=4) {
            *x = (6 + *x) * 40 + offsetX;
            *y = (6 + 5 + *y - 2) * 40 + offsetY;
        } else if ( *x >= 5 && *x <= 9) {
            int tempX = *x;
            *x = (6 + 5 + *y - 2) * 40 + offsetX;
            *y = (6 + 9 - tempX ) * 40 + offsetY;
        } else if ( *x >= 10 && *x <= 14) {
            *x = (6 + 14 - *x) * 40 + offsetX;
            *y = (7 - *y) * 40 + offsetY;
        } else {
            int tempX = *x;
            *x = (7 - *y) * 40 + offsetX;
            *y = (6 + tempX - 15) * 40 + offsetY;
        }
    } else {
        if ( *y == 0) {
            *x = 8*40 + offsetX;
            *y = 8*40 + offsetY;
        } else if ( *x == 0) {
            *x = 6 * 40 + offsetX;
            *y = 10 * 40 + offsetY;
        } else if ( *x == 1) {
            *x = 8 * 40 + offsetX;
            *y = 10 * 40 + offsetY;
        } else if ( *x == 2) {
            *x = 10 * 40 + offsetX;
            *y = 10 * 40 + offsetY;
        } else if ( *x == 3) {
            *x = 10 * 40 + offsetX;
            *y = 8 * 40 + offsetY;
        } else if ( *x == 4) {
            *x = 10 * 40 + offsetX;
            *y = 6 * 40 + offsetY;
        } else if ( *x == 5) {
            *x = 8 * 40 + offsetX;
            *y = 6 * 40 + offsetY;
        } else if ( *x == 6) {
            *x = 6 * 40 + offsetX;
            *y = 6 * 40 + offsetY;
        } else if ( *x == 7) {
            *x = 6 * 40 + offsetX;
            *y = 8 * 40 + offsetY;
        }
    } 
}

static void scene2Board(int *y, int *x) {
    const int offsetX = 5;
    const int offsetY = 5;

    int sy = (*y - offsetY) / 40;
    int sx = (*x - offsetX) / 40;

    *x = -1;
    *y = -1;

    if ( sx >= 6 && sx <= 10 && sy >= 11) {
        *y = sy - 9;
        *x = sx - 6;
    } else if ( sx >= 11 && sy >= 6 && sy <= 10) {
        *x = 15 - sy;
        *y = sx - 9;
    } else if ( sx >= 6 && sx <= 10 && sy <= 5) {
        *x = 20 - sx;
        *y = 7 - sy;
    } else if ( sx <= 5 && sy >= 6 && sy <= 10) {
        *x = sy + 9;
        *y = 7 - sx;    
    } else if ( sx >= 6 && sx <= 10 && sy >= 6 && sy <=10) {
        if ( sx == 8 && sy == 8) {
            *x = 0;
            *y = 0;
        } else if ( sx == 6 && sy == 10) {
            *x = 0;
            *y = 1;
        } else if ( sx == 8 && sy == 10) {
            *x = 1;
            *y = 1;
        } else if ( sx == 10 && sy == 10) {
            *x = 2;
            *y = 1;
        } else if ( sx == 10 && sy == 8) {
            *x = 3;
            *y = 1;
        } else if ( sx == 10 && sy == 6) {
            *x = 4;
            *y = 1;
        } else if ( sx == 8 && sy == 6) {
            *x = 5;
            *y = 1;
        } else if ( sx == 6 && sy == 6) {
            *x = 6;
            *y = 1;
        } else if ( sx == 6 && sy == 8) {
            *x = 7;
            *y = 1;
        }
    } 

}

void MainWindow::restart() {

    if ( state != AI4::GS_PREPARE && state != AI4::GS_SOUTH) {
        // waiting for AI finishing
    }

    Piece    pieces[100];
    for (int n=0; n < 4; n++) {
        AI4::PIECE_COLOR pc = (AI4::PIECE_COLOR)n;
        for ( int i = 0; i < 25; i++) {
            pieces[n*25 + i].setRole(pc, AI4::PR_MINE, 5, 10);
        }
    }
    
    // set the position value
    for (int n=0; n < 4; n++) {
        for ( int i = 0; i < 5; i++) {
            pieces[n*25 + i].x = n*5 + i;
            pieces[n*25 + i].y = 2;
        }

        pieces[n*25 + 5].x = n*5 + 0;
        pieces[n*25 + 5].y = 3;
        pieces[n*25 + 6].x = n*5 + 2;
        pieces[n*25 + 6].y = 3;
        pieces[n*25 + 7].x = n*5 + 4;
        pieces[n*25 + 7].y = 3;

        pieces[n*25 + 8].x = n*5 + 0;
        pieces[n*25 + 8].y = 4;
        pieces[n*25 + 9].x = n*5 + 1;
        pieces[n*25 + 9].y = 4;
        pieces[n*25 + 10].x = n*5 + 3;
        pieces[n*25 + 10].y = 4;
        pieces[n*25 + 11].x = n*5 + 4;
        pieces[n*25 + 11].y = 4;

        pieces[n*25 + 12].x = n*5 + 0;
        pieces[n*25 + 12].y = 5;
        pieces[n*25 + 13].x = n*5 + 2;
        pieces[n*25 + 13].y = 5;
        pieces[n*25 + 14].x = n*5 + 4;
        pieces[n*25 + 14].y = 5;

        for ( int i = 0; i < 5; i++) {
            pieces[n*25 + 15 + i].x = n*5 + i;
            pieces[n*25 + 15 + i].y = 6;
        }
        for ( int i = 0; i < 5; i++) {
            pieces[n*25 + 20 + i].x = n*5 + i;
            pieces[n*25 + 20 + i].y = 7;
        }

    }     

    // set the role value
    for (int i = 0; i < 100; i++) {
        AI4::PIECE_ROLE r;
        if ( sample[i] <= '9' ) {
            r = (AI4::PIECE_ROLE)(sample[i]-'0');
        } else {
            r = (AI4::PIECE_ROLE)(sample[i]-'a'+10);
        }
        pieces[i].role = r;
        
        std::string imageName;
        switch( pieces[i].color ) {
            case AI4::PC_SOUTH:
                imageName = "r";
                break;
            case AI4::PC_WEST:
                imageName = "d";
                break;
            case AI4::PC_NORTH:
                imageName = "b";
                break;
            case AI4::PC_EAST:
                imageName = "g";
        } 
        std::stringstream ss;
        ss << r;
        imageName = imageName + ss.str();
        imageName = ":res/images/" + imageName + ".png";
        
        if ( pItems[i] == NULL) {
            QPixmap *pixmap = new QPixmap( imageName.c_str() );
            pItems[i] = new PieceItem(pScene, pixmap);                 
        }
        pItems[i]->boardY = pieces[i].y;
        pItems[i]->boardX = pieces[i].x;
        pItems[i]->color = pieces[i].color;
        if ( pieces[i].color != AI4::PC_SOUTH) {
            pItems[i]->setFlag(QGraphicsItem::ItemIsSelectable, false);
        } else {
            pItems[i]->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
        int y = pieces[i].y;
        int x = pieces[i].x;
        board2Scene(&y, &x);
        pItems[i]->setX(x);
        pItems[i]->setY(y);
        
        //pScene->removeItem(pItems[i]);
        pScene->addItem(pItems[i]);
    }

    Piece* fourPieces[4];
    fourPieces[0] = &pieces[0];
    fourPieces[1] = &pieces[25];
    fourPieces[2] = &pieces[50];
    fourPieces[3] = &pieces[75];
    
    gameControler->setupPieces(fourPieces);
    state = AI4::GS_SOUTH;
}

void MainWindow::onAIFinish() {
    Move newMove = aiAgent->moveResult;
    AI4::PLAYER_ACTION ac = aiAgent->actionResult;
    
    if ( ac == AI4::AC_SKIP ) {
       gameControler->doUpdate(AI4::PIECE_COLOR(state), ac);
    } else if ( ac == AI4::AC_SURRENDER) {
       gameControler->doUpdate(AI4::PIECE_COLOR(state), ac);
       for(int i = 0; i < 100; i++) {
            if ( pItems[i]->color == AI4::PIECE_COLOR(state) ) {
                pScene->removeItem(pItems[i]);
            }
       }
    } else if ( ac == AI4::AC_MOVE ) {
        AI4::ACTION_RESULT ar =  gameControler->doAction(AI4::PIECE_COLOR(state), newMove);
        if ( ar == AI4::AR_ILLEGAL ) {
            QMessageBox::information(NULL, "Error", "AI make error!");
            return;
        }
        std::cout << "Move from (" << newMove.fromY << "," << newMove.fromX << ")->(" << newMove.toY << "," << newMove.toX << ")" << std::endl;
        gameControler->doUpdate(ar, newMove); 
        updateBoard(ar, newMove);
    }
   
    // jump to next player
    state = AI4::GAME_STATE((state + 3)%4);
    if ( state == AI4::GS_SOUTH) {
        return;
    }

    aiAgent = new AIAgent();    
    QObject::connect(aiAgent, SIGNAL(signalAIFinish()), this, SLOT(onAIFinish()), Qt::QueuedConnection );
    aiAgent->ExecuteAI(gameControler, AI4::PIECE_COLOR(state) );
}

void MainWindow::onPieceMove(PieceItem *from, QPointF to) {
    // verify current state
    if ( state != AI4::GS_SOUTH) 
        return;

    // check target position is in the board
    int y = to.y();
    int x = to.x();
    scene2Board(&y, &x);
    if ( y < 0 || x < 0)
        return;
    
    // check this move is illegal
    Move newMove;
    newMove.fromY = from->boardY;
    newMove.fromX = from->boardX;
    newMove.toY = y;
    newMove.toX = x;

    AI4::ACTION_RESULT ar =  gameControler->doAction(AI4::PC_SOUTH, newMove);
    if ( ar == AI4::AR_ILLEGAL ) {
        return;
    }
    
    gameControler->doUpdate(ar, newMove);
    updateBoard(ar, newMove);

    state = AI4::GS_WEST;
    aiAgent = new AIAgent();    
    QObject::connect(aiAgent, SIGNAL(signalAIFinish()), this, SLOT(onAIFinish()), Qt::QueuedConnection );
    aiAgent->ExecuteAI(gameControler, AI4::PC_WEST);
}

void MainWindow::updateBoard(AI4::ACTION_RESULT ar, Move newMove) {
    
    int y = newMove.fromY;
    int x = newMove.fromX;
    board2Scene(&y, &x);
    PieceItem *from = static_cast<PieceItem*>(pScene->itemAt(x+5,y+5)); 
    
    y = newMove.toY;
    x = newMove.toX;
    board2Scene(&y, &x);
    PieceItem *to = static_cast<PieceItem*>(pScene->itemAt(x+5,y+5)); 
    
    switch (ar) {
        case AI4::AR_MOVED:
            from->boardY = y = newMove.toY;
            from->boardX = x = newMove.toX;
            board2Scene(&y, &x);
            from->setY(y);
            from->setX(x);
            break;
       
        case AI4::AR_CAPTURE:
            pScene->removeItem(to);
            from->boardY = y = newMove.toY;
            from->boardX = x = newMove.toX;
            board2Scene(&y, &x);
            from->setY(y);
            from->setX(x);
            break;     
        
        case AI4::AR_LING_MINE:
        case AI4::AR_LOST:
            pScene->removeItem(from);            
            break;

        case AI4::AR_LING_BOMP:
        case AI4::AR_BOMP_LING:
        case AI4::AR_LING_LING:
        case AI4::AR_FIRE:
            pScene->removeItem(to);            
            pScene->removeItem(from);            
            break;

        case AI4::AR_FLAG:
            from->boardY = y = newMove.toY;
            from->boardX = x = newMove.toX;
            board2Scene(&y, &x);
            from->setY(y);
            from->setX(x);
            if ( to != NULL) {
                pScene->removeItem(to);     
                AI4::PIECE_COLOR pc = to->color;
                for(int i = 0; i < 100; i++) {
                    if ( pItems[i]->color == pc)
                        pScene->removeItem(pItems[i]);
                }
            }
            break;

        default:
            break;
    }
}

void MainWindow::test() {
    int ret = gameControler->checkBoard();
    std::cout << "Checing Game = " << ret << std::endl;
}
