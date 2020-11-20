#include "app_main.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>


using namespace stf;
using namespace std;


extern UART_HandleTypeDef huart3;
USART serial(&huart3);

GPIO blue_led(LD2_GPIO_Port, LD2_Pin);
GPIO red_led(LD3_GPIO_Port, LD3_Pin);
GPIO button(USER_Btn_GPIO_Port, USER_Btn_Pin);

//static void util_test(void) {
//    serial << "=============" << stf::endl;
//    serial << cos(degree_to_radian(60.0)) << " " << fast_cos(dtr(60.0)) << stf::endl;
//    serial << fast_inv_sqrt(2.0) << stf::endl;
//    serial << abs(-5.5) << stf::endl;
//    serial << map(50.2, from_range(0, 100), to_range(50, 100)) << stf::endl;
//}

void setup(void) {

//    util_test();

}

void loop0(void) {

    serial << "HelloWorld!" << stf::endl;

}



void loop1(void) {
   blue_led.write(High);
   red_led.write(Low);
   delay(300);
   blue_led.write(Low);
   red_led.write(High);
   delay(300);
}

void stf::exception(const char* str) {
    serial << stf::endl << "*****" << string(str) << stf::endl;
}
