#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <GL/glfw.h>
#include <GL/gl2d.h>

#include "uvcoord_sprites.h"

#define GAME_TITLE "CRobozzle" 

#define SPRITE_SIZE 32

#define BOARD_WIDTH_MAX 16
#define BOARD_HEIGHT_MAX 16

#define SCREEN_WIDTH BOARD_WIDTH_MAX*SPRITE_SIZE
#define SCREEN_HEIGHT BOARD_HEIGHT_MAX*SPRITE_SIZE

#define STRING_MAX 256
#define FUNCTIONS_MAX 10
#define COMMANDS_MAX 10
#define STACK_MAX 256

typedef struct Cell Cell;
typedef struct Player Player;
typedef struct Command Command;
typedef struct Function Function;

struct Cell {
	bool isStar;
	int color;
};

struct Player {
	int x,y,heading;
};

struct Command {
	int code;
	int color;
};

struct Function {
	Command commands[COMMANDS_MAX];
	int numCommands;
};



int sp=STACK_MAX;

bool push(int *stack,int value) {
	if(sp>0) {
		stack[--sp]=value;
		return true;
	} 
	return false;
}

bool pop(int *stack,int *value) {
	if(sp<STACK_MAX) {
		(*value)=stack[sp++];
		return true;
	}
	return false;
}



bool inrect(int i,int j,int x,int y,int w,int h) {
	return i>=x && i<=x+w && j>=y && j<=y+h;
}



