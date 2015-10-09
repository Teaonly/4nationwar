TEMPLATE	= app
TARGET		= ai4
CONFIG		+= qt warn_on debug console

CONFIG(debug, debug|release) {
    DESTDIR = debug
	OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
} else {
    DESTDIR = release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
}



HEADERS		= mainwindow.h boardview.h boardscene.h pieceitem.h aiagent.h
SOURCES		= main.cpp mainwindow.cpp boardview.cpp boardscene.cpp pieceitem.cpp aiagent.cpp

HEADERS    += ../engine/piece.h\
              ../engine/boardcontext.h\
              ../engine/cpu.h\
              ../engine/ai4nationwar.h\
              ../engine/game.h\
              ../engine/randman.h\
              ../engine/simpleman.h\
              ../engine/player.h\
              ../engine/router.h\
              ../engine/firetable.h\
              ../engine/gtree.h

SOURCES    += ../engine/boardcontext.cpp\
              ../engine/game.cpp\
              ../engine/piece.cpp\
              ../engine/player.cpp\
              ../engine/randman.cpp\
              ../engine/simpleman.cpp\
              ../engine/router.cpp\
              ../engine/gtree.cpp\
              ../engine/firetable.cpp\
              ../engine/cpu.cpp

RESOURCES  += ai4.qrc


