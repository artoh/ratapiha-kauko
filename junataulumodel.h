#ifndef JUNATAULUMODEL_H
#define JUNATAULUMODEL_H

#include <QAbstractTableModel>
#include <QSqlQueryModel>


class JunaTauluModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit JunaTauluModel(QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QString junaNumero(int rivi) const;


    enum {
        JunaNro = 0,
        JunaTyyppi = 1,
        Reitti = 2,
        Lahtoaika = 3,
        Mista = 4,
        MistaRaide = 5,
        Saapumisaika = 6,
        Minne = 7,
        MinneRaide = 8,
        Vaunuja = 9
    };

public slots:
    void paivita();

protected:
    QSqlQueryModel model_;
    
};

#endif // JUNATAULUMODEL_H
