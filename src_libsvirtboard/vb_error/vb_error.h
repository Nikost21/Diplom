#ifndef VB_ERROR_H
#define VB_ERROR_H

#include <stdio.h>
#include <errno.h>/*Данный заголовочный файл нужен для обработки 
                   *ошибок стандартных функций*/
#include <string>
#include <string.h>
#include <iostream>

//В каком файле произошла ошибка
enum vb_part_id{
    VB_TIMER, //Ошибка в vb_timer.cpp
    VB_IO //Ошибка в vb_io.cpp
};

/* Функция обработки ошибок */
void vb_error( vb_part_id part_id);

#endif
