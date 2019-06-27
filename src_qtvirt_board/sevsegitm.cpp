#include "sevsegitm.h"

SevSegItm::SevSegItm(QObject * parent)
        : QGraphicsScene(parent)
{
    Boff.setStyle(Qt::SolidPattern);
    Bon.setStyle(Qt::SolidPattern);
    Boff.setColor(QColor(100,0,0));
    Bon.setColor(QColor(255,0,0));
    A.setRect(15,0,35,3);
    B.setRect(50,3,3,35);
    C.setRect(50,41,3,35);
    D.setRect(15,76,35,3);
    E.setRect(12,41,3,35);
    F.setRect(12,3,3,35);
    G.setRect(15,38,35,3);
    A.setBrush(Boff);
    B.setBrush(Boff);
    C.setBrush(Boff);
    D.setBrush(Boff);
    E.setBrush(Boff);
    F.setBrush(Boff);
    G.setBrush(Boff);
    addItem(&A);
    addItem(&B);
    addItem(&C);
    addItem(&D);
    addItem(&E);
    addItem(&F);
    addItem(&G);
}

void SevSegItm::setState(int val){
    state = val;
    if((state &(1<<0))!=0)
        A.setBrush(Bon);
    else
        A.setBrush(Boff);
    if((state &(1<<1))!=0)
        B.setBrush(Bon);
    else
        B.setBrush(Boff);
    if((state &(1<<2))!=0)
        C.setBrush(Bon);
    else
        C.setBrush(Boff);
    if((state &(1<<3))!=0)
        D.setBrush(Bon);
    else
        D.setBrush(Boff);
    if((state &(1<<4))!=0)
        E.setBrush(Bon);
    else
        E.setBrush(Boff);
    if((state &(1<<5))!=0)
        F.setBrush(Bon);
    else
        F.setBrush(Boff);
    if((state &(1<<6))!=0)
        G.setBrush(Bon);
    else
        G.setBrush(Boff);
}
