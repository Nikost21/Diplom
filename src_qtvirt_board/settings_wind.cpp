#include "settings_wind.h"
#include "ui_settings_wind.h"
#include <QDebug>

settings_wind::settings_wind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_wind),
    configFile(".config")
{
    ui->setupUi(this);
    readConf();
    SystemCPath = ui->SystemCPath->text();
    LexPath = ui->LexPath->text();
    GccPath = ui->GccPath->text();
    VirtBoardPath = ui->VirtBoardPath->text();
    inclVirtBoard = ui->inclVirtBoard->text();
}

settings_wind::~settings_wind()
{
    delete ui;
}

void settings_wind::on_OkBtn_clicked()
{
    writeConf();
    SystemCPath = ui->SystemCPath->text();
    LexPath = ui->LexPath->text();
    GccPath = ui->GccPath->text();
    VirtBoardPath = ui->VirtBoardPath->text();
    inclVirtBoard = ui->inclVirtBoard->text();
    inclSystemC = ui->inclSystemC->text();
    emit settingChange();
    hide();
}

void settings_wind::readConf(){
    QString line;
    QStringList data;
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!configFile.atEnd()) {
            line = configFile.readLine();
            line = line.left(line.size()-1);
            data = line.split(':');
            if(data.size() == 2){
                if(data[0].compare("SystemCPath") == 0)
                    ui->SystemCPath->setText(data[1]);
                else if(data[0].compare("VirtBoardPath") == 0)
                    ui->VirtBoardPath->setText(data[1]);
                else if(data[0].compare("inclVirtBoard") == 0)
                    ui->inclVirtBoard->setText(data[1]);
                else if(data[0].compare("inclSystemC") == 0)
                    ui->inclSystemC->setText(data[1]);
                else if(data[0].compare("LexPath") == 0)
                    ui->LexPath->setText(data[1]);
                else if(data[0].compare("GccPath") == 0)
                    ui->GccPath->setText(data[1]);
            }
        }
    }
    configFile.close();
}

void settings_wind::writeConf(){
    if(configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        configFile.write("SystemCPath:" + ui->SystemCPath->text().toUtf8() + "\n");
        configFile.write("VirtBoardPath:" + ui->VirtBoardPath->text().toUtf8() + "\n");
        configFile.write("inclVirtBoard:" + ui->inclVirtBoard->text().toUtf8() + "\n");
        configFile.write("inclSystemC:" + ui->inclSystemC->text().toUtf8() + "\n");
        configFile.write("LexPath:" + ui->LexPath->text().toUtf8() + "\n");
        configFile.write("GccPath:" + ui->GccPath->text().toUtf8() + "\n");
    }
    configFile.close();
}

void settings_wind::on_cancelBtn_clicked()
{
    hide();
}

QString settings_wind::getGccPath(){
    return GccPath;
}

QString settings_wind::getLexPath(){
    return LexPath;
}

QString settings_wind::getSystemCPath(){
    return SystemCPath;
}

QString settings_wind::getVirtBoardPath(){
    return VirtBoardPath;
}

QString settings_wind::getInclPathVB(){
    return inclVirtBoard;
}

QString settings_wind::getInclPathSC(){
    return inclSystemC;
}
