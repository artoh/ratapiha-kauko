#include "liikennepaikkadialogi.h"
#include "ui_liikennepaikkadialogi.h"

#include <QSqlQuery>
#include <QListWidgetItem>

LiikennepaikkaDialogi::LiikennepaikkaDialogi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LiikennepaikkaDialogi)
{
    ui->setupUi(this);

    // Haetaan luettelo
    QSqlQuery kysely("select liikennepaikka, nimi from liikennepaikka order by nimi");
    while( kysely.next())
    {
        QListWidgetItem* item = new QListWidgetItem(kysely.value(1).toString(), ui->liikennepaikkaLista);
        item->setData(Qt::UserRole, kysely.value(0).toString());
    }


}

LiikennepaikkaDialogi::~LiikennepaikkaDialogi()
{
    delete ui;
}
