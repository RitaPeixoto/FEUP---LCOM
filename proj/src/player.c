#include "player.h"
 
static Star *stars;
static Star *grey_stars;
static int n_stars;
static Ball *ball;
extern Blank_space *blank_spaces; 
static int k=0;
 
static bool up = false, down = false, left = false, right = false;
static bool succeeded = false, exitt = false;

extern uint32_t timer_counter;

int generate_star_coordinates(uint16_t *x , uint16_t *y){
  int n = 0;
  srand(time(NULL));
  n = rand() % getNumberOfBlankSpaces();
  (*x)= blank_spaces[n].x;
  (*y) = blank_spaces[n].y;

  for(int i = n-1; i < getNumberOfBlankSpaces()-1; i++){
      blank_spaces[i] = blank_spaces[i + 1];
  }
  decrementBlankSpaces();
  return 0;
}
 
int draw_star(int n, uint16_t width, uint16_t height){ //n is the number of stars
  
  stars = NULL;
  grey_stars = NULL;

  k = 0;
  n_stars = n;
  uint8_t *aux;
  Star *star;
  Star *grey_star;
  xpm_image_t img;
  uint16_t x, y;
  uint16_t xcentrado = ceil(gethresolution()/2.0) - (ceil(width/2.0))*15;
	uint16_t ycentrado = ceil(getvresolution()/2.0) - (ceil(height/2.0)+2)*15;

  stars = calloc(n, sizeof(Star));
  grey_stars = calloc(n, sizeof(Star));
  star = malloc(sizeof(Star));
  grey_star = malloc(sizeof(Star));

  for(int i = 0; i < n_stars; i++){
    generate_star_coordinates(&x,&y);  
    star->x = (x);
    star->y = (y);
    star->st_catch = false;
 
    aux = xpm_load(star_xpm, XPM_8_8_8_8, &img);
 
    star->img = img;
 
    stars[i] = *star;

    if(draw_xpm(star->x, star->y, star->img)!=0){
      printf("Something went wrong while trying to draw the star!\n");
      return 1;
    }  
   
    grey_star->x = ((4+i)*15 + xcentrado);
    grey_star->y = (height*15 +2*15 + ycentrado);
    grey_star->st_catch = false;
    aux = xpm_load(star2_xpm, XPM_8_8_8_8, &img);
    grey_star->img = img;
    grey_stars[i] = *grey_star;
 
 
    if(draw_xpm(grey_star->x,grey_star->y,grey_star->img)!=0){
      printf("Something went wrong while trying to draw the star!\n");
      return 1;
    }  
  }
  return 0;
 
}
 
 
 
void create_player(uint16_t width, uint16_t height){
 
  free(ball);
  uint16_t xcentrado = ceil(gethresolution()/2.0) - (ceil(width/2.0))*15;
	uint16_t ycentrado = ceil(getvresolution()/2.0) - (ceil(height/2.0)+2)*15;

  ball = malloc(sizeof(Ball));
 
  if (ball == NULL){
    printf("ERROR: memory could not be allocated to the player\n");
    return;
  }
 
  ball->x = ((2)*15 + xcentrado);
  ball->y = ((height-1)*15 + ycentrado);
  ball->speed = 10;
 
  uint8_t *aux;
  xpm_image_t img;
  aux = xpm_load(b_xpm, XPM_8_8_8_8, &img);
  ball->img = img;

  draw_player();
 
}
 
 
 
int draw_player(){
 
  if(draw_xpm(ball->x, ball->y, ball->img)!=0){
      printf("Something went wrong while trying to draw the ball!\n");
    return 1;
  }  
 
  return 0;
}


//http://stanislavs.org/helppc/make_codes.html?fbclid=IwAR19WbKHLATTu7hePrLYE0mIc3GJSxDAFPfzdmX50byI89lQeREO-kqBOZk-- to know the make/break codes
int check_movement(uint8_t bytes[2], bool make, uint16_t width, uint16_t height){
 
  bool movement = false, possible;
  exitt = false;
  succeeded = false;
 
  if(bytes[0] == 0xE0){
 
    if(make){
 
      switch (bytes[1]){
 
      case 0x48:
      movement = true;
      up = true;
      break;
 
      case 0x4b:
      movement = true;
      left = true;
      break;
 
      case 0x4d:
      movement = true;
      right = true;
      break;
 
      case 0x50:
      movement = true;
      down = true;
      break;
 
      }
    }
    else {
      switch (bytes[1]){
 
      case 0xc8:
      up = false;
      break;
 
      case 0xcb:
      left = false;
      break;
 
      case 0xcd:
      right = false;
      break;
 
      case 0xd0:
      down = false;
      break;
 
      }
    }
  }
 
  uint16_t x = 0, y = 0;
 
  if(movement){
    movement = false;
    if(up && !down && !left && !right){
      x = ball->x;
      y = ball->y - ball->speed;
      movement = true;
    }
    else if (down && !right && !left && !up){
      x = ball->x;
      y = ball->y + ball->speed;
      movement = true;
    }
    else if (right && !left && !down && !up){
      x = ball->x + ball->speed;
      y = ball->y;
      movement = true;
    }
    else if (left && !right && !down && !up) {
      x = ball->x - ball->speed;
      y = ball->y;
      movement = true;
    }

    if(!movement){
      return 0;
    }


    possible = check_colisions(up, down, left, right, x, y, width, height);
 
    check_catch_star(up, down, left, right, x, y, width, height);
 
    if(possible){
      clean_xpm(ball->img, ball->x, ball->y);
      ball->x = x;
      ball->y = y;
      draw_player();
    }
    if(exitt){
      down = false;
      up = false;
      left = false;
      right = false;

      if(succeeded){
        return 2;
      }
      else {
        return 1;
      }
    }
   
  }
 
  return 0;
 
}
 
