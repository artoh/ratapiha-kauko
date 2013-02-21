#include "junalistaikkuna.h"
#include "aikataulumuokkaaja.h"
#include "junataulumodel.h"


#include <QSortFilterProxyModel>
#include <QTableView>
#include <QSqlQuery>
#include <QDockWidget>
#include <QSqlRecord>



JunalistaIkkuna::JunalistaIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    model_ = new JunaTauluModel(this);

    proxy_ = new QSortFilterProxyModel(this);
    proxy_->setSourceModel(model_);

    taulu_ = new QTableView;
    taulu_->setModel(proxy_);
    taulu_->setSortingEnabled(true);
    proxy_->sort( JunaTauluModel::Lahtoaika );
    taulu_->setSelectionMode(QAbstractItemView::SingleSelection);
    taulu_->setSelectionBehavior(QAbstractItemView::SelectRows);

    setCentralWidget(taulu_);

    muokkaaja_ = new AikatauluMuokkaaja(this);
    QDockWidget* muokkaajaDock = new QDockWidget(tr("Aikataulu"));
    muokkaajaDock->setWidget(muokkaaja_);
    addDockWidget(Qt::RightDockWidgetArea, muokkaajaDock);

    connect(muokkaaja_, SIGNAL(junaPaivitetty(QString)), model_, SLOT(paivita())  );
    connect(taulu_->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(valitseMuokkaukseen()));

    taulu_->selectRow(0);

    setWindowTitle("Ratapiha: Junat");

}


void JunalistaIkkuna::valitseMuokkaukseen()
{
    QModelIndex index = proxy_->mapToSource( taulu_->currentIndex() );
    if( index.isValid())
    {
        QString junatunnus = model_->junaNumero(index.row());
        muokkaaja_->valittuJuna(junatunnus);
        muokkaaja_->selaaja()->haeJunaAikataulu(junatunnus);
    }
}
