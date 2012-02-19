/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOKISKO_H
#define KAUKOKISKO_H

#include "kisko.h"

#include <QString>
#include <QLineF>

class KaukoScene;
class RaideTieto;

class KaukoKisko : public Kisko
{
public:


    KaukoKisko(KaukoScene* skene,
               const QLineF& viiva,
               int kiskoid,
               const QString& liikennepaikka,
               int raide,
               const QString& kiskodata);

    enum { Type = UserType + 1 } ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }

    void asetaRaide( RaideTieto* praidetieto);

protected:
    RaideTieto* raidetieto() { return raidetieto_; }


    KaukoScene* skene_;
    QString raidetunnus_;
    Laituri laituri_;


    bool naytajunanumero_;
    bool naytaraidenumero_;

    RaideTieto* raidetieto_;


};

#endif // KAUKOKISKO_H
