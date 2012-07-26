#ifndef JUNANUPPI_H
#define JUNANUPPI_H

#include <QGraphicsItem>
#include "veturi.h"

/** Pienennetyssä karttanäkymässä junien kohdalle piirrettävä nupikka*/
class JunaNuppi : public QGraphicsItem
{
public:
    JunaNuppi(Veturi* veturi);


    int type() const { return UserType + 600; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected:
    Veturi* veturi_;


};



#endif // JUNANUPPI_H
