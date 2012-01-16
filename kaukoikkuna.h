/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOIKKUNA_H
#define KAUKOIKKUNA_H

#include "kaukoscene.h"
#include "kaukoview.h"

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QAction>


class KaukoIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit KaukoIkkuna(QWidget *parent = 0);
    
signals:
    
public slots:
    void uusiIkkuna();
    void nakymanVaihto(int valintaind);
    void editori();


protected:
    void luoTyoPalkki();
    void luoAktiot();


    QToolBar* hallintaToolBar_;
    QComboBox* nakymaValinta_;

    QAction* uusiIkkunaAktio_;
    QAction* editoriAktio_;


    KaukoScene* skene_;
    KaukoView* view_;

};

#endif // KAUKOIKKUNA_H
