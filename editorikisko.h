/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORIKISKO_H
#define EDITORIKISKO_H

#include "kisko.h"
#include <QLineF>

class EditoriScene;

class EditoriKisko : public Kisko
{
public:
    EditoriKisko(EditoriScene* skene, const QLineF& viiva, int kiskoid=0, const QString& liikennepaikka=QString(), int raide = 0,
                 const QString& kiskodata = QString(), int sn=0);


    enum { Type = UserType + 2 } ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }

    void valitse(bool onko=true);

    void asetaLiikennepaikka(const QString& lyhenne);
    void asetaRaide(int raide);
    /** Asettaa raiteen ja liikenenpaikan myös vaihdekavereille*/
    void levitaRaiteenAsetus();

protected:
    // Tarkastetaan päiden tyypit ja kaverit tekee myös saman
    void paidenTarkistusToimet();

    EditoriScene* skene_;
    bool valittu_;

};

#endif // EDITORIKISKO_H
