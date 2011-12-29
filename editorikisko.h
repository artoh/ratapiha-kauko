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


protected:
    EditoriScene* skene_;

};

#endif // EDITORIKISKO_H
