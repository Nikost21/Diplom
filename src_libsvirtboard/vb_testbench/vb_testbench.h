/* vb_testbench.h
 * Заголовочный файл тестера*/

#ifndef VB_TESTBENCH_H
#define VB_TESTBENCH_H

#include <systemc.h>
#include "vb_timer/vb_timer.h"
#include "vb_io/vb_io.h"
#include <stdlib.h>

#define DEF_STEP_MODEL 100 //Переодичность опросов по умолчанию
#define TM_MS(s) sc_time( (s), SC_MS) //Макрос для перевода int в sc_time

class vb_testbench : public sc_module{
   private:
        vec_cmds cdatain;//Список команд ввода данных
        vec_cmds cdataout;//Список команд управления
        vec_cmds cctlin;//Список команд вывода данных
        vec_cmds cctlout;//Список команд вывода информации о моделировании
        int _model_step;//Переодичность опроса в мс
        bool stopped = true;//Флаг остановки моделирования
    public:
        vb_timer sync_timer;/*vb_timer с помощью которого осуществляется 
                             *синзронизация реального и модельного времени*/
        sc_out<bool> btn1, btn2, btn3, btn4, btn5;//Порты для кнопок
        sc_in<bool> led1, led2, led3, led4, led5;/*Порты для светодиодных
                                                  *индикаторов*/
        sc_in<sc_bv<7>> sevseg1, sevseg2;/*Порты для семисегментных
                                          *индикаторов*/
        virtual void handlcctl(const cmd & cctl);/*Обработка команд
                                                    *управления от пользователя*/
        virtual void func_outcctl(vec_cmds * cctl);/*Составление списка
                                                        *выходных команд для пользователя*/
        virtual void handlcdata(const cmd & cdata);/*Oбработка вхoдных
                                                    *данных от пользователя*/
        virtual void func_outcdata(vec_cmds * cdata);/*Составление списка
                                                   *выходных данных для пользователя*/
        void setStopped(bool);//Функция для остоновки/возобновления моделирования
        void test_thread();/*Процесс SC_THREAD который передает данные от
                            *пользователя в тестируемый модуль*/
        typedef vb_testbench SC_CURRENT_USER_MODULE;//Служебная строка для библиотеки SystemC
        vb_testbench(sc_module_name,int); //Конструктор класса vb_testbench
};
#endif
