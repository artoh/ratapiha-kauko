#include "liikennepaikkadialogi.h"
#include "ui_liikennepaikkadialogi.h"

LiikennepaikkaDialogi::LiikennepaikkaDialogi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LiikennepaikkaDialogi)
{
    ui->setupUi(this);
}

LiikennepaikkaDialogi::~LiikennepaikkaDialogi()
{
    delete ui;
}
