#include "testmod.h"

void testmod::do_display(){
    int stateled = state % 5;
    int statesevseg = state % 10;
    switch(stateled){
        case 0:
            led1.write(true);
            led2.write(false); led3.write(false);
            led4.write(false); led5.write(false);
            break;
        case 1:
            led2.write(true);
            led1.write(false); led3.write(false);
            led4.write(false); led5.write(false);
            break;
        case 2:
            led3.write(true);
            led1.write(false); led2.write(false);
            led4.write(false); led5.write(false);
            break; 
        case 3:
            led4.write(true);
            led1.write(false); led2.write(false);
            led3.write(false); led5.write(false);
            break;
        case 4:
            led5.write(true);
            led1.write(false); led2.write(false);
            led3.write(false); led4.write(false);
            break;
    }
    switch(statesevseg){
        case 0:
            sevseg1.write("1111110"); 
            sevseg2.write("1111110");
            break;
        case 1:
            sevseg1.write("0110000");
            sevseg2.write("0110000");
            break;
        case 2:
            sevseg1.write("1101101");
            sevseg2.write("1101101");
            break;
        case 3:
            sevseg1.write("1111001");
            sevseg2.write("1111001");
            break;
        case 4:
            sevseg1.write("0110011");
            sevseg2.write("0110011");
            break;
        case 5:
            sevseg1.write("1011011");
            sevseg2.write("1011011");
            break;
        case 6:
            sevseg1.write("1011111");
            sevseg2.write("1011111");
            break;
        case 7:
            sevseg1.write("1110000");
            sevseg2.write("1110000");
            break;
        case 8:
            sevseg1.write("1111111");
            sevseg2.write("1111111");
            break;
        case 9:
            sevseg1.write("1110011");
            sevseg2.write("1110011");
            break;
   }
}

void testmod::do_switch1(){
    if(btn1.read() == true){
        state++;
        ev.notify();
    }
}

void testmod::do_switch2(){
    if(btn5.read() == true){
        state--;
        ev.notify();
    }
}
