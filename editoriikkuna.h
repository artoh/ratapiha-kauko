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
#include <QLineEdit>

class EditoriIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditoriIkkuna(int nakyma, QWidget *parent = 0);
    
    QString nykyLiikennePaikka() const;

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

protected:
    void luoAktiot();
    void luoTyoPalkki();
    void haeNakymaLista();
    void haeLiikennepaikkaLista();

    QAction* osoitinAktio_;
    QAction* piirraAktio_;

    QToolBar* hallintaToolBar_;
    QToolBar* muokkausToolBar_;
    QToolBar* tunnisteToolBar_;
    QComboBox* nakymaValinta_;
    QComboBox* liikennepaikkaCombo_;
    QLineEdit* raideLineEdit_;

    EditoriScene *skene_;
    EditoriView* view_;
    
};

#endif // EDITORIIKKUNA_H
