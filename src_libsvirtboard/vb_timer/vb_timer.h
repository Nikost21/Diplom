/* vb_timer.h
 * Заголовочный файл для таймера*/

#ifndef VB_TIMER_H
#define VB_TIMER_H

#include <time.h>//В данном заголовочном файле описаны POSIX таймеры
#include <signal.h>//В данном заголовчном файле описаны POSIX сигналы

#define MAX_NUM_TIMERS 256 //Максимальное количесво таймеров
#define INIT_ID 1000 //Начальный id номер таймера

//extern struct sigaction sigact;/*Данная структура нужна чтобы назначить
//                                *для сигналов POSIX функцию обработчик*/
//extern sigset_t no_msk;//sigset_t представляет собой набор нескольких сигналов

/* Функция инициализации таймеров */
void use_vb_timers();

/* Класс описывающий таймеры vb_timer */
class vb_timer{
        int timerid;// id номер таймера
        timer_t _timer;//Таймер POSIX
        unsigned counter = 0;//Количество истечений таймера
        struct sigevent sigevt;/*Стректрура с помощью которой настаиваются
                                *сигналы POSIX от таймера;*/
        struct itimerspec itimer;/*Структура предназначена для того, чтобы
                                  *назначить таймеру время истечения*/
        bool is_called = false; /*Если установлено значение true,то значит
                                 *таймер истек*/
    public: 
       friend void vb_timer_handler(int,
                                    siginfo_t *,
                                    void *);/*Функция обработчик сигналов
                                             *имеет доступ частным данным
                                             класса*/
        vb_timer();//Конструктор класса vb_timer
        ~vb_timer();//Деструктор класса vb_timer
        //void init();
        void set(int milisec);/*Метод устанавливает таймеру
                               *интервал в милисекундах*/
        unsigned wait();/*Метод приостанвливает выполнение программы 
                         *до истечения таймера. Возвращает количество
                          истечений таймера с предыдущего вызова wait*/ 
        void stop();/*Метод останавливает таймер*/
};

#endif
