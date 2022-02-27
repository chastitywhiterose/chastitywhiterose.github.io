/*chastity tetris*/

#define tetris_array_size 0x1000
uint32_t tetris_grid[tetris_array_size];
uint32_t tetris_grid_backup[tetris_array_size];

uint32_t grid_width=16,grid_height=16;

/*details of current block*/
uint32_t block_color=0x00FFFF;
int block_x=0,block_y=0,block_width=4,block_x1=0,block_y1=0;

int next_block_x=6,next_block_y=0;

uint32_t block_array_i[]=
{
 0,0,0,0,
 1,1,1,1,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array[]=
{
 0,0,0,0,
 1,1,1,1,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_backup[16];

uint32_t bx,by;

uint32_t score=0;

uint32_t empty_color=0x000000;


uint32_t lines_cleared=0,lines_cleared_last=0,lines_cleared_total=0;


void mode_tetris()
{
 uint32_t pixel,r,g,b;
 uint32_t x=0,y=0;
 uint32_t *p=tetris_grid;

 int block_size=height/grid_height;

 block_x=next_block_x;
 block_y=next_block_y;

 printf("block_size==%d\n",block_size);

 /*first empty the grid*/
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

 /*
  x=8;y=8;
  p[x+y*grid_width]=0xFF00FF;
 */


 /*set size and position of polygon*/
 polygon_cx=width*13/16;
 polygon_cy=height/2;
 polygon_radius=height/3;
  /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 framelimit=fps*60*60;

  glColor3f(1.0,1.0,1.0); /*color of the polygon*/


  block_x=next_block_x;block_y=next_block_y;
 
  /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {

  glClear(GL_COLOR_BUFFER_BIT);

 /*make backup of entire grid*/
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   tetris_grid_backup[x+y*grid_width]=p[x+y*grid_width];
   x+=1;
  }
  y+=1;
 }

  /*draw block onto grid at it's current location*/
  by=0;
  while(by<4)
  {
   bx=0;
   while(bx<4)
   {
    if(block_array[bx+by*4]!=0)
    {
     if( p[block_x+bx+(block_y+by)*grid_width]!=0 )
     {
      printf("Error: Block in Way\n");

      /*because a collision has occurred. We restore everything back to the way it was before block was moved.*/

      /*restore backup of block location*/
      block_x=block_x1,block_y=block_y1;

     /*Restore backup of entire grid*/
     y=0;
     while(y<grid_height)
     {
      x=0;
      while(x<grid_width)
      {
       p[x+y*grid_width]=tetris_grid_backup[x+y*grid_width];
       x+=1;
      }
      y+=1;
     }

      break;}
     else
     {
      p[block_x+bx+(block_y+by)*grid_width]=block_color;
     }
    }
    bx+=1;
   }
   by+=1;
  }




/*display the tetris grid*/



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


 /*Restore backup of entire grid*/
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   p[x+y*grid_width]=tetris_grid_backup[x+y*grid_width];
   x+=1;
  }
  y+=1;
 }



 /*change color back to white before drawing text*/
 glColor3f(1.0,1.0,1.0);

 /*draw the boundary wall*/
 glRecti(grid_width*block_size,0*block_size,grid_width*block_size+block_size,height*block_size+block_size);

 /*
   glRasterPos2i(fontsize,fontsize);
   sprintf(text,"%d",frame_displayed);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 */

/*glRasterPos2i(width-fontsize*12,height-fontsize*1);
   sprintf(text,"%d:%02d:%02d",hours,minutes,seconds);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
*/

/*
 glRasterPos2i(width-fontsize*6,height-fontsize*1);
 sprintf(text,"x=%d y=%d",block_x,block_y);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);
*/


 glRasterPos2i(fontsize*17,fontsize*1);
 ftglRenderFont(font,"Long Boi Game", FTGL_RENDER_ALL);

 glRasterPos2i(width-fontsize*10,height-fontsize*8);
 sprintf(text,"Lines: %d",lines_cleared_total);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);

 glRasterPos2i(width-fontsize*10,height-fontsize*7);
 sprintf(text,"Score: %d",score);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);


 glRasterPos2i(fontsize*17,height-fontsize*1);
 ftglRenderFont(font,"Chastity Rose", FTGL_RENDER_ALL);
 
/* gl_bbm_checker();*/
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
