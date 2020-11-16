#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

const int width=720,height=720; /*size of window*/

#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
const int polygon_sides=5;         /*how many sides and/or corners of polygon*/
int polygon_step=2;
float polygon_cx=width/2;          /*horizontal left/right x coordinate of center*/
float polygon_cy=height/2;         /*vertical up/down y coordinate of center*/
float polygon_radius=height/2;     /*radius or how big the polygon will be*/
float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

/*frames per second*/
int fps=60,framelimit=360/polygon_sides;

/*timer variables,used for delay*/
double seconds,seconds1;

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

void star()
{
 int i,i1;
 make_polygon_points();
 glBegin(GL_TRIANGLES);
 i=0; i1=0;
 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  glVertex2f(polygon_xpoints[i1],polygon_ypoints[i1]);
  glVertex2f(polygon_cx,polygon_cy);
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

 data=ftell(fp);

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
  }
 }
}

int main(int argc, char **argv)
{
 GLFWwindow* window;
 printf("Chastity White Rose\nSpinning Polygon\n");
 if(!glfwInit()){return 1;} /*Initialize GLFW*/

 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2); /*Using version 2.1 of OpenGL*/
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
 glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE); /*single buffered window*/
 window=glfwCreateWindow(width,height,"Chastity's OpenGL Spinning Polygon using GLFW",NULL,NULL);
 if(!window){glfwTerminate();return 1;}

 glfwMakeContextCurrent(window);
 glfwSetKeyCallback(window,key_callback);

 glOrtho(0.0,width,height,0.0,-1.0,1.0); /*2D orthographic matrix*/
 glClearColor(0.0,0.0,0.0,1.0); /*color used to clear the window*/
 glColor3f(1.0,1.0,1.0); /*color of the polygon*/

 /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);
  star();
  glFlush();
  
 /*optionally save frame as file*/
 if(0)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
  
  polygon_radians+=PI/180;
  glfwPollEvents();

  seconds=glfwGetTime();
  seconds1=seconds+1.0/fps;
  while(seconds<seconds1)
  {
   seconds=glfwGetTime();
  }
 }

 glfwTerminate();
 return 0;
}
