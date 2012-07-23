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
#include <QActionGroup>
#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

class EditoriRaide;

class EditoriIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditoriIkkuna(int nakyma, QWidget *parent = 0);
    
    QString nykyLiikennePaikka() const;
    int nykyNopeusRajoitus() const;

signals:


public slots:
    void nakymanVaihto(int valintaind);
    /** Tämä tulee Actionilta */
    void vaihdaTilaActionilta();
    void laitaTilaNappiAlas(int valittuTila);
    void liikennePaikkaValittu(int index);
    void raideNumeroaMuutettu();

    /** Kun kisko valittu, päivitetään valintoja sen mukaisiksi*/
    void kiskoValittu(EditoriKisko* kisko);

    /** Kun klikataan jotain kiskon määritettä.. */
    void paivitaKiskonMaareet();

    void paivitaRaiteenMaareet();

    void paivitaOpastimet();

protected:
    void luoAktiot();
    void luoTyoPalkki(bool ratanakyma=false);
    void haeNakymaLista();
    void haeLiikennepaikkaLista();
    void teeSnCombo();

    /** Määrittää, mitä opastimia voidaan valita*/
    void opastintenHimmennys(EditoriRaide* raidePointteri);

    QAction* osoitinAktio_;
    QAction* vieritysAktio_;
    QAction* piirraAktio_;
    QAction* pyyhiAktio_;
    QAction* tekstiAktio_;
    QAction* viivainAktio_;

    QActionGroup* valittuihinLiittyvat_;

    QAction* laituriVasenAktio_;
    QAction* laituriOikeaAktio_;
    QAction* siltaAktio_;

    QAction* naytaJunanumerotAktio_;
    QAction* naytaaRaidenumerotAktio_;

    QAction* naytaNopeusRajoitusAktio_;

    QActionGroup* kulkutietyypitAktiot_;
    QAction* ensisijainenKulkutieAktio_;
    QAction* toissijainenKulkutieAktio_;
    QAction* vainVaihtotieAktio_;

    QActionGroup* opastinAktiot_;

    QAction* poEtelaanAktio_;
    QAction* poPohjoiseenAktio_;
    QAction* roEtelaanAktio_;
    QAction* roPohjoiseenAktio_;
    QAction* soEtelaanAktio_;
    QAction* soPohjoiseenAktio_;
    QAction* eoEtelaanAktio_;
    QAction* eoPohjoiseenAktio_;
    QAction* raideRisteysAktio_;

    QActionGroup* raiteenMaareAktiot_;
    QAction* sahkoistettyAktio_;
    QAction* valvomatonAktio_;

    QAction* spEtelaanAktio_;
    QAction* spPohjoiseenAktio_;

    QToolBar* hallintaToolBar_;
    QToolBar* muokkausToolBar_;
    QToolBar* tunnisteToolBar_;
    QToolBar* rataKiskonToolBar_;
    QToolBar* nakymaKiskonToolBar_;


    QComboBox* nakymaValinta_;
    QComboBox* liikennepaikkaCombo_;
    QLabel* metriLabel_;
    QLineEdit* raideLineEdit_;
    QComboBox* snCombo_;

    EditoriScene *skene_;
    EditoriView* view_;
    
};

#endif // EDITORIIKKUNA_H
