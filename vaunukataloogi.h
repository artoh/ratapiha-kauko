/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 26.6.2011
**************************************************************************/


#ifndef VAUNUKATALOOGI_H
#define VAUNUKATALOOGI_H

#include <QListWidget>

/** Vaunuluettelo, joka tukee vaunujen draggaamista ulos,
  ja aikanaan ratapihalle raiteille laiteltavaksi

 */
class VaunuKataloogi : public QListWidget
{
    Q_OBJECT
public:
    explicit VaunuKataloogi(QWidget *parent = 0);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void teeDrag();

    QPoint aloitusPaikka_;
};

#endif // VAUNUKATALOOGI_H
