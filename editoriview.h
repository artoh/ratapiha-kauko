/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 18.6.2011
**************************************************************************/

#ifndef EDITORIVIEW_H
#define EDITORIVIEW_H


#include "editoriscene.h"
#include <QGraphicsView>
#include <QGraphicsLineItem>

class EditoriKisko;
class EditoriIkkuna;

class EditoriView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit EditoriView(EditoriScene* skene, EditoriIkkuna* ikkuna);
    enum EditoriTila { Osoitin, Piirto, Pyyhi, Teksti } ;

    EditoriKisko* valittuKisko() { return valittuKisko_; }

signals:
    void editorinTilaVaihtunut(int tila);
    void naytettavaRaiteenPituus(qreal pituus);
    void kiskoValittu(EditoriKisko* kisko);
    
public slots:
    /** Valitsee käytettävän näyttötilan */
    void valitseTila( int tila);

    void poistaValinta();

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    static qreal pyorista(qreal koordinaatti);
    static QPointF kohdista(const QPointF& pos);

public:
    int tila() const { return tila_; }

private:
    EditoriScene* skene_;
    EditoriIkkuna* ikkuna_;
    int tila_;  // int jotta tilaa voidaan muuttaa
    QGraphicsLineItem* piirtoViiva_;
    EditoriKisko* valittuKisko_;
};

#endif // EDITORIVIEW_H
