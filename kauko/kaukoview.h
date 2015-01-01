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
};

#endif // KAUKOVIEW_H
