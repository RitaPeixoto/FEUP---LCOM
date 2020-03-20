#include "play.h"


//static int n = 0;
static bool two_bytes = false; //if it is two byte long
static uint8_t size, bytes[2];	

void create_game(uint16_t width, uint16_t height){
  clean_screen();
  create_maze(width, height); 
  draw_star(2 + getLevel(), width, height);
  create_player(width, height);
}



int play(uint16_t width, uint16_t height){ 

	bool make = true; //if it is a make or break code 
  int game = 0; 


  if(two_bytes){
    two_bytes = false;
    bytes[1] = getScancode();
    size = 2;
  }

  else if(getScancode() == TWO_BYTE_LONG){
    bytes[0] = getScancode();
    size = 1;
    two_bytes = true;
  }

  if(!two_bytes){
    if(getScancode() & BIT(7))
      make = false;

    game = check_movement(bytes, make, width, height);
  }
    
  if(game == 1){
    //não conseguiu passar o nível
    return 1;
  }
  else if (game == 2 ){
    //passou o nível

    return 2;
  }
  
  return 0;
}


