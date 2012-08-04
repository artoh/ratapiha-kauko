/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 6.1.2012
**************************************************************************/

#ifndef KAUKOVIEW_H
#define KAUKOVIEW_H

#include <QGraphicsView>
#include "kaukoscene.h"

class KaukoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit KaukoView(KaukoScene *skene );

    enum KaukoTila { Vierita,
                     JunaKulkutieAlkaa,
                     JunaKulkutiePaattyy,
                     VaihtoKulkutieAlkaa,
                     VaihtoKulkutiePaattyy,
                     PeruKulkutie,
                     VaihteenKaanto,
                     SeisKasky,
                     AjaKasky,
                     JunaNumeronSyotto,
                     AukiajetunVaihteenKaanto,
                     AutomaatioOn,
                     AutomaatioEi,
                     AlHp} ;
    


    int tila() { return tila_; }

signals:
    void tilaVaihtunut(int tila);
    
public slots:
    void valitseTila( int tila);

protected:

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QString raiteenPaaTunnusPisteessa( QPoint piste);

    QString alkaaRaiteesta_;    /** Ensimmäisen klikkauksen pää+raidetunnus */
    int tila_;

    void komento(const QString& komento);   /** Välittää komennon asetinlaitteelle */
};

#endif // KAUKOVIEW_H
