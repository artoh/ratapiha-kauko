#ifndef JUNALISTAIKKUNA_H
#define JUNALISTAIKKUNA_H

#include <QMainWindow>

class QSortFilterProxyModel;
class JunaTauluModel;
class QTableView;
class AikatauluMuokkaaja;
class QComboBox;



class JunalistaIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit JunalistaIkkuna(QWidget *parent = 0);
    

signals:
    
public slots:
    void valitseMuokkaukseen();
    void suodatusTyypinAsetus(int indeksi);
    void junaPaivitetty(const QString& junatunnus);
    void asemaHaku(const QString& asemat);
    
protected:
    QSortFilterProxyModel* proxy_;
    JunaTauluModel* model_;
    QTableView* taulu_;
    AikatauluMuokkaaja* muokkaaja_;
    QComboBox* suodatusTyyppiCombo_;

};

#endif // JUNALISTAIKKUNA_H
