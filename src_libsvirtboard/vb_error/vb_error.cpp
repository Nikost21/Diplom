#include "vb_error/vb_error.h"

//Cтроковые константы для каждого файла
const std::string msg_vb_none = "error: ";
const std::string msg_vb_timer = "error in vb_timer: ";
const std::string msg_vb_io = "error in vb_io: ";

//Буфер для чтения ошибок стандартных функций
char err_buf[128];

/* Функция обработки ощибок */
void vb_error( vb_part_id part_id){
    std::string where;
    std::string error_msg;
    switch(part_id){
        case VB_TIMER:
            where = msg_vb_timer;
            break;
        case VB_IO:
            where = msg_vb_io;
            break;
    }
    error_msg = strerror_r(errno,err_buf,128);
    error_msg = err_buf;
    std::cerr << where << error_msg << std::endl;
    exit(-1);
}
