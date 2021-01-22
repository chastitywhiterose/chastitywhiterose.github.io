#include <stdio.h>
#include <stdlib.h>
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
#include "gl_bbm_polygon_array.h"
#include "gl_bbm_palette.h"

/*frames per second and other animation variables*/
int fps=60,framelimit=1,seconds=0,minutes=0,hours=0;

/*
 Timer variables,used for delay. They are double and not to be confused with integer variables of display times.
*/
double glfwseconds,glfwseconds1;

GLFWwindow* window; /*window pointer*/
int text_switch=1;

/*header for different operating modes*/
#include "glfw_modes.h"

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
   
      /*keys 0 to 7 change the polygon to my favorite colors*/
   case GLFW_KEY_0:
    glColor3f(0.0f, 0.0f, 0.0f);
   break;
   case GLFW_KEY_1:
    glColor3f(0.0f, 0.0f, 1.0f);
   break;
   case GLFW_KEY_2:
    glColor3f(0.0f, 1.0f, 0.0f);
   break;
   case GLFW_KEY_3:
    glColor3f(0.0f, 1.0f, 1.0f);
   break;
   case GLFW_KEY_4:
    glColor3f(1.0f, 0.0f, 0.0f);
   break;
   case GLFW_KEY_5:
    glColor3f(1.0f, 0.0f, 1.0f);
   break;
   case GLFW_KEY_6:
    glColor3f(1.0f, 1.0f, 0.0f);
   break;
   case GLFW_KEY_7:
    glColor3f(1.0f, 1.0f, 1.0f);
   break;

   /*can change whether polygon filled or only lines*/   
   case GLFW_KEY_8:
    polyfunc=gl_bbm_polygon2;
   break;
   case GLFW_KEY_9:
    polyfunc=gl_bbm_polygon3;
   break;
   
   /*can move the polygon anywhere with keyboard*/
   case GLFW_KEY_H:
    polygon_cx-=1;
   break;
   case GLFW_KEY_J:
    polygon_cy+=1;
   break;
   case GLFW_KEY_K:
    polygon_cy-=1;
   break;
   case GLFW_KEY_L:
    polygon_cx+=1;
   break;
   
   /*polygon functions with gl begin/end*/
   case GLFW_KEY_Q:
    polyfunc=gl_bbm_polygon;
   break;
   case GLFW_KEY_W:
    polyfunc=gl_bbm_polygon1;
   break;
   case GLFW_KEY_E:
    polyfunc=gl_bbm_polygon2;
   break;
   case GLFW_KEY_R:
    polyfunc=gl_bbm_polygon3;
   break;
   case GLFW_KEY_T:
    polyfunc=gl_bbm_polygon4;
   break;
   
   case GLFW_KEY_A:
    polyfunc=gl_bbm_polygon_array;
   break;
   case GLFW_KEY_S:
    polyfunc=gl_bbm_polygon_array1;
   break;
   case GLFW_KEY_D:
    polyfunc=gl_bbm_polygon_array2;
   break;
   case GLFW_KEY_F:
    polyfunc=gl_bbm_polygon_array3;
   break;
   case GLFW_KEY_G:
    polyfunc=gl_bbm_polygon_array4;
   break;
   
   case GLFW_KEY_F1:
    mode_f1();
   break;
   case GLFW_KEY_F2:
    mode_f2();
   break;
   case GLFW_KEY_F3:
    mode_f3();
   break;
   
   /*if in checkerboard mode these change rectangle size*/
   case GLFW_KEY_N:
    if(rectsize>1){rectsize-=1;}
   break;
   case GLFW_KEY_M:
    rectsize+=1;
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

 printf("Chastity White Rose\nSpinning Polygon\n");
 
 glfwSetErrorCallback(error_callback);
 
 if(!glfwInit()){return 1;} /*Initialize GLFW*/
  


 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2); /*Using version 2.1 of OpenGL*/
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
 glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE); /*single buffered window*/
 window=glfwCreateWindow(width,height,"Chastity's OpenGL Spinning Polygon using GLFW",NULL,NULL);
 if(!window){glfwTerminate();return 1;}
 
 glfwMakeContextCurrent(window);
 glfwSetKeyCallback(window,key_callback);
 glfwSetMouseButtonCallback(window,mouse_button_callback);
 glfwSetCursorPosCallback(window, cursor_position_callback);

 glOrtho(0.0,width,height,0.0,-1.0,1.0); /*2D orthographic matrix*/
 glClearColor(0.0,0.0,0.0,1.0); /*color used to clear the window*/
 
 printf("The OpenGL version is: %s\n",glGetString(GL_VERSION));

 
 /*now that context,fonts,and colors are loaded, can start a program!*/
 
 mode_f1();

 

 glfwTerminate();
 ftglDestroyFont(font);
 return 0;
}
