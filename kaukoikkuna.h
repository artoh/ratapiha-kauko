#ifndef KAUKOIKKUNA_H
#define KAUKOIKKUNA_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QAction>

class KaukoIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit KaukoIkkuna(QWidget *parent = 0);
    
signals:
    
public slots:
    void uusiIkkuna();


protected:
    void luoTyoPalkki();
    void luoAktiot();


    QToolBar* hallintaToolBar_;
    QComboBox* nakymaValinta_;

    QAction* uusiIkkunaAktio_;
};

#endif // KAUKOIKKUNA_H
