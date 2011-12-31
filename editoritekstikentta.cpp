/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoritekstikentta.h"
#include "editoriscene.h"

#include <QFont>
#include <QSizePolicy>
#include <QGraphicsView>

EditoriTekstiKentta::EditoriTekstiKentta(EditoriScene *skene, QPointF sijainti) :
    QGraphicsProxyWidget()
{
    lineEdit_ = new QLineEdit;
    setWidget(lineEdit_);
    lineEdit_->setFont(QFont("Helvetica",5));
    lineEdit_->setFrame(true);

    setPos( sijainti);
    setFocusPolicy(Qt::ClickFocus);
    skene->addItem(this);
    setFocus();

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resize(100.0, 12.0);

    connect( lineEdit_, SIGNAL(editingFinished()), this, SLOT(muokkausValmis()));
}

void EditoriTekstiKentta::muokkausValmis()
{

}

