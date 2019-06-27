#include "vb_timer/vb_timer.h"/*В данном заголовочном файле
                               *описан класс vb_vector*/
#include "vb_error/vb_error.h"//В заголовочном файле описана обработка ошибок
#include <vector> //В данном заголовочном файле описан контейнер vector
#include <unistd.h>/*В данном заголовочном файле описаны функции
                    *стандартные функции UNIX*/

unsigned next_timerid = INIT_ID;/*Переменная хранит значения, которое
                                 *будет присвоено новому экземпляру vb_timer*/
struct sigaction sigact;;/*Данная структура нужна чтобы назначить
                          *для сигналов POSIX функцию обработчик*/

sigset_t no_msk;;/*Данная переменная нужна чтобы установить 
                  *пустую маску для сигналов*/

std::vector<vb_timer *> timers_ref;/*В данном контейнере храянтся
                                    *указатели на созданные таймеры*/

/*Функция обрабатывает сигналы от таймеров*/
void vb_timer_handler(int signo,
                      siginfo_t * siginfo,
                      void * ucontext){
    for(int i = 0; i < timers_ref.size(); i++){
        timers_ref[i]->is_called = false;//Сброс значения
        if (timers_ref[i]->timerid == siginfo->si_int){
            /*Выполняется если в структуре siginfo, предназначеной для переди 
            *инофрмации через сигналы, хранится значение равное 
            *id номеру текущего таймера*/
            timers_ref[i]->counter++;
            timers_ref[i]->is_called = true;
        }
    }
}

/* Функция инициализации таймеров */
void use_vb_timers(){
    int ret;//Переменная в которой хранятся значения стандартных функций UNIX
    memset(&sigact, 0,sizeof(sigact));//Сброс sigact
    sigact.sa_sigaction = vb_timer_handler;/*Назначить функцию-обработчик
                                            *сигналов*/
    sigact.sa_flags = SA_SIGINFO;/*Использовать структуру siginfo
                                  *при передачи сигналов POSIX*/
    ret = sigemptyset(&no_msk);/*Создать пустую маску 
                                *блокировки сигналов*/
    if(ret == -1) vb_error(VB_TIMER);/*Обработка ошибок в фукции 
                                              *sigemptyset*/
    sigact.sa_mask = no_msk;/*Назначить маску блокировки сигналов*/
    ret = sigaction( SIGALRM, &sigact, NULL);/*Назначение примененных 
                                              *параметров сигналу SIGALRM*/
    if(ret == -1) vb_error(VB_TIMER);//Аналогично
    timers_ref.reserve(MAX_NUM_TIMERS);/*Установить максимальное 
                                        *количество таймеров*/
}


//Конструктор класса таймеров vb_timer
vb_timer::vb_timer(){
    int ret;
    timerid = next_timerid;//Назначение id номера таймеру
    timers_ref.push_back(this);//Добавление таймера в массив
    //Настройка действтий которые будет выполнятся по истечению таймера
    sigevt.sigev_notify = SIGEV_SIGNAL;//Будет посылаться сигнал
    sigevt.sigev_signo = SIGALRM;//Будет использоваться сигнал SIGALRM
    sigevt.sigev_value.sival_int = timerid;//Передаваемое значение для сигнала
    next_timerid++;//Инкрементация следующего значения id номера
    //Применение значений
    ret = timer_create( CLOCK_MONOTONIC,
                        &sigevt,  &_timer);
    if(ret == -1) vb_error(VB_TIMER);
}

/*Метод устанавливает таймеру интервал в милисекундах*/
void vb_timer::set(int milisec){
    int ret;
    time_t sec = milisec/1000;//Количество целых секунд секунд
    long nsec = (milisec % 1000) * 1000000;//Количество наносекунд
    //Присваивание получившихся значений к структуре itimer
    itimer.it_interval.tv_sec = sec;
    itimer.it_interval.tv_nsec = nsec;
    itimer.it_value.tv_sec = sec;
    itimer.it_value.tv_nsec = nsec;
    //Установка интервала POSIX таймера
    ret = timer_settime( _timer, 0, &itimer, NULL);
    if(ret == -1) vb_error(VB_TIMER);
}

/*Метод останавливает таймер*/
void vb_timer::stop(){
    int ret;
    /*Присваивание всем полям структуры itimer нулевых значений. Так как
     *При этих значениях таймер останавливается*/
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_nsec = 0;
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_nsec = 0;
    ret = timer_settime( _timer, 0, &itimer, NULL);
    if(ret == -1) vb_error(VB_TIMER);
}

/*Метод приостанвливает выполнение программы до истечения таймера.
 *Возвращает количество истечений таймера с предыдущего вызова wait*/
unsigned vb_timer::wait(){
    int ret;
    while(true){
        pause();//Остановка выполнения программы до любого сигнала
        /* Выйти из цикла, если был сигнал был послан этим таймером */
        if(is_called == true) break;
    }
    is_called = false;//Cрос значения
    unsigned ret_coint = counter;
    counter = 0;//Сброс количества истечений
    return ret_coint;
}

//Деструктор класса таймеров vb_timer
vb_timer::~vb_timer(){
    int ret;
    ret = timer_delete(_timer);//Удаление POSIX таймера
    if(ret == -1) vb_error(VB_TIMER);
    std::vector<vb_timer *>::iterator i;
    //Цикл по timers_ref
    for(i = timers_ref.begin();
        i != timers_ref.end();
        i++)
    {
        /*Если id номер таймера равен номеру этого таймера,
         *то удалить его из массива*/
        if((*i)->timerid == timerid){
            timers_ref.erase(i);
            return;
        }
    }
}
