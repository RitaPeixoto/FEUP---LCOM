#include "maze.h"

static Node *nodes; //array of nodes
static Wall *walls; //array of walls
Blank_space *blank_spaces; //array of blank spaces in the maze 
static int number_of_blank_spaces=0; 


//this is an algorithm from https://en.wikipedia.org/wiki/Maze_generation_algorithm based on depth-first search
Wall* getwalls(){
	return walls;
}
Blank_space* getBlankSpace(){
	return blank_spaces;
}
int getNumberOfBlankSpaces(){
	return number_of_blank_spaces;
}

void decrementBlankSpaces(){
	number_of_blank_spaces--;
}

int (init)(uint16_t width, uint16_t height){
	free(nodes);
  
	Node *n;
	
	nodes = calloc( width * height, sizeof(Node)); //allocating memory for the maze
	if (nodes == NULL){
		printf("ERROR: memory could not be allocated to the maze\n");
		return 1;
	}	

	
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){

			n = nodes + i + j * width;

			//note: walls in odd positions

			if ((i*j) % 2) { 
				n->x = i;
				n->y = j;
				n->directions = 15; //assuming that all directions can be explored (4 youngest bits set)
				n->c = ' '; 
			}

			else {
				n->c = '#'; //adding walls between nodes
			} 

		}
	}
	return 0;
}


Node *Link(Node *n, uint16_t width, uint16_t height) {

	//we'll try to connect the Node n to a random neighbor and we'll return the adress of the next node that should be visited

	int x, y;
	char direction;
	Node *dest;
	
	
	if (n == NULL) { //if it's a null pointer, nothing can be done
		return NULL;
	}
	
	
	while (n->directions) // we have to explore all directions
	{
		//Randomly pick one direction
		direction = (1 << (rand()%4)); //we have 4 possible directions so we do a shift left in order to set the respective bit
		

		if (~(n->directions) & direction) { // if this direction has already been explored, we have to try again
			continue;
		} 
		
		n->directions &= ~direction; //mark direction as explored
		
		//note: building maze: top to bottom and left to right

		switch (direction)
		{
			//trying to go right
			case 1:
				if ((n->x + 2) < width) {
					x = (n->x + 2);
					y = n->y;
				}

				else {
					continue;
				}
				 
				break;
			
			//trying to go down
			case 2:
				if ((n->y + 2) < height) {
					x = n->x;
					y = (n->y + 2);
				}

				else {
					continue;
				}
				 
				break;
			
			//trying to go left	
			case 4:
				if ((n->x - 2) >= 0) {
					x = (n->x - 2);
					y = n->y;
				}
				
				else {
					continue;
				}
				 
				break;
			
			//trying to go up
			case 8:
				if ((n->y - 2) >= 0) {
					x = n->x;
					y = (n->y - 2);
				}
				
				else {
					continue;
				}
				 
				break;

		}
		
		//Get destination node into pointer (makes things a tiny bit faster)
		dest = nodes + x + y * width;
		
	
		if (dest->c == ' ') { //ensure that destination node is not a wall!
			
			if (dest->parent != NULL){ //If destination is a linked node already - abort
				continue;
			}

			dest->parent = n; //otherwise, adopt node
			
			nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].c = ' '; //removing walls between nodes
			
			
			return dest; //return address of the child node
		}
	}
	
	return n->parent; //return address of parent node
}

void draw_maze(uint16_t width, uint16_t height)
{
	blank_spaces = NULL;
  blank_spaces = calloc( width * height, sizeof(Blank_space)); //allocating memory for the blank spaces

	int index = 0;
	number_of_blank_spaces = 0;

	walls = calloc( width * height, sizeof(Wall)); //allocating memory 
	int counter = 0;

	Wall *wall;
	wall = malloc(sizeof(Wall));

  uint32_t blue = 0xe6ffff, green = 0x00FF00; //blue = 0x0000FF,
	uint16_t xcentrado = ceil(gethresolution()/2.0) - (ceil(width/2.0))*15;
	uint16_t ycentrado = ceil(getvresolution()/2.0) - (ceil(height/2.0)+2)*15;
	

  for(int i = 1; i < (width+1); i++){
    for(int j = 1; j < (height+1); j++){

			if(i == width && j == 2){ //position of exit wall
				vg_draw_retangle(i*15+xcentrado, j*15+ycentrado, 13, 13, green); //green color to point out the exit
			}
			else if (i == 1 && j == (height-1)){ //position of entry wall
				vg_draw_retangle(i*15+xcentrado, j*15+ycentrado, 13, 13, blue); //blue color to point out the entry
			}

      else if(nodes[(j-1)+(i-1)*height].c != ' '){ //if it is a wall
				walls[counter].x = i*15+xcentrado;
				walls[counter].y = j*15+ycentrado;
				vg_draw_retangle(i*15+xcentrado, j*15+ycentrado, 13, 13,blue); //normal wall
				counter++;
      }
			else if((nodes[(j-1)+(i-1)*height].c == ' ') && !(i ==((2)*15 + xcentrado) && j == ((height-1)*15 + ycentrado))){ //different from the initial coordinates of the player
				blank_spaces[index].x = i*15+xcentrado;
				blank_spaces[index].y = j*15+ycentrado;
				index++;
				number_of_blank_spaces++;
				
			}
    }
  }
}

int create_maze(uint16_t width, uint16_t height){
	free(nodes);
  
  if(init(width, height) != 0){  //setup crucial nodes
    printf("It was detected an error in init(width, height)!\n");
    return 1;
  }

  Node *start, *last;

  //setup start node
	start = nodes + 1 + width;
	start->parent = start;
	last = start;
	
	
	while ((last = Link(last, width, height)) != start); //Connect nodes until start node is reached and can't be left

	draw_maze(width, height); //draw the final maze

  return 0;
}

