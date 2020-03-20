#include "video_gr.h"

static vbe_mode_info_t vmi_p;
static uint16_t h_resolution, v_resolution;// resolution of x and y in pixels --> resolution of the screen 
static uint8_t bits_ppixel; //bits per pixel
static unsigned int vram_base, vram_size;
static uint8_t *video_mem;/* Process (virtual) address to which VRAM is mapped */
static uint8_t *double_buff ;/*Fpr double buffer*/

static struct minix_mem_range mr;


int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *v){

  mmap_t mem_map;
  lm_alloc(sizeof(vbe_mode_info_t), &mem_map); //alocar memoria


  reg86_t r86;
  memset(&r86, 0, sizeof(r86));	  /* zero the structure */

  r86.ax = 0x4F01;
  r86.cx = mode;
  r86.es = PB2BASE(mem_map.phys);//endereço base
  r86.di = PB2OFF(mem_map.phys);// offset
  r86.intno = 0x10; /* BIOS video services */


  if(sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

  *v = *(vbe_mode_info_t*)mem_map.virt; 


  lm_free(&mem_map);

  return 0;

}




void *(vg_init)(uint16_t mode){


  reg86_t r86;

  if(vbe_get_mode_information(mode, &vmi_p )!=0){//Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address. 
    printf("It was detected an error in vbe_get_mode_information!\n");
    return NULL;
  }
  
  h_resolution = vmi_p.XResolution;
  v_resolution = vmi_p.YResolution;
  bits_ppixel  = vmi_p.BitsPerPixel;

  vram_base = vmi_p.PhysBasePtr;
  vram_size = (h_resolution*v_resolution)*(ceil(bits_ppixel/8.0));//the size of the buffer needs to be the resolution of the screen times the number of bytes per pixel

  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size ;

  /*Section 4.1*/

  if(OK != sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr)){//minix 3's kernel call
  /*
    The first argument specifies the process whose privileges will be affected, and the other two arguments depend on the privileges to change.

  */
    panic("sys_privctl(ADD_MEM) failed!\n");
    return NULL;
  }
  /* MAP MEMORY*/
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  double_buff = (uint8_t *)malloc(vram_size);
  
  if(video_mem == MAP_FAILED){
    panic("Couldn't map video memory");
    return NULL;
  }

    /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86));	  /* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.ax = VBE_CALL; /*VBE call, function 02 -- Set VBE Mode */  
  r86.bx = LINEAR_FRAME_BUFFER | mode;/* set bit 14: linear framebuffer */
  
  /* Make the BIOS call */

  if(sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return NULL;
  }

  return (void *)video_mem;
}

uint16_t gethresolution(){
  return h_resolution;
}

uint16_t getvresolution(){
  return v_resolution;
}

bool (color_of_pixel)(uint16_t x, uint16_t y, uint32_t color){
  int bytes_ppixel = ceil((bits_ppixel)/8.0);
  uint32_t *p_m = (uint32_t*)(double_buff + (x + h_resolution*y)*(bytes_ppixel));
  
  if(*p_m == color){
    return true;
  }

  return false;
}



int (change_pixel_color_32)(uint16_t x, uint16_t y, uint32_t color){
  
  int bytes_ppixel = ceil((bits_ppixel)/8.0);
  uint8_t * p_m = (uint8_t*)double_buff + (x + h_resolution*y)*(bytes_ppixel);
  

  if(x < h_resolution && y < v_resolution){
    for (int i = 0; i < bytes_ppixel; i++){
      //col is the register with the necessary bits in RGB order
      
      //Putting only necessary bytes into the memory
      *(p_m) = color;
      color = (color>>8);
      p_m++;
  
    }
    return 0;
  }

  return 1;
  
}




int (change_pixel_color)(uint16_t x, uint16_t y, uint8_t *color){
  
  int bytes_ppixel = ceil((bits_ppixel)/8.0);
  uint8_t * p_m = (uint8_t*)double_buff + (x + h_resolution*y)*(bytes_ppixel);
  

  if(x < h_resolution && y < v_resolution){
    for (int i = 0; i < bytes_ppixel; i++){
      //col is the register with the necessary bits in RGB order
      
      //Putting only necessary bytes into the memory
      *(p_m) = *color;
      color++;
      p_m++;
  
    }
    return 0;
  }

  return 1;
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  
  for(int i = 0; i < len; i++){
    if(change_pixel_color_32(x+i, y, color)!=0){
      return 1;
    }
  }

  return 0;
}
 
 
 
int (vg_draw_retangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  

  for(int i=0; i<height; i++){
    if(vg_draw_hline(x,y+i,width,color)!=0){
      return 1;
    }
  }
  
 
  return 0;
}


int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img){

  uint32_t *color = (uint32_t*)img.bytes;
  
  for(int aux_y = y; aux_y < y + img.height; aux_y++){

    for(int aux_x = x; aux_x < x + img.width; aux_x++){
      
      if(*color != xpm_transparency_color(XPM_8_8_8_8)){
        change_pixel_color_32(aux_x, aux_y, *color);
      }
      color++;    
  
    }
    
  }
  return 0;
}



int clean_xpm(xpm_image_t img, uint16_t x, uint16_t y){

  uint32_t color = xpm_transparency_color(XPM_8_8_8_8);

  for(int aux_y = y; aux_y < y + img.height; aux_y++){

    for(int aux_x = x; aux_x < x + img.width; aux_x++){

      if(change_pixel_color_32(aux_x, aux_y, color) != 0){
        return 1;
      }

    }

  }
  return 0;
}

void clean_screen(){
  vg_draw_retangle(0, 0, 1152, 864, 0x000000);
}

void(updateBuffer)(void){
  memcpy(video_mem, double_buff,vram_size);
}


