#ifndef VB_IO_H
#define VB_IO_H

#include <string>
#include <vector>
#include "vb_error/vb_error.h"//В заголовочном файле описана обработка ошибок

//Структура для передачи данных и команд
struct cmd{
    std::string name;//Имя команды
    long value;//Операнд
};

//Массив команд
typedef std::vector<cmd> vec_cmds;

/* Чтенеие данных и команд из stdin */
bool read_cmds(vec_cmds * cctl, vec_cmds * cdata);

/* Запись данных и команд в stdout */ 
void write_cmds(vec_cmds * cmds,vec_cmds * cdata);

/*Функция преобразует данные и команды в строку */
void vec_cmds2str(char * str,vec_cmds * cctl, vec_cmds * cdata);

/*Функция преобразует строку в данные и команды */
void str2vec_cmds(vec_cmds * cctl, vec_cmds * vcd, char * str);

#endif
