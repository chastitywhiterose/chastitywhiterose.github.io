#include <stdio.h>
/*#include <stdlib.h>*/
/*#include <stdint.h>*/
/*#include <string.h>*/
/*#include <math.h>*/
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

/*size of window*/
int width=1280,height=720;
/*ideal for fullscreen*/
/*int width=1920,height=1080;*/
int fullscreen=0;

/*Timer variables,used for delay. They are double and not to be confused with integer variables of display times.*/
double glfwseconds,glfwseconds1;

GLFWwindow* window; /*window pointer*/


FTGLfont *font;
char fontname[256];
char text[256];
int fontsize=48;
int frame_displayed=0;

char gamename[256];
int blocks_used=7;

/*variables controlling the frame number and names of frame files*/
int frame=0;
char framefilename[64];
char framefilename1[64];
char command[0x100];

/*frames per second and other animation variables*/
int fps=60,framelimit=1,seconds=0,minutes=0,hours=0;


/*large static RAM storage for pixels*/
#define pixels_array_size 0x1000000
GLubyte pixels_array[pixels_array_size];


/*
A quick dirty replacement for never needing to use fwrite to write my integers! However the catch is that it only works for little endian. For this reason I choose little endian as the format the binary integers will take in my own image formats I am creating. It writes integer i to file pointer fp using count bytes. Most PCs these days can't actually do more than 32 or 64 bits.

This function was necessary so that my code won't completely fail if I do programming on a different CPU in which case fwrite would spit out the big endian byte order. Besides it wasn't too hard to write and requires only 3 arguments instead of 4.
*/
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
An OpenGL translation of my BBM function which saves 24 bpp BMP files.
*/
void gl_bbm_SaveBMP(GLubyte *pointer,int width,int height,const char* filename)
{
 int x,y,pixelpointer,bytecount;
 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 fputint(0x4D42,fp,0x1A); /*26 bytes because 14 file header + 12 dib header*/
 pixelpointer=ftell(fp);
 y=height;
 while(y>0)
 {
  bytecount=0;
  y--;
  x=0;
  while(x<width)
  {
   fputc(pointer[2],fp);
   fputc(pointer[1],fp);
   fputc(pointer[0],fp);
   pointer+=4;
   bytecount+=3;
   x++;
  }
  while(bytecount&3){fputc(0,fp);bytecount++;}
 }
 x=ftell(fp); /*get the size of the file now that it has been written.*/
 fseek(fp,2,SEEK_SET); /*seek back to offset 2*/
 fputint(x,fp,4); /*write file size*/
 fseek(fp,0xA,SEEK_SET); /*seek to location of pixel pointer*/
 fputint(pixelpointer,fp,4); /*Tell it where the pixels start!*/
 fputint(12,fp,4); /*The size of this header (12 bytes)*/
 fputint(width,fp,2); /*The bitmap width in pixels (unsigned 16-bit)*/
 fputint(height,fp,2); /*The bitmap height in pixels (unsigned 16-bit)*/
 fputint(1,fp,2); /*The number of color planes, must be 1*/
 fputint(24,fp,2); /*Bits Per Pixel*/
 fclose(fp);
}


/*saves the current window as an image file and increments frame number*/
void gl_bbm_save_frame()
{
 sprintf(framefilename,"o/%08d.bmp",frame);
 /*printf("%s\n",filename);*/
 glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels_array);
 gl_bbm_SaveBMP(pixels_array,width,height,framefilename);

 /*
  This next optional section converts to PNG with ImageMagick.
  Depending on which colorspace is selected, it can make a difference in file size.
 */
 if(1)
 {
  sprintf(framefilename1,"o/%08d.png",frame);
/*  sprintf(command,"magick convert %s -monochrome %s",framefilename,framefilename1);*/
/*  sprintf(command,"magick convert %s -colorspace lineargray %s",framefilename,framefilename1);*/
  /*sprintf(command,"magick convert %s -colorspace rgb %s",framefilename,framefilename1);*/

 sprintf(command,"gm convert %s %s",framefilename,framefilename1);

  system(command);
  printf("%s\n",command);
  if( remove( framefilename ) != 0 ){printf("could not delete file %s\n",framefilename);}
 }

 frame++;
}





