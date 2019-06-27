#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QProcessEnvironment sc_mult_drivers = QProcessEnvironment::systemEnvironment();
    sc_mult_drivers.insert("SC_SIGNAL_WRITE_CHECK","DISABLE");
    vbproc.setProcessEnvironment(sc_mult_drivers);
    _windOpenAuto = new WindOpenAuto(this);
    _settingsWind = new settings_wind(this);
    led1 = new vbled(this); led2 = new vbled(this);
    led3 = new vbled(this); led4 = new vbled(this);
    led5 = new vbled(this);
    btn1 = new vbbtn(this); btn2 = new vbbtn(this);
    btn3 = new vbbtn(this); btn4 = new vbbtn(this);
    btn5 = new vbbtn(this);
    ui->setupUi(this);
    sevseg1 = new SevSegItm();
    ui->sevseg1_view->setScene(sevseg1);
    sevseg2 = new SevSegItm();
    ui->sevseg2_view->setScene(sevseg2);
    lal1.setText("led1");
    ui->ll1->addWidget(led1);
    ui->ll1->addWidget(&lal1);
    lal2.setText("led2");
    ui->ll2->addWidget(led2);
    ui->ll2->addWidget(&lal2);
    lal3.setText("led3");
    ui->ll3->addWidget(led3);
    ui->ll3->addWidget(&lal3);
    lal4.setText("led4");
    ui->ll4->addWidget(led4);
    ui->ll4->addWidget(&lal4);
    lal5.setText("led5");
    ui->ll5->addWidget(led5);
    ui->ll5->addWidget(&lal5);
    lab1.setText("btn1");
    ui->lb1->addWidget(btn1);
    ui->lb1->addWidget(&lab1);
    lab2.setText("btn2");
    ui->lb2->addWidget(btn2);
    ui->lb2->addWidget(&lab2);
    lab3.setText("btn3");
    ui->lb3->addWidget(btn3);
    ui->lb3->addWidget(&lab3);
    lab4.setText("btn4");
    ui->lb4->addWidget(btn4);
    ui->lb4->addWidget(&lab4);
    lab5.setText("btn5");
    ui->lb5->addWidget(btn5);
    ui->lb5->addWidget(&lab5);
    connect(ui->actModAuto, &QAction::triggered,
            this, &MainWindow::OpenModuleAuto);
    connect(_windOpenAuto, &WindOpenAuto::vbprocCompilied,
            this,           &MainWindow::startModel);
    connect(ui->actSettings, &QAction::triggered,
            this,            &MainWindow::OpenSettings);
    connect(_settingsWind, &settings_wind::settingChange,
            this,          &MainWindow::updateSettings);
    updateSettings();
    connect(btn1, &vbbtn::clicked,
            this, &MainWindow::btn1_clicked);
    connect(btn2, &vbbtn::clicked,
            this, &MainWindow::btn2_clicked);
    connect(btn3, &vbbtn::clicked,
            this, &MainWindow::btn3_clicked);
    connect(btn4, &vbbtn::clicked,
            this, &MainWindow::btn4_clicked);
    connect(btn5, &vbbtn::clicked,
            this, &MainWindow::btn5_clicked);
    connect(&vbproc,&QProcess::readyReadStandardOutput,
            this,   &MainWindow::recieveData);
}

MainWindow::~MainWindow()
{
    //_windOpenAuto->hide();
    //delete _windOpenAuto;
    delete led1; delete led2; delete led3;
    delete led4; delete led5;
    delete btn1; delete btn2; delete btn3;
    delete btn4; delete btn5;
    delete sevseg1; delete sevseg2;
    disconnect(&vbproc,&QProcess::readyReadStandardOutput,
               this,   &MainWindow::recieveData);
    vbproc.kill();
    delFile(namevb);
    delete ui;
}

void MainWindow::recieveData(){
    OutPipe = vbproc.readAllStandardOutput();
    str2vec_cmds(&cCtlOut,&cDataOut,OutPipe.data());
    handlOutCmd();
    handlOutData();
    cCtlOut.clear();
    cDataOut.clear();
}

void MainWindow::handlOutCmd(){
    vec_cmds::iterator i;
    QString text;
    for (i = cCtlOut.begin();
         i != cCtlOut.end();
         i++){
        if ((*i).name.compare("time")==0)
            ui->time_label->setText(text.sprintf("%.2f",(*i).value/1000.0));
    }
}

