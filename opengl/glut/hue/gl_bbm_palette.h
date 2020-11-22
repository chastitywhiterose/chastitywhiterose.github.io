/*
This file is all about being able to make color palettes for my OpenGL programs. The idea was based on something I had written for my BBM project but that required much modification to be compatible with the way colors are specified in OpenGL
*/

#define bbm_palette_max 0x1000000

/*global variables to be used to store the colors and how many there are*/
uint32_t bbm_palette[bbm_palette_max],bbm_palette_length=0;

void bbm_palette_view()
{
 uint32_t x=0;
 while(x<bbm_palette_length)
 {
  printf("bbm_palette[%d]=%06X;\n",x,bbm_palette[x]);
  x++;
 }
 printf("bbm_palette_length=%d;\n",bbm_palette_length);
}


/*
 This function was copied from a very old project known as CK3D. It attempts to make a rainbow using the integer n passed to it using degrees of red,green,blue.
 It seems that the number of colors is equal to 6*n. This means that if n is 20, you get 6*20==120;
 The max value n can be is 255 which gives 1530 colors! However my favorite is n=40 because 240 colors is really convenient for fitting under the 256 color gif limit.
*/
void bbm_palette_rainbow(int n)
{
 int x,color;
 int red=n,green=0,blue=0;
 x=0;
 while(green<n)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  green++;
 }
 while(red>0)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  red--; 
 }
 while(blue<n)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  blue++;
 }
 while(green>0)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  green--; 
 }
 while(red<n)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  red++; 
 }
 while(blue>0)
 {
  color=255*red/n;color*=256;color+=255*green/n;color*=256;color+=255*blue/n;bbm_palette[x]=color;
  x++;
  blue--; 
 }
 bbm_palette_length=x;
}

/*a function for testing how many colors are created with my rainbow function*/
void bbm_palette_rainbow_test(int n)
{
 bbm_palette_rainbow(n);
 bbm_palette_view();
}
