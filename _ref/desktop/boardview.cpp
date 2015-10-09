#include <cmath>
#include "boardview.h"

BoardView::BoardView(QGraphicsScene *pScene):QGraphicsView(pScene) {

    setDragMode(QGraphicsView::NoDrag);	//RubberBandDrag,NoDrag,ScrollHandDrag
	setTransformationAnchor(AnchorUnderMouse);
	setResizeAnchor(AnchorViewCenter);
	//setRubberBandSelectionMode(Qt::ContainsItemShape); 

	/*
	QMatrix m(1,0,0,-1,0,0);
	setTransform( QTransform(m) );
	*/
    //Qt::ScrollBarAlwaysOff
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOff); 
}

