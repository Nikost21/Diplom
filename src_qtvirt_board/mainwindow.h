#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QIODevice>
#include <QTimer>
#include <windopenauto.h>
#include "vb_io/vb_io.h"
#include "vbled.h"
#include "vbbtn.h"
#include "sevsegitm.h"
#include <QGraphicsScene>
#include "settings_wind.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QList<QByteArray> ListStr;
    vec_cmds cDataIn;
    vec_cmds cCtlIn;
    vec_cmds cDataOut;
    vec_cmds cCtlOut;
    QByteArray InPipe;
    QByteArray OutPipe;
    QProcess vbproc;
    QString namevb;
    Ui::MainWindow *ui;
    WindOpenAuto * _windOpenAuto;
    settings_wind * _settingsWind;
    vbled * led1,* led2,* led3,* led4,* led5;
    vbbtn * btn1,* btn2,* btn3,* btn4,* btn5;
    QLabel lal1, lal2, lal3, lal4, lal5;
    QLabel lab1, lab2, lab3, lab4, lab5;
    SevSegItm * sevseg1;
    SevSegItm * sevseg2;
private slots:
    void updateSettings();
    void OpenModuleAuto(bool);
    void OpenSettings();
    void recieveData();
    void handlInCmd();
    void handlOutCmd();
    void handlInData();
    void handlOutData();
    void startModel(QString);
    void btn1_clicked();
    void btn2_clicked();
    void btn3_clicked();
    void btn4_clicked();
    void btn5_clicked();
    void on_start_btn_clicked();
    void on_stop_btn_clicked();
};

#endif // MAINWINDOW_H
