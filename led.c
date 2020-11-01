#include "led.h"
#include "bl654_dk.h"


void Led_Init(){
	//hardward dependent. For example: gpio_init();		
}

void Led_On(LedId_t led_idx){
    //hardward dependent. For example: gpio_set(pin,HIGH);
}

void Led_Off(LedId_t led_idx){
   //hardward dependent. For example: gpio_set(pin,LOW);
}

void Led_Toggle(LedId_t led_idx){
   //hardward dependent. For example: gpio_toggle(pin);
}