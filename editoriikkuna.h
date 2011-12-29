/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORIIKKUNA_H
#define EDITORIIKKUNA_H

#include "editoriikkuna.h"
#include "editoriscene.h"
#include "editoriview.h"

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QMainWindow>

class EditoriIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditoriIkkuna(int nakyma, QWidget *parent = 0);
    
signals:
    
public slots:
    void nakymanVaihto(int valintaind);

protected:
    void luoAktiot();
    void luoTyoPalkki();

    QToolBar* hallintaToolBar_;
    QComboBox* nakymaValinta_;

    EditoriScene *skene_;
    EditoriView* view_;
    
};

#endif // EDITORIIKKUNA_H
