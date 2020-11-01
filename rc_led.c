#include "rc_led.h"
#include "led.h"
#include "stdio.h"

void RC_Led(uint8_t * data){

  switch(data[0]){
  case RC_LED_1:   
    Led_Toggle(L1);
  break;
  case RC_LED_2:
    Led_Toggle(L2);
  break;
  case RC_LED_3:
    Led_Toggle(L3);
  break;
  case RC_LED_4:
    Led_Toggle(L4);
  break;
  default:
    //printf("rc_led: default");
  break;
  }  
}