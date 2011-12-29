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

class EditoriView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit EditoriView(EditoriScene* skene);
    enum EditoriTila { Osoitin, Piirto } ;

    
signals:
    void editorinTilaVaihtunut(int tila);
    void naytettavaRaiteenPituus(qreal pituus);
    
public slots:

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    static qreal pyorista(qreal koordinaatti);
    static QPointF kohdista(const QPointF& pos);

public:
    EditoriTila tila() const { return tila_; }

private:
    EditoriScene* skene_;
    EditoriTila tila_;
    QGraphicsLineItem* piirtoViiva_;
};

#endif // EDITORIVIEW_H
