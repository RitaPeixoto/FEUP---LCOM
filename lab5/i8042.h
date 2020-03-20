#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ           1  //section 4.1
#define DELAY_US          20000  //section 4.2


//section 3.1
#define STATUS_REGISTER   0x64   
#define OUT_BUF           0x60
#define ESC_MAKE_CODE     0x01
#define ESC_BREAK_CODE    0x81
#define KBC_CMD_REG       0x64
            

#define TWO_BYTE_LONG     0xE0

//status register 
#define OBF               BIT(0)// output buffer is full (data available for reading)
#define PAR_ERROR         BIT(7)// Parity error- invalid data
#define TO_ERROR          BIT(6)// Timeout error- invalid data 
#define AUX               BIT(5)// Mouse data
#define IBF               BIT(1)// Input buffer full -- don’t write commands or arguments


//KBC commmands --> slide 14
#define READ_CMD_BYTE     0x20 // returns command byte
#define WRITE_CMD_BYTE    0x60 // takes A: command byte
#define KBC_CHECK         0xAA // returns 0x55, if ok; returns 0xFC, if error
#define KBD_I_CHECK       0xAB // returns 0, if ok
#define DISABLE_KBD       0xAD
#define ENABLE_KBD        0xAE



//KBC "Command Byte" --> slide 15
#define KBC_DIS2          BIT(5) // disable mouse
#define KBC_DIS           BIT(4) // disable keyboard interface
#define KBC_INT2          BIT(1) // enable interrupt on OBF, from mouse
#define KBC_INT           BIT(0) // enable interrupt on OBF, from keyboard
//Bits 7, 6, 3, 2 not revelant for this Lab


#endif /* _LCOM_I8042_H */

