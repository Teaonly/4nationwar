#ifndef _BOARDSCENE_H_
#define _BOARDSCENE_H_

#include <vector>
#include <QtGui>
using namespace std;

class PieceItem;

class BoardScene : public QGraphicsScene {
	Q_OBJECT
public:
    BoardScene(QObject *parent = 0);

public slots:
    void onSelectedChanged();
signals:
    void pieceMove(PieceItem *, QPointF);
   
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

	void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );

    PieceItem *targetItem;   
    QPointF   targetPos;

    QPixmap*   backgroud_pixmap;
};
#endif
