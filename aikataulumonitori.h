#ifndef AIKATAULUMONITORI_H
#define AIKATAULUMONITORI_H

#include <QMainWindow>

#include <QDateTime>
#include <QTextBrowser>
#include <QComboBox>
#include <QList>

#include "monitoritietue.h"

class AikatauluMonitori : public QMainWindow
{
    Q_OBJECT
public:
    explicit AikatauluMonitori(QWidget *parent = 0);
    
signals:
    
public slots:
    void paivita(const QDateTime& aika);
    void haeAsema(int indeksi);

protected:
    QTextBrowser* monitori_;
    QString asemanNimi_;
    QComboBox* asemaValintaCombo_;

    QList<MonitoriTietue> tietueet_;
};

#endif // AIKATAULUMONITORI_H
