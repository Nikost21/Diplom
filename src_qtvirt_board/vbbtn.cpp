#include "vbbtn.h"

vbbtn::vbbtn(QWidget * parent) : QAbstractButton(parent),
    imgon("vbbtnon.png"), imgoff("vbbtnoff.png")
{
    setCheckable(true);
    setChecked(false);
    setMinimumSize(QSize(40,40));
}

void vbbtn::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QPainter painter(this);
    if(isChecked()==false){
        painter.drawPixmap(0,0,40,40,imgoff);
    }else{
        painter.drawPixmap(0,0,40,40,imgon);
    }
}
