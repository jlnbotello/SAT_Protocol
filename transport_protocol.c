/**
 * @file transport_protocol.c
 * @brief Implementation of transport protocol
 *
 * @date 	28/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
#include "transport_protocol.h"
#include "stdbool.h"
#include "stdio.h"
#include <string.h>

/*==================[MACROS AND DEFINITIONS]=================================*/
#define ON_BUILD_ASSERT(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#define TP_MAX_ID 255

#define ST_WAIT_HEADER 0xA1
#define ST_WAIT_ID 0xA2
#define ST_WAIT_TYPE 0xA3
#define ST_WAIT_PAYLOAD_LEN 0xA4
#define ST_WAIT_CHECKSUM 0xA5
#define ST_WAIT_PAYLOAD 0xA6

/*==================[INTERNAL DATA DECLARATION]==============================*/
static uint8_t state = ST_WAIT_HEADER;
static TP_Packet_t out_pkt = {};
static TP_Packet_t in_pkt = {};
static uint8_t id = TP_MAX_ID; //cycling id
extern const uint8_t TP_CallbackTableSize;
extern const TP_Callback_t TP_CallbackTable[];

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static uint8_t get_next_id();

static uint32_t calculate_checksum();

static bool id_ok();

static bool checksum_ok();

static void analyze_in_pkt();

static void st_wait_header(uint8_t in);

static void st_wait_id(uint8_t in);

static void st_wait_type(uint8_t in);

static void st_wait_payload_len(uint8_t in);

static void st_wait_checksum(uint8_t in);

static void st_wait_payload(uint8_t in);

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static uint8_t get_next_id() {
  if (id < TP_MAX_ID)
    id++;
  else
    id = 0;
  return id;
}

static uint32_t calculate_checksum() {
  //TODO: calculate_checksum
  return 0xF0F0F0F0;
}

static bool id_ok() {
  static uint8_t last_id = 0;
  uint8_t expected_id = 0;

  if (last_id < TP_MAX_ID) // cyclical id
    expected_id = last_id + 1;
  else
    expected_id = 0;

  last_id = in_pkt.id;

  return true; //(expected_id == in_pkt.id)
}

static bool checksum_ok() {
  return true; //TODO
}

static void analyze_in_pkt() {
  if (id_ok())
    if (checksum_ok())
      if (in_pkt.type < TP_CallbackTableSize) 
        TP_CallbackTable[in_pkt.type](in_pkt.payload);      
      //TODO: Error handling
}

static void st_wait_header(uint8_t in) {
  if (in == TP_HEADER) {
    in_pkt.header = in;
    state = ST_WAIT_ID;
  }
}

static void st_wait_id(uint8_t in) {
  in_pkt.id = in;
  state = ST_WAIT_TYPE;
}

static void st_wait_type(uint8_t in) {
  in_pkt.type = in;
  state = ST_WAIT_PAYLOAD_LEN;
}

static void st_wait_payload_len(uint8_t in) {
  in_pkt.payload_len = in;
  state = ST_WAIT_CHECKSUM;
}

static void st_wait_checksum(uint8_t in) {
  static uint8_t offset = 0;
  uint8_t * pkt_as_array = (uint8_t *) &in_pkt;
  pkt_as_array[TP_CHECKSUM_POS + offset] = in; // WARNING: ENDIANESS!
  offset++;
  if (offset == TP_CHECKSUM_LEN) {
    offset = 0;
    state = ST_WAIT_PAYLOAD;
  }
}

static void st_wait_payload(uint8_t in) {
  static uint8_t offset = 0;
  in_pkt.payload[offset] = in;
  offset++;
  if (offset == in_pkt.payload_len) {
    analyze_in_pkt();
    offset = 0;
    state = ST_WAIT_HEADER;
  }
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void TP_Init() {
  ON_BUILD_ASSERT(sizeof(out_pkt) != TP_PACKET_LEN);
  out_pkt.header = TP_HEADER;
  out_pkt.type = TP_TYPE_DEFAULT; // Reserved: Future Use
}

void TP_SendPacket(uint8_t *payload, uint8_t length) {
  out_pkt.id = get_next_id();
  out_pkt.payload_len = length;
  memcpy(out_pkt.payload, payload, length);
  out_pkt.checksum = calculate_checksum();

  //here somethig like...
  //ble_nus_send((uint8_t *)out_pkt.raw,out_pkt.payload_len + TP_EXTRAS_LEN )
}

void TP_StateMachine(uint8_t in) {
  switch (state) {
  case ST_WAIT_HEADER:
    st_wait_header(in);
    break;
  case ST_WAIT_ID:
    st_wait_id(in);
    break;
  case ST_WAIT_TYPE:
    st_wait_type(in);
    break;
  case ST_WAIT_PAYLOAD_LEN:
    st_wait_payload_len(in);
    break;
  case ST_WAIT_CHECKSUM:
    st_wait_checksum(in);
    break;
  case ST_WAIT_PAYLOAD:
    st_wait_payload(in);
    break;
  }
}
/*==================[END OF FILE]============================================*/