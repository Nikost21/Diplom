#include "vbled.h"

vbled::vbled(QWidget * parent) : QLabel(parent),
    imgoff("vbledoff.png"), imgon("vbledon.png")
{
    state = false;
    setPixmap(imgoff);
}

void vbled::setState(bool flg){
    state = flg;
    if(state)
        setPixmap(imgon);
    else
        setPixmap(imgoff);
}
