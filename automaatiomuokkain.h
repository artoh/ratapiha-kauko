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
    void saantoValittu(bool tyhjenna = false);
    void aikatauluRaide();
    void uusiSaanto();
    void tarkistaEhto();

    void asetaHerateraide(const QString& raiteenpaa);
    void asetaMaaliraide(const QString& raiteenpaa);

    void kytkeAutomaatio(bool paalle);

    void poista();
    void tallenna();

signals:
    void valitseMaaliraide();
    void valitseHerateraide();

protected:
    QString raiteenpaatila(const QString& raiteenpaa);

private:
    Ui::AutomaatioMuokkain *ui;


    QSqlQueryModel* opastimenModel_;
    QSqlQueryModel* heratteenModel_;

    int muokattavaSaanto_;
    QString opastin_;
};

#endif // AUTOMAATIOMUOKKAIN_H
