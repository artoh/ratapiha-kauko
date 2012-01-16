/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 6.1.2012
**************************************************************************/

#ifndef KAUKOVIEW_H
#define KAUKOVIEW_H

#include <QGraphicsView>
#include "kaukoscene.h"

class KaukoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit KaukoView(KaukoScene *skene );
    
signals:
    
public slots:

protected:
    void wheelEvent(QWheelEvent *event);

    
};

#endif // KAUKOVIEW_H
