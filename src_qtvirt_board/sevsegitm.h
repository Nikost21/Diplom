#ifndef SEVSEGITM_H
#define SEVSEGITM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

class SevSegItm : public QGraphicsScene
{
    Q_OBJECT
    QGraphicsRectItem A,B,C,D,E,F,G;
    QBrush Bon, Boff;
    int state;
public:
    explicit SevSegItm(QObject * parent = 0);
    void setState(int);
};

#endif // SEVSEGITM_H
