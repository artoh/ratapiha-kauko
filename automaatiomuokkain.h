#ifndef AUTOMAATIOMUOKKAIN_H
#define AUTOMAATIOMUOKKAIN_H

#include <QWidget>

class QSqlQueryModel;

namespace Ui {
class AutomaatioMuokkain;
}

class AutomaatioMuokkain : public QWidget
{
    Q_OBJECT
    
public:
    explicit AutomaatioMuokkain(QWidget *parent = 0);
    ~AutomaatioMuokkain();
    
public slots:
    void valitseOpastin(const QString& opastin);
    void saantoValittu();
    void aikatauluRaide();

private:
    Ui::AutomaatioMuokkain *ui;


    QSqlQueryModel* opastimenModel_;
    QSqlQueryModel* heratteenModel_;

    int muokattavaSaanto_;
};

#endif // AUTOMAATIOMUOKKAIN_H
