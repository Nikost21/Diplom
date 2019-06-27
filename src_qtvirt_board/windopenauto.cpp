#include "windopenauto.h"
#include "ui_windopenauto.h"

WindOpenAuto::WindOpenAuto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindOpenAuto)
{
    ui->setupUi(this);
}

WindOpenAuto::~WindOpenAuto(){
    delFile(nameModel);
    delFile(nameObjMain);
    delFile(nameObjMod);
    delFile(nameSc_main);
    delete ui;
}

QByteArray WindOpenAuto::err_out(){
    QByteArray tmp;
    QFile errFile(QDir::tempPath() + "/errors_compile");
    if(errFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        tmp = errFile.readAll();
    }
    return tmp;
}

void WindOpenAuto::updatePathName(){
    nameGcc = getPathFile(GccPath,"g++");//Имя компилятора
    nameVBLex = getPathFile(LexPath,"vbllex");//Имя лексического анализатора
    nameModel = getTempNameF(QDir::tempPath() +
                              "/" + "vb_proc",
                              ""); //Генерирование имени процесса моделирования
    nameObjMod = getTempNameF(QDir::tempPath() +
                              "/" + "module",
                              ".o");//Генерирование имени объектного файла объектного кода модуля
    nameObjMain = getTempNameF(QDir::tempPath() +
                               "/" + "main",
                               ".o");
    nameSc_main = getTempNameF(QDir::tempPath() +
                               "/" + "main",
                               ".cpp");
    nameObjMain = getTempNameF(QDir::tempPath() +
                               "/" + "main",
                               ".o");
    nameSc_main = getTempNameF(QDir::tempPath() +
                               "/" + "main",
                               ".cpp");
    nameErr_file = QDir::tempPath() + "/errors_compile";
}

QString WindOpenAuto::getPathFile(QString path, QString name){//Функция для получения пути к файлу
    QString PathFile;
    if(path.isEmpty()) PathFile = name;
    else PathFile = path + "/" + name;
    return PathFile;
}

QString WindOpenAuto::getTempNameF(QString str1, QString str2){//Функция получения имени временного файла
    QString tempname = str1 + str2;
    if(QFileInfo::exists(tempname)){
        unsigned long i = 0;
        do{
            tempname = str1 + QString::number(i) + str2;
            i++;
        }while (QFileInfo::exists(tempname));
    }
    return tempname;
}

void delFile(QString nameObj){//Функция удаления файла с именем nameObj
    if(!nameObj.isEmpty()){
        QFile delfile(nameObj);
        delfile.remove();
    }
}

void WindOpenAuto::on_CompileButton_clicked(){
    ui->textBrowser->clear();
    if(nameNoduleCpp.isEmpty() || nameModuleH.isEmpty()){
        QMessageBox::warning(this,"Ошибка компиляции",
                             "Выбраны не все файлы",QMessageBox::Ok);
        return;
    }
    if(!compileModule()){
        QMessageBox::warning(this,"Ошибка компиляции",
                             "Ошибка при компиляции файла модуля\n" +
                             nameNoduleCpp,QMessageBox::Ok);
        ui->textBrowser->setText(err_out());
        delFile(nameObjMod);
        return;
    }
    if(!genSc_main()){
        QMessageBox::warning(this,"Ошибка генерации",
                             "Ошибка при генерации файла main\n" +
                             nameSc_main,QMessageBox::Ok);
        delFile(nameObjMod);
        return;
    }
    if(!compileMain()){
        QMessageBox::warning(this,"Ошибка компиляции",
                             "Ошибка при компиляции файла main\n" +
                             nameSc_main,QMessageBox::Ok);
        ui->textBrowser->setText(err_out());
        delFile(nameObjMod);
        delFile(nameObjMain);
        return;
    }else{
        delFile(nameSc_main);
    }
    if(!compileModel()){
        QMessageBox::warning(this,"Ошибка компиляции",
                             "Ошибка при компиляции программы для моделирования"
                             ,QMessageBox::Ok);
        ui->textBrowser->setText(err_out());
        delFile(nameObjMain);
        delFile(nameObjMod);
        return;
    }
    delFile(nameObjMain);
    delFile(nameObjMod);
    delFile(nameErr_file);
    emit vbprocCompilied(nameModel);
    hide();
}

bool WindOpenAuto::compileModule(){
    QProcess CompModule;//Процесс компиляциии модуля
    qDebug() << nameGcc;
    QStringList args;
    args << "-g" << "-c";
    if(!(inclVirtBoard.isEmpty()))
        args << ("-I" + inclVirtBoard);
    if(!(inclSystemC.isEmpty()))
        args << ("-I" + inclSystemC);
    args << nameNoduleCpp << "-o" << nameObjMod;
    qDebug() << args;
    CompModule.setStandardErrorFile(nameErr_file);
    CompModule.start(nameGcc,args);
    CompModule.waitForFinished(-1);
    if(CompModule.exitCode()!=0){
        qDebug() << "Error compile:" << nameObjMod;
        return false;
    }
    return true;
}

