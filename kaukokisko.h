/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOKISKO_H
#define KAUKOKISKO_H

#include <QGraphicsItem>
#include <QString>
#include <QLineF>

class KaukoScene;

class KaukoKisko : public QGraphicsItem
{
public:
    enum Laituri { LaituriEi, LaituriVasen, LaituriOikea };

    KaukoKisko(KaukoScene* skene,
               const QLineF& viiva, int numero, Laituri laituri,
               char kiskotyyppi, char etelatyyppi, char pohjoistyyppi,
               bool naytajunanumero, bool naytaraidenumero);

    enum { Type = UserType + 1 } ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }

    qreal pituus() const { return pituus_; }

protected:
    KaukoScene* skene_;
    qreal pituus_;
    QString raidetunnus_;
    Laituri laituri_;

    char kiskotyyppi_;
    char etelatyyppi_;
    char pohjoistyyppi_;
    bool naytajunanumero_;
    bool naytaraidenumero_;


};

#endif // KAUKOKISKO_H
