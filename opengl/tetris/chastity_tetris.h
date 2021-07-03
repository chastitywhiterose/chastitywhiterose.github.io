/*chastity tetris*/

#define tetris_array_size 0x1000
uint32_t tetris_array[tetris_array_size];

/*details of current block*/
uint32_t block_color=0x00FF00;
int block_x=0,block_y=0,old_block_x=0,old_block_y=0;

int next_block_x=8,next_block_y=0;

uint32_t score=0;

uint32_t empty_color=0x000000;

int direction=0,up=1,down=2,left=3,right=4;

void mode_tetris()
{
 uint32_t pixel,r,g,b;
 uint32_t x=0,y=0;
 uint32_t *p=tetris_array;
 uint32_t grid_width=16,grid_height=16;

 int block_size=height/grid_height;

 printf("block_size==%d\n",block_size);

 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   p[x+y*grid_width]=empty_color;
   x+=1;
  }
  y+=1;
 }

 x=8;y=8;
 p[x+y*grid_width]=0xFF00FF;



 /*set size and position of polygon*/
 polygon_cx=width/2;
 polygon_cy=height/2;
 polygon_radius=height/3;
  /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 framelimit=fps*60*60;

  glColor3f(1.0,1.0,1.0); /*color of the polygon*/


  block_x=next_block_x;block_y=next_block_y;
  old_block_x=block_x;old_block_y=block_y;

 
  /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);



  if(block_x<0||block_x>=grid_width){block_x=old_block_x;}
  else if(block_y<0||block_y>=grid_height){block_y=old_block_y;}

  if(p[block_x+block_y*grid_width]==empty_color)
  {
    p[old_block_x+old_block_y*grid_width]=empty_color;
    p[block_x+block_y*grid_width]=block_color;
    old_block_x=block_x;old_block_y=block_y;
  }
  else
  {

   block_x=old_block_x;
   block_y=old_block_y;


   if(direction==down)
   {
    block_x=next_block_x;block_y=next_block_y;
    old_block_x=block_x;old_block_y=block_y;
   }

  }

  direction=0; /*reset the direction until next input*/


/*drawing the tetris grid*/
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {

   pixel=p[x+y*grid_width];
   r=(pixel&0xFF0000)>>16;
   g=(pixel&0x00FF00)>>8;
   b=(pixel&0x0000FF);

 
/* printf("x=%d y=%d ",x,y);
   printf("red=%d green=%d blue=%d\n",r,g,b);
*/

   glColor3ub(r, g, b);
   glRecti(x*block_size,y*block_size,x*block_size+block_size,y*block_size+block_size);

   x+=1;
  }
  y+=1;
 }
 /*end of drawing code for grid*/

 /*change color back to white before drawing text*/
 glColor3f(1.0,1.0,1.0);

 /*draw the boundary wall*/
 glRecti(grid_width*block_size,0*block_size,grid_width*block_size+block_size,height*block_size+block_size);

 /*
   glRasterPos2i(fontsize,fontsize);
   sprintf(text,"%d",frame_displayed);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 */
   glRasterPos2i(width-fontsize*5,fontsize);
   sprintf(text,"%d:%02d:%02d",hours,minutes,seconds);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);

 glRasterPos2i(width-fontsize*6,height-fontsize*2);
 sprintf(text,"x=%d y=%d",block_x,block_y);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);

 glRasterPos2i(width-fontsize*6,height-fontsize);
 sprintf(text,"%06d",score);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);


/*
 glRasterPos2i(fontsize*18,height-fontsize);
 ftglRenderFont(font,"Chastity", FTGL_RENDER_ALL);
*/ 

 
 /*gl_bbm_checker();*/
 /*polyfunc();*/
 
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
 if(0)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
 
  polygon_radians+=PI/180;
  
  /*check for keypresses or other events*/
  glfwPollEvents();
 
  
  glfwseconds=glfwGetTime();
  glfwseconds1=glfwseconds+1.0/fps;
  while(glfwseconds<glfwseconds1)
  {
   glfwseconds=glfwGetTime();
  }
  
  
 }
 
}
