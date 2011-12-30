/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KISKO_H
#define KISKO_H

#include <QGraphicsItem>

/** Kiskojen abstrakti kantaluokka

    Abstrakti kantaluokka KaukoKiskolle ja EditoriKiskolle.
    Sisältää kiskossa tarvittavia tietorakenteita

  */
class Kisko : public QGraphicsItem
{
public:
    enum Laituri { LaituriEi, LaituriVasemmalla, LaituriOikealla };
    enum PaanTyyppi { Valille, Paa, VaihdeVasen, VaihdeOikea };

    Kisko( const QLineF& viiva, int kiskoid=0, const QString& liikennepaikka=QString(), int raide = 0,
           const QString& kiskodata = QString(), int sn=0);


    qreal pituus() const { return pituus_; }
    Laituri laituri() const { return laituri_; }
    QString liikennePaikka() const {return liikennepaikka_; }
    int raide() const { return raide_; }

    QPointF etelainen() const { return viiva_.p1(); }
    QPointF pohjoinen() const { return viiva_.p2(); }

    PaanTyyppi etelaTyyppi() const { return etelapaaTyyppi_; }
    PaanTyyppi pohjoisTyyppi() const { return pohjoispaaTyyppi_; }

signals:
    
public slots:


protected:
    int kiskoid_;
    QString liikennepaikka_;
    int raide_;
    int sn_;
    Laituri laituri_;
    QLineF viiva_;
    PaanTyyppi etelapaaTyyppi_;
    PaanTyyppi pohjoispaaTyyppi_;

    qreal pituus_; /** Kiskon pituus */

    
};

#endif // KISKO_H
