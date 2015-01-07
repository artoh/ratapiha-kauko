#ifndef KAUKOVIEW_H
#define KAUKOVIEW_H

#include <QGraphicsView>

#include "kaukoscene.h"

class KaukoView : public QGraphicsView
{
    Q_OBJECT
public:
    enum KaukoTila
    {
        VIERITA,
        JUNAKULKUTIE_ALKAA,
        JUNAKULKUTIE_PAATTYY
    };


    KaukoView(KaukoScene *scene);
    ~KaukoView();


signals:
    void tilaVaihtunut(int tila);
public slots:
    void valitseTila(int tila);


protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QString raiteenPaaPisteessa(const QPoint& piste);

    KaukoScene* skene_;
    int tila_;

    /** Kulkutiekäskyssä tähän talletetaan se raiteenpää, josta kulkutie alkaa */
    QString alkaaTunnuksesta_;
};

#endif // KAUKOVIEW_H
