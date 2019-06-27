#include <systemc.h>

SC_MODULE(testmod){
    sc_in<bool> btn1,btn5;
    sc_out<bool> led1, led2, led3, led4, led5;
    sc_out<sc_bv<7>> sevseg1, sevseg2;
    unsigned state = 0;
    sc_event ev;
    void do_display();
    void do_switch1();
    void do_switch2();
    SC_CTOR(testmod){
        SC_METHOD(do_switch1);
        sensitive << btn1;
        SC_METHOD(do_switch2);
        sensitive << btn5;
        SC_METHOD(do_display);
        sensitive << ev;
    }
};
