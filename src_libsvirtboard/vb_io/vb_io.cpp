#include <sys/select.h>//В заголовочном файле описана функция select()
#include <unistd.h>/*В данном заголовочном файле описаны функции
                    *стандартные функции UNIX*/
#include <string.h>
#include "vb_io/vb_io.h"
#include <time.h>//В данном заголовочном файле описаны POSIX таймеры
#include <stdio.h>

//Длины для различных буферов
#define LEN_BUF 8192
#define LEN_NAME 1024 

char inbuf[LEN_BUF];//Входной буфер
char outbuf[LEN_BUF];//Выходной буфер

/* Чтенеие данных и команд из stdin */
bool read_cmds(vec_cmds * cctl, vec_cmds * cdata){
    //Очистить массивы команд
    cctl->clear();
    cdata->clear();
    //Инициализация функции select()
    struct timeval zerotime = {0, 0};//Время ожидания у функции select
    int ret;
    fd_set stdin_set;
    FD_ZERO(&stdin_set);
    FD_SET(STDIN_FILENO,&stdin_set);
    /*Функция select ожидает появленияновых данных в потоке
     *в течение некоторого времени timerval*/
    ret = select(STDIN_FILENO + 1,
                 &stdin_set,
                 NULL, NULL, &zerotime);
    if(ret == -1) vb_error(VB_IO);//Ошибка в функции select
    if(ret != 0){
        //Считать данные из stdin если есть данные
        ret = read(STDIN_FILENO,inbuf,LEN_BUF);
        if(ret == -1) vb_error(VB_IO);
        //Преобразовать команды
        str2vec_cmds(cctl,cdata,inbuf);
        return true;
    }
    if((cctl->size() == 0)&&(cdata->size() == 0))
        //Если данных нет
        return false;
}

/*Функция преобразует данные и команды в строку */
void vec_cmds2str(char * str, vec_cmds * cctl, vec_cmds * cdata){
    vec_cmds::iterator i;
    str[0] = '\0';
    char strcmd[LEN_NAME];
    strcmd[0] = '\0';
    //Цикл в котором в строку добавляются команды
    for(i = cctl->begin();
        i != cctl->end();
        i++)
    {
        sprintf(strcmd,"#%s:%li\n",(*i).name.c_str(),(*i).value);
        strcat(str,strcmd);
    }
    vec_cmds::iterator j;
    //Цикл в котором в стоку добавляются данные
    for(j = cdata->begin();
        j != cdata->end();
        j++)
    {
        sprintf(strcmd,"-%s:%li\n",(*j).name.c_str(),(*j).value);
        strncat(str,strcmd,LEN_BUF);
    }
}

/*Функция преобразует строку в данные и команды */
void str2vec_cmds(vec_cmds * cctl, vec_cmds * cdata, char * str){
    cctl->clear();
    cdata->clear();
    //Указатель p1 указывает на еще не прочитанную часть буфера
    char * p1 = str;
    //Указатель p2 нужен для поиска очередной строки в буфере
    char * p2 = p1;
    char namebuf[LEN_NAME];
    long value;
    char type_cmd;
    cmd tmp;
    while(true){
        //Нахождение окончания строки
        p2 = index(p1,'\n');
        if(p2 == NULL) break;
        *p2 = '\0';
        if(sscanf(p1,"%c%[^:]:%li",
                  &type_cmd,namebuf,&value) == 3){
            //Если строка прочитана корректно
            tmp.name = namebuf;
            tmp.value = value;
            switch(type_cmd){
                case '#':
                    //Добавление в список команд
                    cctl->push_back(tmp);
                    break;
                case '-':
                    //Добавление с список данных
                    cdata->push_back(tmp);
                    break;
            }
        }
        //переход к следующей строке
        p1 = p2 + 1;
    }
}

/*Функция преобразует строку в данные и команды */
void write_cmds(vec_cmds * cctl, vec_cmds * cdata){
    vec_cmds2str(outbuf,cctl,cdata);
    std::cout << outbuf << std::endl;
    cctl->clear();
    cdata->clear();
}
