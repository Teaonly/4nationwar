#include <QtGui>

#include "pieceitem.h"
#include "boardscene.h"

using namespace std;

PieceItem::PieceItem(BoardScene *scene, QPixmap *img)
	:QGraphicsPixmapItem(*img), pScene(scene), pImg(img) {
	
    setFlag(QGraphicsItem::ItemIsMovable, false);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setCursor(Qt::PointingHandCursor);
}

PieceItem::~PieceItem() {
		
}

