/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 25.6.2011
**************************************************************************/


#ifndef AJOPOYTA_H
#define AJOPOYTA_H

#include <QWidget>
#include <QTimer>


#include "veturi.h"

namespace Ui {
    class Ajopoyta;
}

/** Veturin ajopöytä, jolla vetureita ohjataan*/
class Ajopoyta : public QWidget
{
    Q_OBJECT

public:
    explicit Ajopoyta(QWidget *parent = 0);
    ~Ajopoyta();

signals:
    void keskitysPyynto(QGraphicsItem* item);

public slots:
    void valitseVeturi();
    void valitseVeturi(int veturinNumero);

    void ajopoytaYksi(bool onko);
    void ajopoytaKaksi(bool onko);

    void naytaNopeus(int nopeus);
    void naytaJkvIlmoitus(QPixmap jkvkuva);
    void hataJarru();
    void keskita();
    void tilaMuutos(int ajopoyta, int tavoitenopeus, const QPixmap& kuva = QPixmap());

    void asetaTavoiteNopeus(int nopeus);

    // Päivittää jkv-näytön
    void paivitaJKVnaytto();

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::Ajopoyta *ui;

    Veturi* veturi_;
    QTimer* timer_;


};

#endif // AJOPOYTA_H
