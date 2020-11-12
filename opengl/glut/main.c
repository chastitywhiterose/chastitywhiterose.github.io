#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

const int width=720,height=720; /*size of window*/

#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
const int polygon_sides=3;         /*how many sides and/or corners of polygon*/
float polygon_cx=width/2;          /*horizontal left/right x coordinate of center*/
float polygon_cy=height/2;         /*vertical up/down y coordinate of center*/
float polygon_radius=height/2;     /*radius or how big the polygon will be*/
float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

int fps=60; /*frames per second for animation in the OpenGL window*/
int framelimit=360/polygon_sides; /*how many frames will be written to files*/

/*declare two arrays for points. One for x and one for y.*/
float polygon_xpoints[0x1000],polygon_ypoints[0x1000];

void make_polygon_points()
{
 double angle,x,y;
 int i=0;
 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians;
  x=polygon_cx+sin(angle)*polygon_radius;
  y=polygon_cy-cos(angle)*polygon_radius;
  polygon_xpoints[i]=x;
  polygon_ypoints[i]=y;
  i++;
 }
}

void polygon()
{
 int i;
 make_polygon_points();
 glBegin(GL_POLYGON);
 i=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  i+=1;
 }
 glEnd();
}


#define pixels_array_size 0x1000000
GLubyte pixels_array[pixels_array_size];

void fputint(unsigned long int i,FILE *fp,int count)
{
 while(count>0)
 {
  fputc(i,fp);
  i>>=8;
  count--;
 }
}

/*
This function is my own custom routine for making monochrome bitmap files. This format has existed in all versions of Windows and almost all software supports loading and viewing it. This makes it ideal when I need to save image files in my programs and only need the colors black and white.
*/
void gl_bbm_SaveBMP1(GLubyte *pointer,int width,int height,const char* filename)
{
 int x,y,data,bpp=1,bit,red,green,blue,gray;
 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 else{printf("File \"%s\" opened.\n",filename);}

 fputint(0x4D42,fp,0x1A); /*26 bytes because 14 file header + 12 dib header*/

 /*Set up the pallete*/
 fputint(0x000000,fp,3);
 fputint(0xFFFFFF,fp,3);

 /*fprintf(fp,"Chastity Checker");*/
 data=ftell(fp);
 /*printf("Pixels Start at file address: %d\n",data);*/

 y=height;
 while(y>0)
 {
  int bitcount=0;
  int c=0;
  y--;
  x=0;
  while(x<width)
  {
   /*make grayscale from rgb then turn into either black or white*/
   red=pointer[0];
   green=pointer[1];
   blue=pointer[2];
   gray=(red+green+blue)/3;
   bit=gray>>7;
   
   pointer+=4;

   c<<=1;
   c|=bit;
   bitcount++;
   x++;
   if(bitcount%8==0)
   {
    fputc(c,fp);
   }
  }

  /*This loop fixes things when the image is not a multiple of 32 bits in width.*/
  while(bitcount%32!=0)
  {
   c<<=1;
   bitcount++;
   if(bitcount%8==0)
   {
    fputc(c,fp);
   }
  }
 }

 /*start of header fill code*/

 x=ftell(fp); /*get the size of the file now that it has been written.*/
 fseek(fp,2,SEEK_SET); /*seek back to offset 2*/
 fputint(x,fp,4);

 fseek(fp,0xA,SEEK_SET); /*seek back to offset A*/
 fputint(data,fp,4); /*Tell it where the pixels start!*/

 fseek(fp,0xE,SEEK_SET); /*seek back to location for dib header size*/
 fputint(12,fp,4); /*The size of this header (12 bytes)*/

 fseek(fp,0x12,SEEK_SET); /*Seek to width and height location*/
 fputint(width,fp,2); /*The bitmap width in pixels (unsigned 16-bit)*/
 fputint(height,fp,2); /*The bitmap height in pixels (unsigned 16-bit)*/

 fseek(fp,0x16,SEEK_SET);
 fputint(1,fp,2); /*The number of color planes, must be 1*/

 fseek(fp,0x18,SEEK_SET); /*Bits Per Pixel location*/
 fputint(bpp,fp,2);

 /*end of header fill code*/

 fclose(fp);

 /*printf("Saved to file: %s\n",filename);*/

}

int frame=0;
/*saves the current window as an image file and increments frame number*/
void gl_bbm_save_frame()
{
 char filename[256];
 sprintf(filename,"o/%08d.bmp",frame);
 /*printf("%s\n",filename);*/
 glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels_array);
 gl_bbm_SaveBMP1(pixels_array,width,height,filename);
 frame++;
}

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
 polygon(); /*draw the polygon*/
 glFlush(); /*refresh window*/
}

void gl_bbm_main_loop(int x)
{
 display();
 
 /*optionally save frame as file*/
 if(0)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glutLeaveMainLoop();}
 }
 
 polygon_radians+=polygon_radians_step;
 glutTimerFunc(1000/fps,gl_bbm_main_loop,0);
}

int main(int argc, char **argv)
{
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
