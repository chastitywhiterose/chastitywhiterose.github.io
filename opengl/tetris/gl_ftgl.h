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
   return 0;
  }
 }

 printf("Font \"%s\" loaded\n",fontname);
 return 1;
}
