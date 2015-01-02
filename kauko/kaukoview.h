#ifndef KAUKOVIEW_H
#define KAUKOVIEW_H

#include <QGraphicsView>

#include "kaukoscene.h"

class KaukoView : public QGraphicsView
{
    Q_OBJECT
public:
    KaukoView(KaukoScene *scene);
    ~KaukoView();
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // KAUKOVIEW_H
