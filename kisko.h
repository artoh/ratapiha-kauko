/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KISKO_H
#define KISKO_H

#include <QGraphicsItem>
#include <QList>


/** Kiskojen abstrakti kantaluokka

    Abstrakti kantaluokka KaukoKiskolle ja EditoriKiskolle.
    Sisältää kiskossa tarvittavia tietorakenteita

  */
class Kisko : public QGraphicsItem
{
public:
    enum Laituri { LaituriEi = 0, LaituriVasemmalla = 1, LaituriOikealla = 2, LaituriMolemmat = 3};
    enum PaanTyyppi { Virhe = 0, Valille = 1, Paa = 2, LiikennePaikanPaa = 3, RaidePuskuri = 4 /** Päättyvä raide */ ,
                      VaihdeJatkos = 10, VaihdeVasen = 11, VaihdeOikea = 12 };

    Kisko( const QLineF& viiva, int kiskoid=0, const QString& liikennepaikka=QString(), int raide = 0,
           const QString& kiskodata = QString());


    qreal pituus() const { return pituus_; }
    Laituri laituri() const { return laituri_; }
    QString liikennePaikka() const {return liikennepaikka_; }
    int kiskoId() const { return kiskoid_; }
    int raide() const { return raide_; }


    bool silta() const { return silta_; }

    QPointF etelainen() const { return viiva_.p1(); }
    QPointF pohjoinen() const { return viiva_.p2(); }

    PaanTyyppi etelaTyyppi() const { return etelapaaTyyppi_; }
    PaanTyyppi pohjoisTyyppi() const { return pohjoispaaTyyppi_; }

    /** Hakee naapurikiskot sanotusta sijainnista (yleensä päästä) */
    QList<Kisko*> haeNaapurit(QPointF sijainnista);

    QLineF viiva() const { return viiva_; }
    void tarkistaPaanTyypit();

    /** Vaihtaa etelan pohjoiseksi ja pohjoisen eteläksi*/
    void kaannaSuunta();


signals:
    
public slots:


protected:
    int kiskoid_;
    QString liikennepaikka_;
    int raide_;
    Laituri laituri_;
    QLineF viiva_;
    PaanTyyppi etelapaaTyyppi_;
    PaanTyyppi pohjoispaaTyyppi_;

    bool silta_;

    qreal pituus_; /** Kiskon pituus */

private:
    PaanTyyppi tarkistaTyyppiPaalle( QPointF piste );

    
};

#endif // KISKO_H
