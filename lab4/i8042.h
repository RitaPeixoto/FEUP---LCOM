#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US          20000  //section 4.2
#define MOUSE_IRQ         12 //the mouse controller raises IRQ line 12 -> section 3
#define KBD_IRQ           1  //section 4.1


//section 3.1
#define STATUS_REGISTER   0x64 
#define IN_BUF            0x60  
#define OUT_BUF           0x60
#define ESC_MAKE_CODE     0x01
#define ESC_BREAK_CODE    0x81
#define CMD_REG           0x64
            

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
//Bits 7, 6, 3, 2 not revelant for this Lab OBF

//Mouse Commands
#define MOUSE_LB          BIT(0) // left mouse button pressed
#define MOUSE_RB          BIT(1) // right mouse OBFbutton pressed 
#define MOUSE_MB          BIT(2) // middle mouse button pressed 
#define MOUSE_CHECK_BIT   BIT(3) //the third bit of the first byte of the packet is always set to 1
#define MOUSE_DELTA_X     BIT(4) // mouse x-displacement: rightwards is positive  ---> if 1, x is negative 
#define MOUSE_DELTA_Y     BIT(5) // mouse y-displacement: upwards is positive  ---> if 1, y is negative
#define MOUSE_X_OV        BIT(6) //mouse x-displacement overflow 
#define MOUSE_Y_OV        BIT(7) // mouse y-displacement overflow 


#define ACK               0xFA  // if everything OK
#define NACK              0xFE  // if valid byte
#define ERROR             0xFC  // second consecutive invalid byte


#define WRITE_BYTE        0xD4  //write byte to mouse
//Commands passed as arguments of command 0xD4
#define MOUSE_RESET       0xFF  // mouse reset
#define MOUSE_RESEND      0xFE  // for serial communications errors
#define MOUSE_SETDEFAULTS 0xF6 // set default values
#define MOUSE_DISABLE     0xF5  //in stream mode, shoould be sent before any other command
#define MOUSE_ENABLE      0xF4  //in stream mode only
#define REMOTE_MODE       0xF0  // set remote mode 
#define READ_DATA         0xEB  // read data
#define STREAM_MODE       0xEA  //set stream mode
#endif /* _LCOM_I8042_H */

