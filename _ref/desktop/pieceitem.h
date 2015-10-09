#ifndef _PIECEITEM_H_
#define _PIECEITEM_H_

#include <QtGui>
#include "../engine/ai4nationwar.h"

class BoardScene;
class PieceItem: public QGraphicsPixmapItem {
public:
	PieceItem(BoardScene *, QPixmap *);
	~PieceItem();
    int boardY;
    int boardX;
    AI4::PIECE_COLOR color;
private:
	BoardScene *pScene;
    QPixmap  *pImg;
	friend class BoardScene;
};

#endif
