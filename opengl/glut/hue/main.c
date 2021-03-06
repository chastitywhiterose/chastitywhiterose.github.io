#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

const int width=720,height=720; /*size of window*/

/*required for saving image frames*/
#include "gl_bbmio.h"

/*required for sin and cos functions*/
#include <math.h>

/*PI is required for controlling angles in radians*/
#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
const int polygon_sides=4;         /*how many sides and/or corners of polygon*/
int polygon_step=1;                /*controlling the step through points of star polygons*/
float polygon_cx=width/2;          /*horizontal left/right x coordinate of center*/
float polygon_cy=height/2;         /*vertical up/down y coordinate of center*/
float polygon_radius=height/2;     /*radius or how big the polygon will be*/
float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

/*declare two arrays for points. One for x and one for y.*/
float polygon_xpoints[0x1000],polygon_ypoints[0x1000];

#include "gl_bbm_polygon.h"

int palette_index=0;

#include "gl_bbm_palette.h"

int fps=60; /*frames per second for animation in the OpenGL window*/
int framelimit=360/polygon_sides; /*how many frames will be written to files*/

/* Function called whenever a key is pressed. */
void keyboard(unsigned char key,int x,int y)
{
 /*printf("key==%X\n",key);*/
 switch(key)
 {
  case 0x1B: /* Exit on ESC/Escape key press */
   glutLeaveMainLoop();
  break;
 }
}

/* Display callback function */
void display()
{
 glClear(GL_COLOR_BUFFER_BIT); /*clear window*/
 gl_bbm_polygon(); /*draw the polygon*/
 glFlush(); /*refresh window*/
}

void gl_bbm_main_loop(int x)
{
 
 /*set the new color of the polygon from indexed palette*/
 glColor3ubv((GLubyte*)&bbm_palette[palette_index]);
 palette_index=(palette_index+1)%bbm_palette_length;
 
 display();
 
 /*optionally save frame as file*/
 if(0)
 {
  gl_bbm_save_frame();
  if(frame==framelimit)
  {
   glutLeaveMainLoop();
   if(0)
   {
    system("gm convert -delay 5 -loop 0 o/*.bmp o/0.gif");
    system("rm o/*.bmp");
   }

  }
 }
 
 polygon_radians+=polygon_radians_step;
 glutTimerFunc(1000/fps,gl_bbm_main_loop,0);
}

int main(int argc, char **argv)
{
 bbm_palette_rainbow(15);
 bbm_palette_view();
 /* return 0;*/ 
 
 printf("Chastity White Rose\nSpinning Polygon\n");

 glutInit(&argc, argv); /*Initialize FreeGLUT*/
 glutInitContextVersion(2,1); /*Using version 2.1 of OpenGL*/
 glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE); /*single buffered RGBA window*/
 glutInitWindowPosition(0,0);
 glutInitWindowSize(width,height);
 glutCreateWindow("Chastity's OpenGL Spinning Polygon using FreeGLUT");

 glOrtho(0.0,width,height,0.0,-1.0,1.0); /*2D orthographic matrix*/

 /*set functions for keyboard and display*/
 glutKeyboardFunc(keyboard);
 glutDisplayFunc(display);
 
 glClearColor(0.0,0.0,0.0,1.0); /*color used to clear the window*/
 glColor3f(1.0,1.0,1.0); /*color of the polygon*/
 
 glutTimerFunc(1000/fps,gl_bbm_main_loop,0);
 glutMainLoop(); /* Run the GLUT event loop */
 
 printf("Good bye world!\n");
 return 0;
}
