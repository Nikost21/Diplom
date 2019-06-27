#ifndef VBLED_H
#define VBLED_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class vbled : public QLabel
{
    Q_OBJECT
    QPixmap imgon, imgoff;
    bool state;
public:
    explicit vbled(QWidget * parent = 0);
    void setState(bool);
};

#endif // VBLED_H
