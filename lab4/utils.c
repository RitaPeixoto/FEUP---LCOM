#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  *lsb = (uint8_t)val; // does the casting of val(2 bytes) to an 8 bit integer obtaining the least significant byte of val

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  val = val>>8; //shift right that gets the most significant byte in the least significant byte position
  *msb = (uint8_t)val; //does the casting of val(2 bytes) to an 8 bit integer obtaining the least significant byte of val, that by now represents the most significant byte 

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  
  uint32_t aux;
  if(sys_inb(port, &aux) != 0){
    printf("It was detected an error in sys_inb!\n");
	  return 1;
  }

  *value = aux & 0xFF;

  return 0;
}
