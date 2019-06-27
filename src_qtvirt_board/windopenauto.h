#ifndef WINDOPENAUTO_H
#define WINDOPENAUTO_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>

void delFile(QString);//Удалить файл

namespace Ui {
class WindOpenAuto;
}

class WindOpenAuto : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindOpenAuto(QWidget *parent = 0);
    ~WindOpenAuto();
    void setSystemCPath(QString);
    void setLexPath(QString);
    void setGccPath(QString);
    void setVirtBoardPath(QString);
    void setInclPathVB(QString);
    void setInclPathSC(QString);
    void updatePathName();
private slots:

    void on_CompileButton_clicked();
    void on_OpenH_clicked();
    void on_OpenCPP_clicked();
    void on_Cancel_clicked();

private:
    QString getTempNameF(QString,QString);//Получить имя временного файла
    QString getPathFile(QString,QString);//Получить полный путь к файлу
    bool compileModule();//Откомпилироваить модуль
    bool genSc_main();//Сгенерировать sc_main файл
    bool AnalyzeModule();//Анализировать код модуля
    bool compileMain();//Откомпилировать sc_main
    bool compileModel();//Откомпилировать программу моделирования
    QStringList RecognPorts();//Распознать порты модуля
    QByteArray err_out();
    QStringList vblex_data;
    QString nameGcc;//Имя компилятора
    QString nameVBLex;//Имя лексического анализатора
    QString nameNoduleCpp;//Путь к cpp файлу модуля
    QString nameModuleH;//Путь к h файлу модуля
    QString nameModel;//Имя процесса моделирования
    QString nameObjMod;//Имя объектного файла модуля
    QString nameModule;//Имя модуля
    QString nameErr_file;
    QString nameSc_main;
    QString nameObjMain;
    Ui::WindOpenAuto *ui;
    //Внешние переменные:
    QString SystemCPath;//Имя каталога в котором хранится библиотека SystemC
    QString LexPath;//Путь к лексическому анализатору
    QString GccPath;//Путь к компилятору
    QString VirtBoardPath;//Путь к библиотеке виртуальной отладочной платы
    QString inclVirtBoard;//Заголовочные файлы виртуальной отладочной платы
    QString inclSystemC;//Заголовочные файлы библиотеки SystemC
signals:
    void vbprocCompilied(QString);
};

#endif // WINDOPENAUTO_H