void MainWindow::handlOutData(){
    vec_cmds::iterator i;
    for( i = cDataOut.begin();
         i != cDataOut.end();
         i++){
        if((*i).name.compare("led1")==0){
            if((*i).value == 0) led1->setState(false);
            else led1->setState(true);
        }else if((*i).name.compare("led2") == 0){
            if((*i).value == 0) led2->setState(false);
            else led2->setState(true);
        }else if((*i).name.compare("led3") == 0){
            if((*i).value == 0) led3->setState(false);
            else led3->setState(true);
        }else if((*i).name.compare("led4") == 0){
            if((*i).value == 0) led4->setState(false);
            else led4->setState(true);
        }else if((*i).name.compare("led5") == 0){
            if((*i).value == 0) led5->setState(false);
            else led5->setState(true);
        }else if((*i).name.compare("sevseg1") == 0){
            sevseg1->setState((*i).value);
        }else if((*i).name.compare("sevseg2") == 0){
            sevseg2->setState((*i).value);
        }
    }
}

void MainWindow::handlInCmd(){
    InPipe.clear();
    vec_cmds::iterator i;
    for( i = cCtlIn.begin();
         i != cCtlIn.end();
         i++){
        InPipe.append("#" + QString((*i).name.c_str()) + ":" +
                      QString::number((*i).value) +
                      "\n");
    }
    qDebug() << InPipe;
    cCtlIn.clear();
    vbproc.write(InPipe);
}

void MainWindow::handlInData(){
    InPipe.clear();
    vec_cmds::iterator i;
    for( i = cDataIn.begin();
         i != cDataIn.end();
         i++){
        InPipe.append("-" + QString((*i).name.c_str()) + ":" +
                      QString::number((*i).value) + "\n");
    }
    qDebug() << InPipe;
    cDataIn.clear();
    vbproc.write(InPipe);
}

void MainWindow::OpenModuleAuto(bool i){
    _windOpenAuto->show();
}

void MainWindow::startModel(QString nm){
    if(vbproc.state()!=QProcess::NotRunning){
        vbproc.kill();
        vbproc.waitForFinished(-1);
    }
    namevb = nm;
    vbproc.start(namevb + " 50");
    qDebug() << vbproc.state();
    qDebug() << namevb;
}

void MainWindow::btn1_clicked(){
    cmd tmp;
    tmp.name = "btn1";
    if(btn1->isChecked()){
        tmp.value = 1;
    }else{
        tmp.value = 0;
    }
    cDataIn.push_back(tmp);
    handlInData();
}

void MainWindow::btn2_clicked(){
    cmd tmp;
    tmp.name = "btn2";
    if(btn2->isChecked()){
        tmp.value = 1;
    }else{
        tmp.value = 0;
    }
    cDataIn.push_back(tmp);
    handlInData();
}

void MainWindow::btn3_clicked(){
    cmd tmp;
    tmp.name = "btn3";
    if(btn3->isChecked()){
        tmp.value = 1;
    }else{
        tmp.value = 0;
    }
    cDataIn.push_back(tmp);
    handlInData();
}

void MainWindow::btn4_clicked(){
    cmd tmp;
    tmp.name = "btn4";
    if(btn4->isChecked()){
        tmp.value = 1;
    }else{
        tmp.value = 0;
    }
    cDataIn.push_back(tmp);
    handlInData();
}

void MainWindow::btn5_clicked(){
    cmd tmp;
    tmp.name = "btn5";
    if(btn5->isChecked()){
        tmp.value = 1;
    }else{
        tmp.value = 0;
    }
    cDataIn.push_back(tmp);
    handlInData();
}

void MainWindow::on_start_btn_clicked()
{
    cmd tmp;
    tmp.name = "start";
    tmp.value = 0;
    cCtlIn.push_back(tmp);
    handlInCmd();
}

void MainWindow::on_stop_btn_clicked()
{
    cmd tmp;
    tmp.name = "stop";
    tmp.value = 0;
    cCtlIn.push_back(tmp);
    handlInCmd();
}

void MainWindow::OpenSettings(){
    _settingsWind->show();
}

void MainWindow::updateSettings(){
    _windOpenAuto->setSystemCPath(_settingsWind->getSystemCPath());
    _windOpenAuto->setLexPath(_settingsWind->getLexPath());
    _windOpenAuto->setGccPath(_settingsWind->getGccPath());
    _windOpenAuto->setVirtBoardPath(_settingsWind->getVirtBoardPath());
    _windOpenAuto->setInclPathVB(_settingsWind->getInclPathVB());
    _windOpenAuto->setInclPathSC(_settingsWind->getInclPathSC());
    _windOpenAuto->updatePathName();
}
