#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <GLFW/glfw3.h>

const int width=1280,height=720; /*size of window*/

#include <FTGL/ftgl.h>
FTGLfont *font;
char fontname[256];
char text[256];
int fontsize=48;
int frame_displayed=0;

/*Header for my font functions depending on ftgl variables above.*/
#include "gl_ftgl.h"

/*required for saving image frames. Depends on width and height variables.*/
#include "gl_bbmio.h"

#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
int polygon_sides=3;         /*how many sides and/or corners of polygon*/
int polygon_step=1;
float polygon_cx=width/2;          /*horizontal left/right x coordinate of center*/
float polygon_cy=height/2;         /*vertical up/down y coordinate of center*/
float polygon_radius=height/3;     /*radius or how big the polygon will be*/
float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

/*declare two arrays for points. One for x and one for y.*/
float polygon_xpoints[0x1000],polygon_ypoints[0x1000];

#include "gl_bbm_polygon.h"


/*frames per second and other animation variables*/
int fps=60,framelimit=1,seconds=0,minutes=0,hours=0;

/*
 Timer variables,used for delay. They are double and not to be confused with integer variables of display times.
*/
double glfwseconds,glfwseconds1;

GLFWwindow* window; /*window pointer*/
int text_switch=1;

/*header for different operating modes*/
#include "chastity_tetris.h"

/*
Resource for key names:
https://www.glfw.org/docs/latest/group__keys.html
*/
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
 if(action==GLFW_PRESS || action==GLFW_REPEAT)
 {
  
  switch(key)
  {
   /* Exit on escape key press */
   case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window,GLFW_TRUE);
   break;

   /*keys that move the current block*/
   case GLFW_KEY_UP:
   case GLFW_KEY_W:
    old_block_x=block_x;old_block_y=block_y;
    block_y-=1;
    direction=up;
   break;
   case GLFW_KEY_LEFT:
   case GLFW_KEY_A:
    old_block_x=block_x;old_block_y=block_y;
    block_x-=1;
    direction=left;
   break;
   case GLFW_KEY_DOWN:
   case GLFW_KEY_S:
    old_block_x=block_x;old_block_y=block_y;
    block_y+=1;
    score+=1;
    direction=down;
   break;
   case GLFW_KEY_RIGHT:
   case GLFW_KEY_D:
    old_block_x=block_x;old_block_y=block_y;
    block_x+=1;
    direction=right;
   break;

   
  }
 }
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
 double x,y;
 glfwGetCursorPos(window,&x,&y);

 if(action == GLFW_PRESS)
 {
  if(button==GLFW_MOUSE_BUTTON_LEFT)
  {
   polygon_cx=x;
   polygon_cy=y;
  }
  if(button==GLFW_MOUSE_BUTTON_MIDDLE)
  {
   polygon_radius=x+y;
  }
  if(button==GLFW_MOUSE_BUTTON_RIGHT)
  {
   text_switch^=1;
  }
 }
}

void cursor_position_callback(GLFWwindow *window,double x,double y)
{
 if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS)
 {
  polygon_cx=x;
  polygon_cy=y;
 }
 if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_PRESS)
 {
  polygon_radius=x+y;
 }
}

void error_callback(int error,const char *description)
{
 fprintf(stderr,"Error: %s\n",description);
}





int main(int argc, char **argv)
{
 if(!chastity_ftgl_begin()){return 0;}
 ftglSetFontFaceSize(font,fontsize,0);

 printf("This might be a game someday.\n");
 
 glfwSetErrorCallback(error_callback);
 
 if(!glfwInit()){return 1;} /*Initialize GLFW*/
  


 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2); /*Using version 2.1 of OpenGL*/
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
 glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE); /*single buffered window*/
 window=glfwCreateWindow(width,height,"Chastity's Game Work in Progress",NULL,NULL);
 if(!window){glfwTerminate();return 1;}
 
 glfwMakeContextCurrent(window);
 glfwSetKeyCallback(window,key_callback);
 glfwSetMouseButtonCallback(window,mouse_button_callback);
 glfwSetCursorPosCallback(window, cursor_position_callback);

 glOrtho(0.0,width,height,0.0,-1.0,1.0); /*2D orthographic matrix*/
 glClearColor(0.0,0.0,0.0,1.0); /*color used to clear the window*/
 
 
 printf("The OpenGL version is: %s\n",glGetString(GL_VERSION));

 
 /*now that context,fonts,and colors are loaded, can start a program!*/
 
 mode_tetris();

 

 glfwTerminate();
 ftglDestroyFont(font);
 return 0;
}
