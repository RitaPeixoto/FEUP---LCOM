#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"



int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (read_command)(uint8_t *aux);
int (write_command)(uint8_t command);
int (write_kbc_byte)(int port, uint8_t cmd);
void (mouse_packet_parse)(struct packet *p, uint8_t pack[3]);
struct mouse_ev* (detect_event_mouse)( struct packet *p);
void (mouse_ih)(void);
bool (mouse_gesture)(struct packet *p, uint8_t x_len, uint8_t tolerance);


enum state_t{
  Initial,
  First_draw,
  Vertex,
  Second_draw
};

