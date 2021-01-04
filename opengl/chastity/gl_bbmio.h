/*
Header file for file input/output

Most of the time these functions are never used because the beauty of using OpenGL and window toolkits like freeglut and glfw is that graphics are draw directly to the screen making image files almost obsolete.

There are specific times when I have a need to save image files for later use of encoding into video files. For that reason I will always preserve the functions here.
*/


/*
simple function to enable XOR mode
*/
void gl_Enable_XOR()
{
 glEnable(GL_COLOR_LOGIC_OP);
 glLogicOp(GL_XOR);
}





/*
large static RAM storage
*/
#define pixels_array_size 0x1000000
GLubyte pixels_array[pixels_array_size];

/*store pixels of the current window into the above array*/
void gl_window_pixels_save()
{
 glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels_array);
}



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
Saves to BMP format but monochrome! This is probably the oldest and most reliable way of quickly making black and white only images.
*/

void gl_bbm_SaveBMP1(GLubyte *pointer,int width,int height,const char* filename)
{

 int x,y,data,bpp=1,bit,red,green,blue,gray;
 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 else{/*printf("File \"%s\" opened.\n",filename);*/}

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



/*
a translation of my bbm function which saves 8 bpp grayscale BMP files.
*/
void gl_bbm_SaveBMP8(GLubyte *pointer,int width,int height,const char* filename)
{
 int x,y,pixelpointer,bytecount,red,green,blue,gray;
 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 fputint(0x4D42,fp,0x1A); /*26 bytes because 14 file header + 12 dib header*/
 
  /*Set up the pallete*/
 x=0;
 while(x<256)
 {
  fputc(x,fp);fputc(x,fp);fputc(x,fp);
  x++;
 }
 
 pixelpointer=ftell(fp);
 y=height;
 while(y>0)
 {
  bytecount=0;
  y--;
  x=0;
  while(x<width)
  {
   /*make grayscale from rgb*/
   red=pointer[0];
   green=pointer[1];
   blue=pointer[2];
   gray=(red+green+blue)/3;
   fputc(gray,fp);
   
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
 fputint(8,fp,2); /*Bits Per Pixel*/
 fclose(fp);
}



/*
A function specifically designed for making 3 bits per pixel color image. BMP files support 4 bits per pixel indexed images. So in this case only 3 bits are actually used in each half byte. This was ported to my opengl code for programs that actually only use 8 colors to save space when making tons of images.
*/

void gl_bbm_SaveBMP3(GLubyte *pointer,int width,int height,const char* filename)
{
 int x,y,data,bpp=3,bits,gray,bitcount,color,pixel,red,green,blue,palette_length=8;
 FILE* fp;
 fp=fopen(filename,"wb+");
 if(fp==NULL){printf("Failed to create file \"%s\".\n",filename); return;}
 else{/*printf("File \"%s\" opened.\n",filename);*/}

 fputint(0x4D42,fp,0x1A); /*26 bytes because 14 file header + 12 dib header*/

 /*Set up the pallete*/

  bpp/=3;
  gray=(1<<bpp)-1;

  x=0;
  while(x<palette_length)
  {
   color=x;
   blue=color&gray;color>>=bpp;
   green=color&gray;color>>=bpp;
   red=color&gray;color>>=bpp;

    red<<=16;
    green<<=8;

    bitcount=0;
    while(bitcount<8)
    {
     color<<=bpp;
     color|=red;
     color|=green;
     color|=blue;
     bitcount+=bpp;
    }

   fputint(color,fp,3);

   x++;
  }

 /*fprintf(fp,"Chastity Checker");*/
 data=ftell(fp);
 /*printf("Pixels Start at file address: %d\n",data);*/

 y=height;
 while(y>0)
 {
  y--;
  bits=0;
  x=0;
  while(x<width)
  {
   red=pointer[0];
   green=pointer[1];
   blue=pointer[2];
   pointer+=4;

   /*right shift each component by the bpp to eliminate extra bits.*/
   red>>=8-bpp;
   green>>=8-bpp;
   blue>>=8-bpp;

   /*next left shift red and green back to their proper position as they would be in 3 bits rgb. blue doesn't need to be shifted.*/
   red<<=2;
   green<<=1;

   pixel=red|green|blue;

   bits<<=4;
   bits|=pixel;
   bitcount+=4;

  if(bitcount%8==0)
  {
   fputc(bits,fp);
  }

  x++;
 }

  /*This loop fixes things when the image is not a multiple of 32 bits in width.*/
  while(bitcount%32!=0)
  {
   bits<<=4;
   bitcount+=4;
   if(bitcount%8==0)
   {
    fputc(bits,fp);
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
 fputint(4,fp,2); /*4 according to the Microsoft API even though only 3 bits are used in this function.*/

 /*end of header fill code*/

 fclose(fp);

 /*printf("Saved to file: %s\n",filename);*/

}




/*
a translation of my bbm function which saves 24 bpp BMP files.
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


/*variables controlling the frame number and names of frame files*/
int frame=0;
char framefilename[64];
char framefilename1[64];
char command[0x100];

/*
Uses the ancient pnmtopng program if it is installed as a hack to allow for converting PAM files to PNG format. It also works with the older three PBM, PGM, and PPM formats.
*/
void pnmtopng(const char* infile,const char* outfile)
{
 char command[256];
 sprintf(command,"pnmtopng <%s >%s",infile,outfile);
 printf("%s\n",command);
 system(command);
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
  sprintf(command,"magick convert %s -colorspace lineargray %s",framefilename,framefilename1);
/*  sprintf(command,"magick convert %s -colorspace rgb %s",framefilename,framefilename1);*/

  system(command);
  if( remove( framefilename ) != 0 ){printf("could not delete file %s\n",framefilename);}
 }

 printf("%s\n",command);

 
 frame++;
}



/*
This function is more of a proof of concept than anything useful. What it does is read all the pixels in the window,use an xor operation to invert the colors, then write them back. Although this is slower than just drawing the intended colors in the first place, it shows that I can access the pixels through traditional memory access. This means theoretically my whole BBM library can be translated to work with opengl. Although the pixel format used is not identical to my conventions used in that library. 
*/
void gl_window_pixels_xor()
{
 int x,y;
 GLubyte *pointer;
 glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels_array);
 pointer=pixels_array;
 y=0;
 while(y<height)
 {
  x=0;
  while(x<width)
  {
   pointer[0]^=0xFF;
   pointer[1]^=0xFF;
   pointer[2]^=0xFF;
   pointer+=4;
   x+=1;
  }
  y+=1;
 }
 glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels_array);
}
