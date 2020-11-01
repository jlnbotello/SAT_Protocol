/**
 * @file transport_protocol.h
 * @brief Handles incoming transport packet and derives them to corresponding protocol type
 *
 * @date 	28/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*  Transport Packet
    +--------+--------+--------+----------------+----------+-------------+
    | Header | Id     | Type   | Payload Length | Checksum | Payload     |
    +--------+--------+--------+----------------+----------+-------------+
    | 1 Byte | 1 Byte | 1 Byte | 1 Byte         | 4 Byte   | 0-252 Bytes |
    +--------+--------+--------+----------------+----------+-------------+
    Made with https://www.tablesgenerator.com/text_tables# 
*/

#ifndef TRANSPORT_PROTOCOL_H
#define TRANSPORT_PROTOCOL_H
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define TP_HEADER       	0xAA
#define TP_TYPE_DEFAULT   	0x00
	
#define TP_HEADER_POS   	0
#define TP_ID_POS       	1
#define TP_TYPE_POS     	2
#define TP_LENGTH_POS   	3
#define TP_CHECKSUM_POS     4
#define TP_PAYLOAD_POS      8
	

#define TP_HEADER_LEN		1
#define TP_ID_LEN			1
#define TP_TYPE_LEN			1
#define TP_LENGTH_LEN		1
#define TP_CHECKSUM_LEN 	4
#define TP_PAYLOAD_LEN		252

#define TP_EXTRAS_LEN		TP_HEADER_LEN	+\
							TP_ID_LEN 		+\
							TP_TYPE_LEN 	+\
							TP_LENGTH_LEN	+\
							TP_CHECKSUM_LEN
							
#define TP_PACKET_LEN 		TP_EXTRAS_LEN	+\
							TP_PAYLOAD_LEN
                                        
/*==================[TYPEDEF]================================================*/
typedef void (*TP_Callback_t) (uint8_t * data);

typedef struct{
  uint8_t header;
  uint8_t id;
  uint8_t type; //reserved
  uint8_t payload_len;
  uint32_t checksum;
  uint8_t payload[TP_PAYLOAD_LEN];  
} TP_Packet_t;

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void TP_Init();
void TP_SendPacket(uint8_t type, uint8_t * payload, uint8_t length);
void TP_StateMachine(uint8_t in);

/*==================[EXTERNAL CALLBACK DEFINITION]===========================*/
#define TP_CREATE_CALLBACK_TABLE(...)      \
void TP_DefaultCallback(uint8_t * data) {  \
  printf("TP: Default callback triggered. Fisrt Element: %d",*data);\
} \
const TP_Callback_t TP_CallbackTable[] = {TP_DefaultCallback,__VA_ARGS__}; \
const uint8_t TP_CallbackTableSize = sizeof(TP_CallbackTable)/sizeof(TP_CallbackTable[0]);

/*==================[END OF FILE]============================================*/
#endif //TRANSPORT_PROTOCOL_H