/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011, 2012
** Gnu Public Licence 2 - No Warranty !!!
** 2.1.2012
**************************************************************************/
#ifndef MITTANAUHA_H
#define MITTANAUHA_H

#include <QGraphicsItem>
#include <QLineF>

/** Piirron apuväline, jossa viivat sadan metrin, viidensadan metrin ja kilometrin välein.*/
class MittaNauha : public QGraphicsItem
{
public:
    MittaNauha(QLineF viiva);

    int type() const { return UserType + 800; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected:
    qreal pituus_;
};


#endif // MITTANAUHA_H