int main(int argc, char **argv) {

	glImage image_sprites[SPRITES_NUM_IMAGES];

	GLuint texture_sprites;

	int quit = 0;
	
	int i,j;

	int x,y;
	
	bool isMouseButtonLeftHold=false;
	bool isMouseButtonRightHold=false;
	
	int mouseX,mouseY;

	int boardWidth=0;
	int boardHeight=0;

	bool isStar;
	int color;
	int code;

	Player player;
	Player playerRestore;

	bool isHold=false;
	bool isCode=false;	
	bool isEdit=false;
	bool isPlay=false;
	
	int numStars=0;
	int numStarsRestore=0;

	FILE *fin,*fout;;

	int centerX,centerY;

	char s[STRING_MAX];

	Cell board[BOARD_HEIGHT_MAX][BOARD_WIDTH_MAX];

	Cell boardRestore[BOARD_HEIGHT_MAX][BOARD_WIDTH_MAX];

	Function functions[FUNCTIONS_MAX];
	int numFunctions=0;

	int commands[]={0,1,2,3,4,5,6,7,8,9,13,14,15,16,17,18,19,20,10,11,12,24};
	int numCommands=22;
	int command;
	
	int commandIndex=18;

	int stack[STACK_MAX];

	int currentFunctionIndex=0;
	int currentCommandIndex=0;


	if(argc!=2) {
		printf("Syntax: %s filename\n",argv[0]);
		exit(1);
	}

	srand(time(NULL));

	glScreenInit(SCREEN_WIDTH,SCREEN_HEIGHT);

	glfwSetWindowTitle(GAME_TITLE);

	glfwSwapInterval(1);
	
	texture_sprites = glLoadSpriteset(
			"images/robozzle.tga",
			image_sprites,
			SPRITES_BITMAP_WIDTH,
			SPRITES_BITMAP_HEIGHT,
			SPRITES_NUM_IMAGES,
			sprites_texcoords,
			GL_NEAREST
	); 

 

	for(j=0;j<BOARD_HEIGHT_MAX;j++) {
		for(i=0;i<BOARD_WIDTH_MAX;i++) {
			board[j][i].color=4;
			board[j][i].isStar=false;
		}
	}

	fin=fopen(argv[1],"rt");

	fscanf(fin,"%d %d\n",&boardWidth,&boardHeight);
	fscanf(fin,"%d %d %d\n",&playerRestore.x,&playerRestore.y,&playerRestore.heading);

	fscanf(fin,"%d",&numFunctions);
	for(i=0;i<numFunctions;i++) {
		fscanf(fin,"%d",&functions[i].numCommands);
		for(j=0;j<functions[i].numCommands;j++) {
			functions[i].commands[j].code=-1;
			functions[i].commands[j].color=-1;
		}	
	}

	centerX=(BOARD_WIDTH_MAX-boardWidth)/2;
	centerY=(BOARD_HEIGHT_MAX-boardHeight)/2;

	playerRestore.x+=centerX;
	playerRestore.y+=centerY;

	player=playerRestore;

	for(j=0;j<boardHeight;j++) {
		for(i=0;i<boardWidth;i++) {

			x=i+centerX;
			y=j+centerY;

			fscanf(fin,"%s",s);

			if(strlen(s)!=2) {
				printf("Error: %s: Line %d\n",argv[1],j); 
				exit(1); 
			}

			switch(s[0]) {	
				case 'R': board[y][x].color=0; break;
				case 'G': board[y][x].color=1; break;
				case 'B': board[y][x].color=2; break;
				case 'N': board[y][x].color=3; break;
				case 'X': board[y][x].color=4; break;
				default: 
					printf("Error: %s: Line %d\n",argv[1],j);
					exit(1); 
			}

			switch(s[1]) {
				case 'S': board[y][x].isStar=true; numStars++; break;
				case 'X': board[y][x].isStar=false; break;
				default: 
					printf("Error: %s: Line %d\n",argv[1],j); 
					exit(1); 
			}

		}
	}

	fclose(fin);

	for(j=0;j<BOARD_HEIGHT_MAX;j++) {
		for(i=0;i<BOARD_WIDTH_MAX;i++) {
			boardRestore[j][i]=board[j][i];
		}
	}
	numStarsRestore=numStars;


	while(!quit) {

		glfwGetMousePos(&mouseX,&mouseY);

		if(!isHold) {

			if(isCode) {
	
			} else {
		
				if(isEdit) {

					if(glfwGetKey('W')==GLFW_PRESS) {
						player.y--;
						if(player.y<0) player.y=BOARD_HEIGHT_MAX-1;
						isHold=true;
					}

					if(glfwGetKey('S')==GLFW_PRESS) {
						player.y++;
						if(player.y>BOARD_HEIGHT_MAX-1) player.y=0;
						isHold=true;
					}

					if(glfwGetKey('A')==GLFW_PRESS) {
						player.x--;
						if(player.x<0) player.x=BOARD_WIDTH_MAX-1;
						isHold=true;
					}

					if(glfwGetKey('D')==GLFW_PRESS) {
						player.x++;
						if(player.x>BOARD_WIDTH_MAX-1) player.x=0;
						isHold=true;
					}
		

					if(glfwGetKey('0')==GLFW_PRESS) {
						for(j=0;j<BOARD_HEIGHT_MAX;j++) {
							for(i=0;i<BOARD_WIDTH_MAX;i++) {
								board[j][i].color=4;
								board[j][i].isStar=false;
							}
						}
						isHold=true;
					}

					if(glfwGetKey('1')==GLFW_PRESS) {
						board[player.y][player.x].color=0;
						isHold=true;
					}

					if(glfwGetKey('2')==GLFW_PRESS) {
						board[player.y][player.x].color=1;
						isHold=true;
					}

					if(glfwGetKey('3')==GLFW_PRESS) {
						board[player.y][player.x].color=2;
						isHold=true;
					}

					if(glfwGetKey('4')==GLFW_PRESS) {
						board[player.y][player.x].color=3;
						isHold=true;
					}

					if(glfwGetKey('5')==GLFW_PRESS) {
						board[player.y][player.x].color=4;
						
						if(board[player.y][player.x].isStar) {
							numStars--;
							board[player.y][player.x].isStar=false;
						}
						
						isHold=true;
					}

					if(glfwGetKey('6')==GLFW_PRESS) {
						if(board[player.y][player.x].color!=4) {
							bool isStar=!board[player.y][player.x].isStar;
							board[player.y][player.x].isStar=isStar;
							if(isStar) {
								numStars++;
							} else {
								numStars--;
							}
							
						}
						isHold=true;
					}
					
				} else {

					if(isPlay) {

					} else {

						if(glfwGetKey('W')==GLFW_PRESS) {
							switch(player.heading) {
					 			case 0: player.x--; break;
					 			case 1: player.y--; break;
					 			case 2: player.x++; break;
					 			case 3: player.y++; break;
					 		}

							if(player.x<0) player.x=0;
							if(player.y<0) player.y=0;
							if(player.x>BOARD_WIDTH_MAX-1) player.x=BOARD_WIDTH_MAX-1;
							if(player.y>BOARD_HEIGHT_MAX-1) player.y=BOARD_HEIGHT_MAX-1;

					 		isHold=true;
			   		} 

						if(glfwGetKey('A')==GLFW_PRESS) { 
							player.heading=player.heading-1<0?3:player.heading-1;
							isHold=true;
						} 

						if(glfwGetKey('D')==GLFW_PRESS) {
							player.heading=player.heading+1>3?0:player.heading+1;
							isHold=true;
						}
					}

				}
				
				if(glfwGetKey('7')==GLFW_PRESS) {
					isEdit=!isEdit;
					isHold=true;
				}

				if(glfwGetKey('9')==GLFW_PRESS) {
					for(j=0;j<BOARD_HEIGHT_MAX;j++) {
						for(i=0;i<BOARD_WIDTH_MAX;i++) {
							board[j][i]=boardRestore[j][i];
						}
					}
					
					player.x=playerRestore.x;
					player.y=playerRestore.y;
					player.heading=playerRestore.heading;

					numStars=numStarsRestore;

					isHold=true;
				}

			}

			if(glfwGetKey('8')==GLFW_PRESS) {
				isCode=!isCode;
				isHold=true;
			}

			if(glfwGetKey(GLFW_KEY_ENTER)==GLFW_PRESS) {
				isEdit=false;
				isPlay=!isPlay;
				isHold=true;

				if(isPlay) {
					sp=STACK_MAX;
					currentFunctionIndex=0;
					currentCommandIndex=0;
				}
			}

			if(glfwGetKey('P')==GLFW_PRESS) {

				fout=fopen("puzzles/new","wt");

				fprintf(fout,"%d %d\n",BOARD_WIDTH_MAX,BOARD_HEIGHT_MAX);
				fprintf(fout,"%d %d %d\n",player.x,player.y,player.heading);

				fprintf(fout,"%d ",numFunctions);
				for(j=0;j<numFunctions;j++) {
					fprintf(fout,"%d ",functions[j].numCommands);					
				}
				fprintf(fout,"\n");

				for(j=0;j<BOARD_HEIGHT_MAX;j++) {
					for(i=0;i<BOARD_WIDTH_MAX;i++) {
						isStar=board[j][i].isStar;
						color=board[j][i].color;
						switch(color) {
							case 0: s[0]='R'; break;
							case 1: s[0]='G'; break;
							case 2: s[0]='B'; break;
							case 3: s[0]='N'; break;
							case 4: s[0]='X'; break;
						}
						if(isStar) s[1]='S'; else s[1]='X';
						s[2]='\0';
						fprintf(fout,"%s ",s);
					}
					fprintf(fout,"\n");
				}

				fclose(fout);

				isHold=true;
			}

		} else {
		
			if(	glfwGetKey('W')==GLFW_RELEASE &&
					glfwGetKey('S')==GLFW_RELEASE &&
					glfwGetKey('A')==GLFW_RELEASE &&
					glfwGetKey('D')==GLFW_RELEASE &&
					glfwGetKey('0')==GLFW_RELEASE &&
					glfwGetKey('1')==GLFW_RELEASE &&
					glfwGetKey('2')==GLFW_RELEASE &&
					glfwGetKey('3')==GLFW_RELEASE &&
					glfwGetKey('4')==GLFW_RELEASE &&
					glfwGetKey('5')==GLFW_RELEASE &&
					glfwGetKey('6')==GLFW_RELEASE &&
					glfwGetKey('7')==GLFW_RELEASE &&
					glfwGetKey('8')==GLFW_RELEASE &&
					glfwGetKey('9')==GLFW_RELEASE &&
					glfwGetKey('P')==GLFW_RELEASE &&
					glfwGetKey(GLFW_KEY_ENTER)==GLFW_RELEASE) {
				isHold=false;
			}

		}
		
		glClearScreen();

		if(isPlay) {

			code=functions[currentFunctionIndex].commands[currentCommandIndex].code;
			color=functions[currentFunctionIndex].commands[currentCommandIndex].color;

			if(board[player.y][player.x].color==4) isPlay=false;

			if(isPlay) {						

				if(code!=-1 && color!=-1) {

					if(color==board[player.y][player.x].color || color==3) {

						if(code>=0 && code<=9) {

							int v1=push(stack,currentFunctionIndex);
							int v2=push(stack,currentCommandIndex+1);
							if(!v1 || !v2) {
								isPlay=false;
							}
							
							currentFunctionIndex=code;
							currentCommandIndex=-1;
							
						} else if(code==10) {
						
							switch(player.heading) {
					 			case 0: player.x--; break;
					 			case 1: player.y--; break;
					 			case 2: player.x++; break;
					 			case 3: player.y++; break;										
							}

							if(player.x<0) { player.x=0; isPlay=false; }
							if(player.y<0) { player.y=0; isPlay=false; }
							if(player.x>BOARD_WIDTH_MAX-1) { player.x=BOARD_WIDTH_MAX-1; isPlay=false; }
							if(player.y>BOARD_HEIGHT_MAX-1) { player.y=BOARD_HEIGHT_MAX-1; isPlay=false; }


						} else if(code==11) {
							player.heading=player.heading-1<0?3:player.heading-1;
						} else if(code==12) {
							player.heading=player.heading+1>3?0:player.heading+1;
						} else if(code>=13 && code<=16) {
							board[player.y][player.x].color=code-13;
						}
						
					}

				}

				if(isPlay) {
					currentCommandIndex++;
					if(currentCommandIndex>functions[currentFunctionIndex].numCommands-1) {
						int v1=pop(stack,&currentCommandIndex);
						int v2=pop(stack,&currentFunctionIndex);
						if(!v1 || !v2) {
							isPlay=false;
						}
					}
					glfwSleep(0.0625);
				}

			}

		}

		if(isCode) {

			for(j=0;j<numFunctions;j++) {
				x=0;
				y=j*32;
				glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[j]);
				for(i=0;i<functions[j].numCommands;i++) {
					x=(i+1)*32;

					code=functions[j].commands[i].code;
					color=functions[j].commands[i].color;

					glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[21]);

					if(color!=-1) {
						glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[color+17]);
					}			
					
					if(code!=-1) {
						glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[code]);
					}				
					
					glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[22]);

					if(	glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS &&
							inrect(mouseX,mouseY,x,y,32,32)) {

						command=commands[commandIndex];
						if(command>=17 && command<=20) {
							functions[j].commands[i].color=command-17;
						} else if(command==24) {
							functions[j].commands[i].code=-1;
							functions[j].commands[i].color=-1;							
						} else {
							functions[j].commands[i].code=command;
							if(functions[j].commands[i].color==-1) {
								functions[j].commands[i].color=3;
							}
						}

					}

					
				}
			}

			for(i=0;i<numFunctions;i++) {
				x=(i%10+1)*32;
				y=(i/10+j+1)*32;
				glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[commands[i]]);

				if(	glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS &&
						inrect(mouseX,mouseY,x,y,32,32)) {
					commandIndex=i;	
				}
				
				if(i==commandIndex) {
					glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[25]);					
				}								
			}

			for(i=0;i<numCommands-10;i++) {
				x=(i%4+1)*32;
				y=(i/4+j+2)*32;
				glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[commands[i+10]]);				

				if(	glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS && 
						inrect(mouseX,mouseY,x,y,32,32)) {
					commandIndex=i+10;	
				}

				if(i+10==commandIndex) {
					glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[25]);					
				}								
			}

		} else {
	
			if(!isEdit) {		
				if(board[player.y][player.x].isStar) {
					board[player.y][player.x].isStar=false;
					numStars--;
					if(numStars<=0) {
						numStars=0;
						isPlay=false;
					}
				}
			}

			for(j=0;j<BOARD_HEIGHT_MAX;j++) {
				for(i=0;i<BOARD_WIDTH_MAX;i++) {
					x=i*32;
					y=j*32;
					glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[board[j][i].color+17]);
					if(board[j][i].isStar) { 
						glSprite(x,y,GL2D_FLIP_NONE,&image_sprites[23]);
					}
				}
			}

			if(isEdit) {
				glSprite(player.x*32,player.y*32,GL2D_FLIP_NONE,&image_sprites[25]);
			} else {
				glSprite(player.x*32,player.y*32,GL2D_FLIP_NONE,&image_sprites[player.heading+26]);
			}

		}


		float TimeStart=glfwGetTime();
		while((glfwGetTime()-TimeStart)<(1.0/60.0)){};
	
		glfwSwapBuffers();
	
		quit=glfwGetKey(GLFW_KEY_ESC) | !glfwGetWindowParam(GLFW_OPENED);
	}
	
	glDeleteTextures(1,&texture_sprites);

	glfwTerminate();

	return 0;
}

