/**
 * @file application_protocol.h
 * @brief Handles incoming application packet and derives them to corresponding port
 *
 * @date 	28/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*  Application Packet
    +--------+--------------+
    | Port   | Data         |
    +--------+--------------+
    | 1 Byte |  0-251 Bytes |
    +--------+--------------+
    Made with https://www.tablesgenerator.com/text_tables# 
*/

#ifndef APPLICATION_PROTOCOL_H
#define APPLICATION_PROTOCOL_H
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define AP_PORT_DEFAULT   	0x00

#define AP_PORT_POS         0
#define AP_DATA_POS         1

/*==================[TYPEDEF]================================================*/
typedef void (*AP_Callback_t)(uint8_t * data);

typedef struct{
  uint8_t port;
  uint8_t * data;
} AP_Packet_t;

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**
 *  @brief Data from lower layer come here. Unique implementation
 *  
 *  @param[in]    raw_pkt       Pointer to AP_Packet_t as raw bytes 
 */
void AP_LowerCallback(uint8_t * raw_pkt);

/**
 *  @brief Application protocol initialization
 */
void AP_Init();

/**
 *  @brief  Creates the AP packet and sends it
 *
 *  @note Each port should create its on send function with the its own port to avoid using other port.
 *        Example: #define ControlProtocol_Send(data,length) AP_SendPacket(CP_PORT,data,length)
 */
void AP_SendPacket(uint8_t port, uint8_t * data, uint8_t length);

/*==================[EXTERNAL CALLBACK DEFINITION]===========================*/
#define AP_CREATE_CALLBACK_TABLE(...)      \
void AP_Port_0_Callback(uint8_t * raw_pay){\
  printf("AP: Default callback triggered. Fisrt Element: %d",*raw_pay);\
}\
const AP_Callback_t AP_CallbackTable[] = {AP_Port_0_Callback, __VA_ARGS__};\
const uint8_t AP_CallbackTableSize = sizeof(AP_CallbackTable)/sizeof(AP_CallbackTable[0]);
 
/*==================[END OF FILE]============================================*/
#endif /* APPLICATION_PROTOCOL_H */