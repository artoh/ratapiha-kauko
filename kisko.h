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
    enum Laituri { LaituriEi, LaituriVasen, LaituriOikea };


    Kisko( const QLineF& viiva);


    qreal pituus() const { return pituus_; }
    
signals:
    
public slots:


protected:
    int kiskoid_;
    QString liikennepaikka_;
    int raide_;
    int sn_;
    Laituri laituri_;

    qreal pituus_; /** Kiskon pituus */

    
};

#endif // KISKO_H
