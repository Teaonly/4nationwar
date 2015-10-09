#include <iostream>
#include "boardscene.h"
#include "pieceitem.h"

BoardScene::BoardScene(QObject *parent)
    : QGraphicsScene(parent) {

    setSceneRect(0, 0, 680, 680);
    backgroud_pixmap = new QPixmap(":res/images/board.png");

    QObject::connect(this,  SIGNAL(selectionChanged()), this, SLOT(onSelectedChanged()));
}

void BoardScene::drawBackground(QPainter *painter, const QRectF&) {
    if ( backgroud_pixmap ) {
        QPoint centerPoint(0,0);
        // Scale new image which size is widgetSize
        QSize widgetSize(680,680);
        QPixmap scaledPixmap = backgroud_pixmap->scaled(widgetSize, Qt::KeepAspectRatio);
        // Calculate image center position into screen
        centerPoint.setX((widgetSize.width()-scaledPixmap.width())/2);
        centerPoint.setY((widgetSize.height()-scaledPixmap.height())/2);
        // Draw image
        painter->drawPixmap(centerPoint, scaledPixmap);
    } 
}

void BoardScene::onSelectedChanged() {
    QList<QGraphicsItem *> items = selectedItems();
    if ( items.size() == 1) {
        targetItem = static_cast<PieceItem*>(items[0]);
    } else {
        emit pieceMove(targetItem, targetPos);
    }
} 

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if ( mouseEvent->modifiers() && Qt::ControlModifier) {
        return;
    }
    targetPos = mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent); 
}

void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