void error_callback(int error,const char *description)
{
 fprintf(stderr,"Error: %s\n",description);
}


/*
 Before main and keyboard functions, include the header file with all my custom written Tetris routines.
These are called depending on which key is pressed. Display of squares is done with OpenGL in this edition but could be replaced with any other programming library.
*/
#include "chastetris.h"

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
   tetris_move_up();
   break;
   case GLFW_KEY_LEFT:
   case GLFW_KEY_A:
   tetris_move_left();
   break;
   case GLFW_KEY_DOWN:
   case GLFW_KEY_S:
    tetris_move_down();
   break;
   case GLFW_KEY_RIGHT:
   case GLFW_KEY_D:
 /*make backup of block location*/
tetris_move_right();
   break;

   case GLFW_KEY_X:
   block_rotate_right();
   break;
   case GLFW_KEY_Z:
   block_rotate_left();
   break;

   case GLFW_KEY_C:
   block_hold();
   break;

   case GLFW_KEY_COMMA:
   tetris_load_state();
   break;
   case GLFW_KEY_PERIOD:
   tetris_save_state();
   break;

  }

 }

}


/*
 This function is intended to load a font. It can try more than one.
*/

int chastity_ftgl_begin()
{
 
 sprintf(fontname,"/usr/share/fonts/truetype/hack/Hack-Regular.ttf"); /*on my q4os installation*/
 font = ftglCreatePixmapFont(fontname);
 if(!font)
 {
  printf("Failed to load first font \"%s\"!\n",fontname);
  printf("Will try another one.\n");
  
  sprintf(fontname,"/usr/share/fonts/TTF/Hack-Regular.ttf"); /*on my manjaro installation*/
  font = ftglCreatePixmapFont(fontname);
  if(!font)
  {
   printf("Failed to load second font \"%s\"!\n",fontname);
   printf("Will try one more time.\n");

   sprintf(fontname,"./font/Hack-Regular.ttf"); /*included with source*/
   font = ftglCreatePixmapFont(fontname);
   
   if(!font)
   {
    printf("Failed to load third font \"%s\"!\n",fontname);
    return 0;
   }
   
  }
 }

 printf("Font \"%s\" loaded\n",fontname);
 return 1;
}


int main(int argc, char **argv)
{
 if(!chastity_ftgl_begin()){return 0;}
 ftglSetFontFaceSize(font,fontsize,0);

 if(blocks_used==1)
 { 
  sprintf(gamename,"Long Boi");
 }
 else
 {
  sprintf(gamename,"Chaste Tris");
 }




 glfwSetErrorCallback(error_callback);
 
 if(!glfwInit()){return 1;} /*Initialize GLFW*/
  
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2); /*Using version 2.1 of OpenGL*/
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
 glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE); /*single buffered window*/

 if(fullscreen!=0)
 {
  width=1920;height=1080;
  window=glfwCreateWindow(width,height,gamename,glfwGetPrimaryMonitor(),NULL);
 }
 else
 {
  width=1280;height=720;
  window=glfwCreateWindow(width,height,gamename,NULL,NULL);
 }

 if(!window){glfwTerminate();return 1;}
 
 glfwMakeContextCurrent(window);
 glfwSetKeyCallback(window,key_callback);


 glOrtho(0.0,width,height,0.0,-1.0,1.0); /*2D orthographic matrix*/
 glClearColor(0.0,0.0,0.0,1.0); /*color used to clear the window*/
  
 printf("The OpenGL version is: %s\n",glGetString(GL_VERSION));
 
 /*now that context,fonts,and colors are loaded, we can start the game!*/
 
 chastetris();

 glfwTerminate();
 ftglDestroyFont(font);
 return 0;
}


/*
I usually supply a makefile with the source containing the compile commands for both Linux and Windows. This is the text for compiling OpenGL programs with GLFW and FTGL included.


linux_glfw_ftgl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw -lOpenGL -lm `pkg-config --cflags --libs ftgl` && ./main
win_glfw_ftgl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL32 -lm `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs ftgl` && ./main
*/