bool WindOpenAuto::AnalyzeModule(){
    QProcess AnalyzeLex;
    AnalyzeLex.setStandardInputFile(nameModuleH);
    qDebug() << nameModuleH;
    QString outFileName = nameObjMod + ".txt";
    AnalyzeLex.setStandardOutputFile(outFileName);
    qDebug() << nameVBLex;
    AnalyzeLex.start(nameVBLex);
    AnalyzeLex.waitForFinished(-1);
    if(AnalyzeLex.exitCode()!=0){
        qDebug() << "Error analyze:" << nameModuleH;
        delFile(outFileName);
        return false;
    };
    QFile outFile(outFileName);
    outFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString buf = outFile.readAll();
    qDebug() << buf;
    outFile.close();
    delFile(outFileName);
    vblex_data = buf.split('\n');
    return true;
}

void WindOpenAuto::on_OpenH_clicked(){
    nameModuleH = QFileDialog::getOpenFileName(this,"Open .cpp file",
                                                 QDir::homePath(),
                                                 "Module (*.h);;All (*.*)");
    ui->nameLabelH->setText(nameModuleH);
}

void WindOpenAuto::on_OpenCPP_clicked(){
    nameNoduleCpp = QFileDialog::getOpenFileName(this,"Open .h file",
                                                 QDir::homePath(),
                                                 "Module (*.cpp);;All (*.*)");
    ui->nameLabelCPP->setText(nameNoduleCpp);
}

QStringList WindOpenAuto::RecognPorts(){
    QStringList::iterator curWord;
    curWord = vblex_data.begin();
    nameModule = *curWord;
    curWord++;
    QStringList rec_port;
    for(;curWord != vblex_data.end();
         curWord++){
        if(!(*curWord).isEmpty())
            rec_port.append(*curWord);
    }
    return rec_port;
}

bool WindOpenAuto::genSc_main(){
    if(!AnalyzeModule())return false;
    QStringList rec_ports = RecognPorts();
    QString _BEGIN =
        "#include <systemc.h>\n\
         #include \"vb_testbench/vb_testbench.h\"\n\
         #include \"vb_timer/vb_timer.h\"\n\
         #include \"" + nameModuleH + "\"\n\
         int sc_main(int argc, char ** argv){\
            long timestep;\
            if(argc != 1) timestep = atoi(argv[1]);\
            else timestep = 10000;\
            use_vb_timers();\
            sc_signal<bool> led1,led2,led3,led4,led5;\
            sc_signal<bool> btn1,btn2,btn3,btn4,btn5;\
            sc_signal<sc_bv<7>> sevseg1,sevseg2;\
            vb_testbench vb(\"TestB\",timestep);\
            vb.led1(led1); vb.led2(led2);\
            vb.led3(led3);vb.led4(led4);\
            vb.led5(led5);\
            vb.btn1(btn1); vb.btn2(btn2);\
            vb.btn3(btn3); vb.btn4(btn4);\
            vb.btn5(btn5);\
            vb.sevseg1(sevseg1);\
            vb.sevseg2(sevseg2);";
    QString _MIDDLE = nameModule + " modlule(\"MODULE\");";
    QStringList::iterator curPort;
    for(curPort = rec_ports.begin();
        curPort != rec_ports.end();
        curPort++){
        _MIDDLE += ("modlule." + (*curPort) + "(" + (*curPort) + ");\n");
    }
    QString _END = "sc_start();return 0;}";
    QFile mainFile(nameSc_main);
    if(mainFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        mainFile.write(_BEGIN.toUtf8());
        mainFile.write(_MIDDLE.toUtf8());
        mainFile.write(_END.toUtf8());
        qDebug() << _MIDDLE;
    }
    mainFile.close();
    return true;
}

bool WindOpenAuto::compileMain(){
    QProcess CompMain;
    QStringList args;
    args << "-g" << "-c";
    if(!(inclVirtBoard.isEmpty()))
        args << ("-I" + inclVirtBoard);
    if(!(inclSystemC.isEmpty()))
        args << ("-I" + inclSystemC);
    args << nameSc_main << "-o" << nameObjMain;
    qDebug() << args;
    CompMain.setStandardErrorFile(nameErr_file);
    CompMain.start(nameGcc,args);
    CompMain.waitForFinished(-1);
    if(CompMain.exitCode()!=0){
        qDebug() << "Error compile:" << nameObjMain;
        return false;
    }
    return true;
}

bool WindOpenAuto::compileModel(){
    QProcess CompModel;
    QStringList args;
    args << "-g";
    if(!(inclVirtBoard.isEmpty()))
        args << ("-L" + VirtBoardPath);
    if(!(inclSystemC.isEmpty()))
        args << ("-L" + SystemCPath);
    args << nameObjMod << nameObjMain;
    args << "-lvb_testbench" << "-lsystemc" << "-lrt";
    args << "-o" << nameModel;
    qDebug() << args;
    CompModel.setStandardErrorFile(nameErr_file);
    CompModel.start(nameGcc,args);
    CompModel.waitForFinished(-1);
    if(CompModel.exitCode()!=0){
        qDebug() << "Error Compile";
        return false;
    }
    return true;
}

void WindOpenAuto::on_Cancel_clicked()
{
    hide();
}

void WindOpenAuto::setGccPath(QString str){
    GccPath = str;
}

void WindOpenAuto::setLexPath(QString str){
    LexPath = str;
}

void WindOpenAuto::setInclPathSC(QString str){
    inclSystemC = str;
}

void WindOpenAuto::setInclPathVB(QString str){
    inclVirtBoard = str;
}

void WindOpenAuto::setSystemCPath(QString str){
    SystemCPath = str;
}

void WindOpenAuto::setVirtBoardPath(QString str){
    VirtBoardPath = str;
}
