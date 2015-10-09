#ifndef _BOARDVIEW_H_
#define _BOARDVIEW_H_

#include <QtGui>

class BoardView : public QGraphicsView {
    Q_OBJECT

public:
    BoardView(QGraphicsScene *pScene);

};

#endif
