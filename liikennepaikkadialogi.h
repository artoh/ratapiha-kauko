#ifndef LIIKENNEPAIKKADIALOGI_H
#define LIIKENNEPAIKKADIALOGI_H

#include <QWidget>

class QListWidgetItem;

namespace Ui {
class LiikennepaikkaDialogi;
}

class LiikennepaikkaDialogi : public QWidget
{
    Q_OBJECT
    
public:
    explicit LiikennepaikkaDialogi(QWidget *parent = 0);
    ~LiikennepaikkaDialogi();

protected slots:
    void valittuListasta(QListWidgetItem* uusi);
    void lyhenneMuuttunut(const QString& lyhenne);
    void tallenna();
    
private:
    Ui::LiikennepaikkaDialogi *ui;

    bool uusiLiikennepaikka_;
};

#endif // LIIKENNEPAIKKADIALOGI_H
