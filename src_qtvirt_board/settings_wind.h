#ifndef SETTINGS_WIND_H
#define SETTINGS_WIND_H

#include <QDialog>
#include <QFile>

namespace Ui {
class settings_wind;
}

class settings_wind : public QDialog
{
    Q_OBJECT

public:
    explicit settings_wind(QWidget *parent = 0);
    ~settings_wind();
    QFile configFile;
    QString getSystemCPath();
    QString getLexPath();
    QString getGccPath();
    QString getVirtBoardPath();
    QString getInclPathVB();
    QString getInclPathSC();
private slots:
    void on_OkBtn_clicked();
    void readConf();
    void writeConf();
    void on_cancelBtn_clicked();
private:
    QString SystemCPath;//Имя каталога в котором хранится библиотека SystemC
    QString LexPath;//Путь к лексическому анализатору
    QString GccPath;//Путь к компилятору
    QString VirtBoardPath;//Путь к библиотеке виртуальной отладочной платы
    QString inclVirtBoard;//Заголовочные файлы виртуальной отладочной платы
    QString inclSystemC;//Заголовочные файлы библиотеки SystemC
    Ui::settings_wind *ui;
signals:
    void settingChange();
};

#endif // SETTINGS_WIND_H
