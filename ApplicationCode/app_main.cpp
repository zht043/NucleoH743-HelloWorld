#include "app_main.h"
#include "usb_device_vcp.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>


using namespace stf;
using namespace std;


extern UART_HandleTypeDef huart3;
USART serial(&huart3);
USB_VCP usb;

GPIO blue_led(LD2_GPIO_Port, LD2_Pin);
GPIO red_led(LD3_GPIO_Port, LD3_Pin);
GPIO button(USER_Btn_GPIO_Port, USER_Btn_Pin);



void setup(void) {
	usb.init();
	usb.send_packet("Hello World USB\n\r");
    serial << "HelloWorld!" << stf::endl;
}

void loop0(void) {
	std::string line = usb.read_line();
	usb.send_packet(line.append("\n\r"));
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
