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
#include "gl_bbm_palette.h"

/*frames per second and other animation variables*/
int fps=60,framelimit=1,seconds=0,minutes=0,hours=0;

/*
 Timer variables,used for delay. They are double and not to be confused with integer variables of display times.
*/
double glfwseconds,glfwseconds1;

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
   
  }
 }
}

int text_switch=1;
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
}

void error_callback(int error,const char *description)
{
 fprintf(stderr,"Error: %s\n",description);
}

GLFWwindow* window;
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
 glColor3f(1.0,1.0,1.0); /*color of the polygon*/
 
 /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 /*This allows the XOR operation when drawing the polygon. However it's important to note that in the loop below GL_COLOR_LOGIC_OP is disabled when drawing text but then enabled before drawing the polygon. The text does not draw at all when logical operations are enabled.*/
 glLogicOp(GL_XOR);
 
 framelimit=fps*60*60;

 /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

  if(text_switch)
  {
   glDisable(GL_COLOR_LOGIC_OP);
   glRasterPos2i(fontsize,fontsize);
   sprintf(text,"%d",frame_displayed);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(width-fontsize*5,fontsize);
   sprintf(text,"%d:%02d:%02d",hours,minutes,seconds);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(fontsize,fontsize*3);
   sprintf(text,"%d",polygon_sides);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(fontsize,fontsize*4);
   sprintf(text,"%d",polygon_step);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);


 glRasterPos2i(width*5/16,fontsize);
 ftglRenderFont(font,"OpenGL Polygons",FTGL_RENDER_ALL);

 glRasterPos2i(fontsize*2,height-fontsize);
 ftglRenderFont(font,"An animation by Chastity White Rose!", FTGL_RENDER_ALL);
 
 glEnable(GL_COLOR_LOGIC_OP);
 }
 
 polyfunc();
 
 glFlush();
  
 frame_displayed++; 
 if(frame_displayed%fps==0)
 {
  seconds++;
  if(seconds%60==0)
  {
   minutes++;seconds=0;
   if(minutes%60==0){hours++;minutes=0;}
  }
 }
 
 /*optionally save frame as file*/
 if(1)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
 
  if(frame_displayed%360==0)
 {
  /*
   This section increments the step of the star polygon. If the step would result in an invalid star polygon being drawn, the step is reset to 1 and the number of points incremented. The genius of this is that it will cycle through every possible regular polygon both convex and star!
  */
  polygon_step+=1;
  if(polygon_step>=(polygon_sides/2)+polygon_sides%2)
  {
   polygon_sides+=1;
   polygon_step=1;
  }
 }
 
  
  polygon_radians+=PI/180;
  
  /*check for keypresses or other events*/
  glfwPollEvents();
 
  /*
  glfwseconds=glfwGetTime();
  glfwseconds1=glfwseconds+1.0/fps;
  while(glfwseconds<glfwseconds1)
  {
   glfwseconds=glfwGetTime();
  }
  */
  
 }
 
 

 glfwTerminate();
 ftglDestroyFont(font);
 return 0;
}
