
#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"


int (kbd_util_sys_inb)(int port, uint8_t *value);
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
void (kbc_ih)(void);
void (kbd_read)(uint8_t *aux);
void (kbd_write)(int port, uint8_t cmd);
int (enable_interrupts)();




#endif /* _LCOM_KEYBOARD_H */