bool check_colisions(bool up, bool down, bool left, bool right, uint16_t x, uint16_t y, uint16_t width, uint16_t height){
  uint16_t xcentrado = ceil(gethresolution()/2.0) - (ceil(width/2.0))*15;
	uint16_t ycentrado = ceil(getvresolution()/2.0) - (ceil(height/2.0)+2)*15;
 
  if(x<(2*15+xcentrado) || y > ((height-1)*15+ycentrado) || y < (2*15+ycentrado) || (x > ((width-1)* 15 + xcentrado) && y != (2*15 + ycentrado))) return false;
 
  if(x>(width*15 + ball->speed + xcentrado)) return false;
 
  uint32_t color = 0xe6ffff, green = 0x00FF00, black = 0x000000;
  
 
  bool possible, exit_wall;
  succeeded = false;
  exitt = false;
 
 
  if(up){
    possible = !color_of_pixel(x, y, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+11, y, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+6, y, color);
    return possible;    
  }
 
  else if(down){
    possible = !color_of_pixel(x, y+11, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+11, y+11, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+6, y+11, color);
 
    return possible;
  }
 
  else if(left){
    possible = !color_of_pixel(x, y, color);
    if(!possible) return false;
    possible = !color_of_pixel(x, y+11, color);
    if(!possible) return false;
    possible = !color_of_pixel(x, y+6, color);
 
    return possible;    
  }
 
  else if(right){
    possible = !color_of_pixel(x+11, y, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+11, y+11, color);
    if(!possible) return false;
    possible = !color_of_pixel(x+11, y+6, color);
 
    exit_wall = color_of_pixel(x+11, y+6, green);
 
    if(exit_wall){ 
      vg_draw_retangle(width*15+xcentrado, 2*15+ycentrado, 13, 13, black);
      clean_xpm(ball->img, ball->x, ball->y);
      draw_player();
      succeeded = exit_maze();
      exitt = true;
      return true;
    }
 
 
    return possible;    
  }
  return true;
}
 
void catch_star(Star *s){
  s->st_catch = true;
  clean_xpm(s->img, s->x, s->y);
  clean_xpm(ball->img, ball->x, ball->y);
  draw_player();
  xpm_image_t img;
  uint8_t *aux;
  
  aux = xpm_load(star3_xpm, XPM_8_8_8_8, &img);
  grey_stars[k].st_catch = true;
  grey_stars[k].img = img;

  if(draw_xpm(grey_stars[k].x, grey_stars[k].y,img)!=0){
    printf("Something went wrong while trying to draw the grey stars!\n");
    return;
  }
  k++;
  
  return;
}
 
void check_catch_star(bool up, bool down, bool left, bool right, uint16_t x, uint16_t y, uint16_t width, uint8_t height){
 
  for(int i = 0; i < n_stars; i++){
    if(up){
      if((x+13) <= (stars[i].x+15) && x >= (stars[i].x-5)){
        if(y >= stars[i].y && y <= (stars[i].y+13) && !stars[i].st_catch){
          catch_star(&stars[i]);
          return;
        }
       
      }
    }
 
    else if(down){
      if((x+13) <= (stars[i].x+15) && x >= (stars[i].x-5)){
        if((y+13) >= stars[i].y && (y+13) <= (stars[i].y+13) && !stars[i].st_catch){
          catch_star(&stars[i]);
          return;
        }
       
      }
    }
 
    else if (left){
      if((y+13) <= (stars[i].y+15) && y >= (stars[i].y-5)){
        if(x >= stars[i].x && x <= (stars[i].x+12) && !stars[i].st_catch){
          catch_star(&stars[i]);
          return;
        }
       
      }
    }
 
    else if (right){
      if((y+13) <= (stars[i].y+15) && y >= (stars[i].y-5)){
        if((x+13) >= stars[i].x && (x+13) <= (stars[i].x+12) && !stars[i].st_catch){
          catch_star(&stars[i]);
          return;
        }
      }
    }
 
 
  }
 
}

bool exit_maze(){
  uint32_t t=2;
  for(int i=0;i<n_stars;i++){
    if((stars[i].st_catch)==false){
     return false;// o que quer dizer que não venceu o nivel
    }
  }
  if(timer_counter >(t+getLevel())*60000){// t = 2 + nivel atual (minutos)
    //se o tempo for superior ao suposto para este nível
    return false;
  }
  return true;
}

