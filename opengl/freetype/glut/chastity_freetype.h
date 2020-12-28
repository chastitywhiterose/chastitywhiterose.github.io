/*This file is my own attempt of learning FreeType. Rather than having all the FreeType initialization and rendering in the main function I instead put the functions and global variables here.*/

FT_Library library;   /* handle to library     */
FT_Face face;      /* handle to face object */
int error; /*to store error codes*/


FT_GlyphSlot  slot;
FT_UInt       glyph_index;
int           pen_x,pen_y,n,num_chars=5;

char fontname[256];
char text[256];

/*load all the correct freetype things and set the font size*/
void chastity_freetype_begin()
{
 printf("Loading FreeType stuff\n");
 error=FT_Init_FreeType(&library);
 if(error){printf("Error!");}
 /*sprintf(fontname,"/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf");*/
 /*sprintf(fontname,"/usr/share/fonts/truetype/freefont/FreeMono.ttf");*/
 /*sprintf(fontname,"/usr/share/fonts/truetype/hack/Hack-Regular.ttf");*/ /*on my q4os installation*/
 sprintf(fontname,"/usr/share/fonts/TTF/Hack-Regular.ttf"); /*on my manjaro installation*/

 error=FT_New_Face(library,fontname,0,&face);
 if(error==FT_Err_Unknown_File_Format)
 {
  printf("error==FT_Err_Unknown_File_Format\n");
 }
 else if(error)
 {
  printf("error==%d\n",error);
  printf("The file named may not exist!\n");
  printf("Or you may be using an invalid index. 0 is safe\n");
 }

 if(1)
 {
 error = FT_Set_Pixel_Sizes(
          face,   /* handle to face object */
          60,      /* pixel_width           */
          60 );   /* pixel_height          */
 }
 else
 {
  error = FT_Set_Char_Size(
          face,    /* handle to face object           */
          0,       /* char_width in 1/64th of points  */
          128*64,   /* char_height in 1/64th of points */
          300,     /* horizontal device resolution    */
          300 );   /* vertical device resolution      */
 }


}

void chastity_freetype_end()
{
 printf("chastity_freetype_end();\n");
 FT_Done_Face    ( face );
 FT_Done_FreeType( library );
}

/* origin is the upper left corner */
/*unsigned char image[HEIGHT][WIDTH];*/
/*0x1000000*/

GLubyte fontimage[0x1000000];

/* This function is for debugging only. */

void gl_draw_bitmap(FT_Bitmap*  bitmap,int destx,int desty)
{
 int bitmap_width,bitmap_height;
 int x=0,y=0,x1,y1,pixel,gray;
 GLint *pointer=(GLint*)fontimage;
 
 bitmap_width=bitmap->width,
 bitmap_height=bitmap->rows;
 
 y=0;
 while(y<bitmap_height)
 {
  y1=height-1-y-desty;
  x=0;
  while(x<bitmap_width)
  {
   pixel=0;
   gray=bitmap->buffer[x+y*bitmap_width];
   pixel<<=8;pixel|=gray;
   pixel<<=8;pixel|=gray;
   pixel<<=8;pixel|=gray;
   x1=destx+x;
   pointer[x1+y1*width]=pixel;
   x++;
  }
  printf("\n");
  y++;
 }

}

void gl_putchar(int charcode,int x,int y)
{
 glyph_index = FT_Get_Char_Index( face, charcode );
 error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT );  
 error = FT_Render_Glyph( face->glyph,FT_RENDER_MODE_NORMAL ); 
 slot = face->glyph;
 gl_draw_bitmap(&slot->bitmap,x+ slot->bitmap_left,y- slot->bitmap_top);
}

void gl_putstr(char *textstr,int pen_x,int pen_y)
{
 num_chars=strlen(textstr);
 for ( n = 0; n < num_chars; n++ )
 {
  gl_putchar(textstr[n],pen_x,pen_y);
  /* increment pen position */
  pen_x += slot->advance.x>>6;
 }

}



/* This function is for debugging only. */
void my_draw_bitmap_to_terminal( FT_Bitmap*  bitmap)
{
 int bitmap_width,bitmap_height;
 int x=0,y=0,pixel;
 
 bitmap_width=bitmap->width,
 bitmap_height=bitmap->rows;
 /*
 printf("bitmap_width=%d\n",bitmap_width);
 printf("bitmap_height=%d\n",bitmap_height);
 */
 
 y=0;
 while(y<bitmap_height)
 {
  x=0;
  while(x<bitmap_width)
  {
   pixel=bitmap->buffer[x+y*bitmap_width];
   if(pixel==0){printf("0");}
   else{printf("1");}
   x++;
  }
  printf("\n");
  y++;
 }

}

