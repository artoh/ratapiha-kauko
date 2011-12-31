/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORITEKSTIKENTTA_H
#define EDITORITEKSTIKENTTA_H

#include <QGraphicsProxyWidget>

#include <QLineEdit>
#include <QPointF>

class EditoriScene;

/** Editoitava tekstikenttä, jota käytetään lisäämään tekstiä
    näkymään.
  */
class EditoriTekstiKentta : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit EditoriTekstiKentta(EditoriScene* skene, QPointF sijainti);
    
signals:
    
public slots:
    void muokkausValmis();

protected:
    QLineEdit* lineEdit_;
};

#endif // EDITORITEKSTIKENTTA_H
