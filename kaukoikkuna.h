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
#include "automaatiomuokkain.h"

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QLineEdit>
#include <QLabel>
#include <QDockWidget>

class RatapihaIkkuna;

class KaukoIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit KaukoIkkuna(RatapihaIkkuna *parent = 0);
    
signals:
    
public slots:
    void uusiIkkuna();
    void nakymanVaihto(int valintaind);
    void editori();

    void kasky();

    void vaihdaTila(QAction* aktio);
    void paivitaNapit(int tila);

    void yhteysAsettimeen(bool onkoYhteytta);

    void paivitaKello(const QDateTime& aika);
    void muokkaaAutomaatiota(const QString& opastin);


protected:
    void luoTyoPalkki();
    void luoAktiot();
    void luoTelakka();


    QToolBar* hallintaToolBar_;
    QToolBar* asetinlaiteToolBar_;
    QToolBar* hataVaraistenToolBar_;
    QComboBox* nakymaValinta_;

    QAction* uusiIkkunaAktio_;
    QAction* editoriAktio_;

    QActionGroup* aslAktiot_;

    QAction* vieritaAktio_;
    QAction* kulkutieAktio_;
    QAction* vaihtoKulkutieAktio_;
    QAction* varattuKulkutieAktio_;
    QAction* puraKulkutieAktio_;
    QAction* kaannaVaihdeAktio_;
    QAction* seisAktio_;
    QAction* ajaAktio_;
    QAction* automaatioAktio_;
    QAction* automaatioPoisAktio_;
    QAction* alhpAktio_;

    QAction* junanumeroAktio_;
    QAction* aukiajetunKaantoAktio_;

    QAction* automaatioMuokkausAktio_;

    QLineEdit* kaskyLine_;

    KaukoScene* skene_;
    KaukoView* view_;

    RatapihaIkkuna* rpIkkuna_;

    QLabel *aikaLabel_;

    QDockWidget* autoMuokkausDock_;
    AutomaatioMuokkain* autoMuokkain_;

};

#endif // KAUKOIKKUNA_H
