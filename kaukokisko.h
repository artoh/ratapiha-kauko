/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOKISKO_H
#define KAUKOKISKO_H

#include "laajennettukisko.h"

#include <QString>
#include <QLineF>
#include <QBrush>

class KaukoScene;
class RaideTieto;
class RaiteenPaa;

class KaukoKisko : public LaajennettuKisko
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
    QBrush laatikonBrushPaalle(RaiteenPaa* paa);

    KaukoScene* skene_;
    QString raidetunnus_;
    Laituri laituri_;



    RaideTieto* raidetieto_;


};

#endif // KAUKOKISKO_H
