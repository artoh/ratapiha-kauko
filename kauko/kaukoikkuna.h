#ifndef KAUKOIKKUNA_H
#define KAUKOIKKUNA_H

#include <QMainWindow>

#include <QToolBar>
#include <QComboBox>

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
    void luoTyokalupalkit();

private:
    KaukoScene *scene_;
    KaukoView *view_;

    QToolBar *hallintaToolBar_;
    QComboBox *nakymaCombo_;

};

#endif // KAUKOIKKUNA_H
