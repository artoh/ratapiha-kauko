#ifndef JUNALISTAIKKUNA_H
#define JUNALISTAIKKUNA_H

#include <QMainWindow>

class QSortFilterProxyModel;
class JunaTauluModel;
class QTableView;
class AikatauluMuokkaaja;




class JunalistaIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit JunalistaIkkuna(QWidget *parent = 0);
    

signals:
    
public slots:
    void valitseMuokkaukseen();
    
protected:
    QSortFilterProxyModel* proxy_;
    JunaTauluModel* model_;
    QTableView* taulu_;
    AikatauluMuokkaaja* muokkaaja_;

};

#endif // JUNALISTAIKKUNA_H
