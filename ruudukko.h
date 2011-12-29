/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 28.12.2011
**
** Ratapiha - kauko
**************************************************************************/

#ifndef RUUDUKKO_H
#define RUUDUKKO_H

#include <QGraphicsObject>
#include <QRectF>

/**   Editori-ikkunassa näytettävä 10 x 10 (metriä) tiheä ruudukko

  */
class Ruudukko : public QGraphicsObject
{
    Q_OBJECT
public:
    Ruudukko(QGraphicsScene* sceene);
    ~Ruudukko();

    int type() const { return UserType + 800; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;


public slots:
    void kokoMuuttunut(QRectF uusikoko);

protected:
    QRectF koko_;
    
};

#endif // RUUDUKKO_H
