/* vb_testbench.cpp
 * исходный код тестера*/

#include "vb_testbench/vb_testbench.h"
#include "vb_error/vb_error.h"
#include "vb_timer/vb_timer.h"
#include "vb_io/vb_io.h"

 /*Процесс SC_THREAD который передает данные от
  *пользователя в тестируемый модуль*/
void vb_testbench::test_thread(){
    sync_timer.set(_model_step);//Установка переодичности опроса
    unsigned time_timer;/*Количество истечений таймера с прошлого 
                         *вызова vb_timer::wait()*/
	while(1){//Бесконечный цикл
        time_timer = sync_timer.wait();//Ожидание истечения таймера
        if(read_cmds(&cctlin,&cdatain)){//Cчитывание данных
            vec_cmds::iterator i;
            for(i = cctlin.begin();//Цикл, в котором обрабатываются
                i != cctlin.end(); //поступающие команды
                i++)
            {
                handlcctl(*i);
                
            }
            if(stopped == false){//Проверка флага остановки моделирования
                vec_cmds::iterator j;
                for(j = cdatain.begin();//Цикл, в котором обрабатываются
                    j != cdatain.end(); //Поступающие данные
                    j++)
                {
                    handlcdata(*j);
                }
            } 
        }
        if(stopped == false)
            wait(TM_MS(time_timer * _model_step));/*продвижение модельного
                                                   *времени */
        func_outcctl(&cctlin);//Составление выходных команд
        if(stopped == false)
            func_outcdata(&cdatain);//Составление выходных данных
        write_cmds(&cctlin,&cdatain);//Вывод данных и команд
    }
}

/*Функция для остоновки/возобновления моделирования*/
void vb_testbench::setStopped(bool flg){
    stopped = flg;
}

/*Oбработка вхoдных данных от пользователя*/
void vb_testbench::handlcdata(const cmd & cdata){
    /* Здесь происходит проверка - какому порту предназначались данные*/
    if(cdata.name.compare("btn1") == 0){
        btn1.write((cdata.value == 0) ? false : true);
    }else
    if(cdata.name.compare("btn2") == 0){
        btn2.write((cdata.value == 0) ? false : true);
    }else
    if(cdata.name.compare("btn3") == 0){
        btn3.write((cdata.value == 0) ? false : true);
    }else
    if(cdata.name.compare("btn4") == 0){
        btn4.write((cdata.value == 0) ? false : true);
    }else
    if(cdata.name.compare("btn5") == 0){
        btn5.write((cdata.value == 0) ? false : true);
    }
}

/*Обработка входных команд от пользователя*/
void vb_testbench::handlcctl(const cmd & cctl){
    /* Здесь происходит проверка - какая именно команда поступила*/
    if(cctl.name.compare("quit") == 0){
        exit(cctl.value);
    }else
    if(cctl.name.compare("start") == 0){
        setStopped(false);
    }else
    if(cctl.name.compare("stop") == 0){
        setStopped(true);
    }
}

/* Функция котороая преобразует тип sc_bv<7> к типу long*/
long bv7_to_long(sc_bv<7> val){
    long retval = 0;//Возвращемое значение устанвливается в 0
    for(char i = 0; i < 7; i++){
        retval |= ((val[6-i]=='1') ? 1 : 0) << i; /*Если i-ый бит sc_bv<7> 
                    установлен в '1' то соответсвующий бит retval
                    устанавливается 1 иначе 0*/
    }
    return retval;
}

/*Составление списка выходных данных для пользователя*/
void vb_testbench::func_outcdata(vec_cmds * cdata){
    cmd tmp;
    tmp.name = "led1";
    tmp.value = (led1.read() == false) ? 0 : 1;
    cdata->push_back(tmp);
    tmp.name = "led2";
    tmp.value = (led2.read() == false) ? 0 : 1;
    cdata->push_back(tmp);
    tmp.name = "led3";
    tmp.value = (led3.read() == false) ? 0 : 1;
    cdata->push_back(tmp);
    tmp.name = "led4";
    tmp.value = (led4.read() == false) ? 0 : 1;
    cdata->push_back(tmp);
    tmp.name = "led5";
    tmp.value = (led5.read() == false) ? 0 : 1;
    cdata->push_back(tmp);
    tmp.name = "sevseg1";
    tmp.value = bv7_to_long(sevseg1.read());
    cdata->push_back(tmp);
    tmp.name = "sevseg2";
    tmp.value = bv7_to_long(sevseg2.read());
    cdata->push_back(tmp);
}

/*Составление списка выходных данных для пользователя*/
void vb_testbench::func_outcctl(vec_cmds * cctl){
    cmd tmp;
    tmp.name = "time";
    tmp.value = (long)(sc_time_stamp().to_seconds() * 1000);
    cctl->push_back(tmp);
}

//Конструктор класса vb_testbench
vb_testbench::vb_testbench(sc_module_name nm,
                           int model_step = DEF_STEP_MODEL):
sc_module(nm){
    _model_step = model_step;//Установка времени моделирования
    SC_THREAD(test_thread);/*Регистрация функции test_thread как SC_THREAD
                            *в ядре моделирования*/
}
