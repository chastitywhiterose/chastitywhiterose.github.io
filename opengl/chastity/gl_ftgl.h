/*
*/

int chastity_ftgl_begin()
{
 /*sprintf(fontname,"/usr/share/fonts/truetype/hack/Hack-Regular.ttf");*/ /*on my q4os installation*/
 sprintf(fontname,"/usr/share/fonts/TTF/Hack-Regular.ttf"); /*on my manjaro installation*/
 font = ftglCreatePixmapFont(fontname);

 if(!font)
 {
  printf("Failed to load font \"%s\"!\n",fontname);
  return 0;
 }
 printf("Font \"%s\" loaded\n",fontname);
 
 ftglSetFontFaceSize(font,fontsize,0);
 
 return 1;
}
