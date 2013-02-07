#ifndef LIIKENNEPAIKKADIALOGI_H
#define LIIKENNEPAIKKADIALOGI_H

#include <QDialog>

namespace Ui {
class LiikennepaikkaDialogi;
}

class LiikennepaikkaDialogi : public QDialog
{
    Q_OBJECT
    
public:
    explicit LiikennepaikkaDialogi(QWidget *parent = 0);
    ~LiikennepaikkaDialogi();
    
private:
    Ui::LiikennepaikkaDialogi *ui;
};

#endif // LIIKENNEPAIKKADIALOGI_H
