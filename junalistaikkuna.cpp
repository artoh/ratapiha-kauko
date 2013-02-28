#include "junalistaikkuna.h"
#include "aikataulumuokkaaja.h"
#include "junataulumodel.h"


#include <QSortFilterProxyModel>
#include <QTableView>
#include <QSqlQuery>
#include <QDockWidget>
#include <QSqlRecord>

#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>
#include <QAction>



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

    proxy_->setFilterRole( Qt::UserRole );

    setCentralWidget(taulu_);

    muokkaaja_ = new AikatauluMuokkaaja(this);
    QDockWidget* muokkaajaDock = new QDockWidget(tr("Aikataulu"));
    muokkaajaDock->setWidget(muokkaaja_);
    addDockWidget(Qt::RightDockWidgetArea, muokkaajaDock);

    connect(muokkaaja_, SIGNAL(junaPaivitetty(QString)), this, SLOT(junaPaivitetty(QString)) );
    connect(taulu_->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(valitseMuokkaukseen())  );

    taulu_->selectRow(0);

    setWindowTitle("Ratapiha: Junat");

    QAction* refreshAktio = new QAction(QIcon(":/r/pic/refresh.png"), tr("Päivitä"), this);
    connect(refreshAktio, SIGNAL(triggered()), model_, SLOT(paivita()) );

    suodatusTyyppiCombo_ = new QComboBox(this);
    suodatusTyyppiCombo_->addItem(tr("Junanumero"), JunaTauluModel::JunaNro);
    suodatusTyyppiCombo_->addItem(tr("Reitti"), JunaTauluModel::Reitti);
    suodatusTyyppiCombo_->addItem(tr("Lähtöasema"), JunaTauluModel::Mista);
    suodatusTyyppiCombo_->addItem(tr("Määräasema"), JunaTauluModel::Minne);

    QLineEdit* suodatusEdit = new QLineEdit(this);
    suodatusEdit->setPlaceholderText(tr("Suodata"));

    QToolBar* tb = addToolBar(tr("Suodatus"));
    tb->addAction(refreshAktio);
    tb->addSeparator();
    tb->addWidget(suodatusTyyppiCombo_);
    tb->addWidget(suodatusEdit);

    connect( suodatusTyyppiCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(suodatusTyypinAsetus(int)) );
    connect( suodatusEdit, SIGNAL(textEdited(QString)), proxy_, SLOT(setFilterRegExp(QString)) );
    connect( suodatusEdit, SIGNAL(textEdited(QString)), this, SLOT(asemaHaku(QString)) );


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

void JunalistaIkkuna::suodatusTyypinAsetus(int indeksi)
{
    proxy_->setFilterKeyColumn( suodatusTyyppiCombo_->itemData(indeksi, Qt::UserRole).toInt() );
    muokkaaja_->lataaReitit();
}

void JunalistaIkkuna::junaPaivitetty(const QString &junatunnus)
{
    model_->paivita();

    // Valitaan kyseinen juna

    if( junatunnus.isEmpty())
        return;

    for(int i=0; i < model_->rowCount(QModelIndex()) ; i++)
        if( model_->junaNumero(i) == junatunnus)
        {
            taulu_->selectRow( proxy_->mapFromSource( model_->index(i,0) ).row()  );

            return;
        }

}

void JunalistaIkkuna::asemaHaku(const QString &asemat)
{
    if( suodatusTyyppiCombo_->itemData( suodatusTyyppiCombo_->currentIndex()) == JunaTauluModel::Mista )
        muokkaaja_->lataaReitit( asemat );
}
