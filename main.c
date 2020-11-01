#include "led.h"
#include "rc_led.h" //remote control led. knows the the led control packet format and control de corresponding led
#include "transport_protocol.h"
#include "application_protocol.h"


/*  Application Packet (they are sent as TP payload)
    +--------+--------------+
    | Port   | Data         |
    +--------+--------------+
    | 1 Byte |  0-251 Bytes |
    +--------+--------------+
    Made with https://www.tablesgenerator.com/text_tables# 
*/


/*  Transport Packet
    +--------+--------+--------+----------------+----------+-------------+
    | Header | Id     | Type   | Payload Length | Checksum | Payload     |
    +--------+--------+--------+----------------+----------+-------------+
    | 1 Byte | 1 Byte | 1 Byte | 1 Byte         | 4 Byte   | 0-252 Bytes |
    +--------+--------+--------+----------------+----------+-------------+
    Made with https://www.tablesgenerator.com/text_tables# 
*/



AP_CREATE_CALLBACK_TABLE(RC_Led,RC_Power); //callback AP->Client Modules. Example: Led, Motor, ADC
TP_CREATE_CALLBACK_TABLE(AP_LowerCallback);//callback TP->AP (unique)

//seudo-code example
int main(void)
{	
	while(1){
	uint8_t in_data = uart_read_byte();
	TP_StateMachine(in_data);
	}
	/*
	to toggle leds uart must receive the following bytes:
	
	TP_HEADER
	CYCLING ID. Must be incremented by one on each sent packet: 0,1,2,3...  ...253,254,255,0,1,2,3..... Checked on receive
	TP_TYPE_DEFAULT
	PAYLOAD_LENGTH
	CHECKSUM
	PAYLOAD
	
	
	*/
}

