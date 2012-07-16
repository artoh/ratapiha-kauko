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
#include <QLineEdit>


class KaukoIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit KaukoIkkuna(QWidget *parent = 0);
    
signals:
    
public slots:
    void rataIkkuna();
    void uusiIkkuna();
    void nakymanVaihto(int valintaind);
    void editori();

    void kasky();

    void vaihdaTila();
    void paivitaNapit(int tila);

protected:
    void luoTyoPalkki();
    void luoAktiot();


    QToolBar* hallintaToolBar_;
    QToolBar* asetinlaiteToolBar_;
    QComboBox* nakymaValinta_;

    QAction* rataIkkunaAktio_;
    QAction* uusiIkkunaAktio_;
    QAction* editoriAktio_;

    QAction* kulkutieAktio_;
    QAction* vaihtoKulkutieAktio_;
    QAction* puraKulkutieAktio_;
    QAction* kaannaVaihdeAktio_;
    QAction* seisAktio_;
    QAction* ajaAktio_;

    QLineEdit* kaskyLine_;

    KaukoScene* skene_;
    KaukoView* view_;

};

#endif // KAUKOIKKUNA_H
