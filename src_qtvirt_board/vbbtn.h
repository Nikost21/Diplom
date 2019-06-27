#ifndef VBBTN_H
#define VBBTN_H

#include <QObject>
#include <QWidget>
#include <QAbstractButton>
#include <QPainter>

class vbbtn : public QAbstractButton
{
    Q_OBJECT
    QPixmap imgon, imgoff;
    void paintEvent(QPaintEvent *e);
public:
    explicit vbbtn(QWidget * parent = 0);
};

#endif // VBBTN_H
