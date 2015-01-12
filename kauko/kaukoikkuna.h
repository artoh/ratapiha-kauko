#ifndef KAUKOIKKUNA_H
#define KAUKOIKKUNA_H

#include <QMainWindow>

#include <QToolBar>
#include <QComboBox>
#include <QActionGroup>

#include "kaukoscene.h"
#include "kaukoview.h"

class KaukoIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit KaukoIkkuna(QWidget *parent = 0);
    ~KaukoIkkuna();

signals:
    void vaihdaNakyma(int nakymaId);

public slots:
    void lisaaNakymaListaan(int id, const QString& nimi);
    void nakymanVaihto(int valintaIndeksi);

protected:
    void luoAktiot();
    void luoTyokalupalkit();

private slots:
    void vaihdaTila(QAction *action);

private:
    KaukoScene *scene_;
    KaukoView *view_;

    QToolBar *hallintaToolBar_;
    QComboBox *nakymaCombo_;

    QToolBar *aslToolBar_;

    QActionGroup *aslAktiot_;

};

#endif // KAUKOIKKUNA_H
