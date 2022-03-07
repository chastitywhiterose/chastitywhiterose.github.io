/*chastity tetris*/

#define tetris_array_size 0x1000
uint32_t tetris_grid[tetris_array_size];
uint32_t tetris_grid_backup[tetris_array_size];

uint32_t grid_width=10,grid_height=20;

/*details of current block*/
uint32_t block_color=0x00FFFF;
int block_x=0,block_y=0,block_width=4,block_x1=0,block_y1=0,current_block_width=4,current_block_id=0;

int next_block_x,next_block_y;

uint32_t block_array_backup[16],backup_block_width,backup_block_color;/*backup during rotation*/

int hold_used=0;
int block_array_hold[16],hold_block_width,hold_block_color,hold_block_id=0; /*the hold block storage*/
int block_array_hold1[16],hold1_block_width,hold1_block_color,hold1_block_id; /*the second hold block storage*/

int moves=0; /*number of valid moves*/
int moves_tried=0; /*number of attempted moves*/
int last_move_spin=0;
int back_to_back=0;


uint32_t bx,by;

uint32_t score=0;

uint32_t empty_color=0x000000;


uint32_t lines_cleared=0,lines_cleared_last=0,lines_cleared_total=0;

uint32_t block_array_i[]=
{
 0,0,0,0,
 1,1,1,1,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_t[]=
{
 0,1,0,0,
 1,1,1,0,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_z[]=
{
 1,1,0,0,
 0,1,1,0,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_j[]=
{
 1,0,0,0,
 1,1,1,0,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_o[]=
{
 1,1,0,0,
 1,1,0,0,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_l[]=
{
 0,0,1,0,
 1,1,1,0,
 0,0,0,0,
 0,0,0,0,
};

uint32_t block_array_s[]=
{
 0,1,1,0,
 1,1,0,0,
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



int block_type=0;

void spawn_block()
{
 int x,y;
 uint32_t *p;

 if(block_type==0)
 {
  p=block_array_i;
  block_color=0x00FFFF;
  current_block_width=4;
  current_block_id='I';
 }

 if(block_type==1)
 {
  p=block_array_t;
  block_color=0xFF00FF;
  current_block_width=3;
  current_block_id='T';
 }

 if(block_type==2)
 {
  p=block_array_z;
  block_color=0xFF0000;
  current_block_width=3;
  current_block_id='Z';
 }

 if(block_type==3)
 {
  p=block_array_j;
  block_color=0x0000FF;
  current_block_width=3;
  current_block_id='J';
 }

 if(block_type==4)
 {
  p=block_array_o;
  block_color=0xFFFF00;
  current_block_width=2;
  current_block_id='O';
 }

 if(block_type==5)
 {
  p=block_array_l;
  block_color=0xFF7F00;
  current_block_width=3;
  current_block_id='L';
 }

 if(block_type==6)
 {
  p=block_array_s;
  block_color=0x00FF00;
  current_block_width=3;
  current_block_id='S';
 }

 /*copy another new block array into the current one*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array[x+y*block_width]=p[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 next_block_x=(grid_width-current_block_width)/2;
 next_block_y=0;

 block_x=next_block_x;
 block_y=next_block_y;



}

void chaste_tris_info()
{
 printf("Welcome to the game \"%s\" by Chastity White Rose\n",gamename);
 printf("Email: chastitywhiterose@gmail.com for any questions!\n\n");

 printf("This game was written in the C programming language by Chastity White Rose.\nThe inspiration came from a talk with River Black Rose. There are two versions.\n");
 printf("\"Long Boi\" containing only I blocks and\n\"Chaste Tris\" when all 7 Tetrominoes are present\n");
 printf("And of course this game is heavily inspired by Tetris created by Alexey Pajitnov in 1984\n\n");

 printf("Because I love playing Tetris, I used all my programming skills to make a simple game that I can play with a few features that I wish other Tetris games had.\n");
 printf("There is infinite time to place the blocks because there is no gravity.\n");


}


void mode_tetris()
{
 uint32_t pixel,r,g,b;
 uint32_t x=0,y=0;
 uint32_t *p=tetris_grid;

 int block_size=height/grid_height;
 int grid_offset_x=block_size; /*how far from the left size of the window the grid display is*/

 next_block_x=(grid_width-current_block_width)/2;
 next_block_y=0;

 /*printf("block_size==%d\n",block_size);*/

 chaste_tris_info();

 spawn_block();


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



 framelimit=fps*60*60;


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
   glRecti(grid_offset_x+x*block_size,y*block_size,grid_offset_x+x*block_size+block_size,y*block_size+block_size);

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

  /*printf("last_move_spin==%d\n",last_move_spin);*/

 /*change color back to white before drawing text*/
 glColor3f(1.0,1.0,1.0);

 /*draw the boundary walls*/
 glRecti(0*block_size,0*block_size,block_size,height*block_size);
 glRecti(grid_offset_x+grid_width*block_size,0*block_size,grid_offset_x+grid_width*block_size+block_size,height*block_size);

 
/*   glRasterPos2i(fontsize,fontsize);
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


 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*1);
 ftglRenderFont(font,gamename, FTGL_RENDER_ALL);

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*3);
 sprintf(text,"Lines: %d",lines_cleared_total);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*4);
 sprintf(text,"Score: %d",score);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*5);
 sprintf(text,"This: %c",current_block_id);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*6);
 sprintf(text,"Hold: %c",hold_block_id);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);


 /*glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*8);
 sprintf(text,"Moves: %d",moves);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);*/

/* glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*9);
 sprintf(text,"Moves Tried: %d",moves_tried);
 ftglRenderFont(font,text, FTGL_RENDER_ALL);
*/

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*10);
 ftglRenderFont(font,"Chastity White Rose", FTGL_RENDER_ALL);

 glRasterPos2i(grid_width*block_size+fontsize*2,fontsize*12);
 ftglRenderFont(font,"River Black Rose", FTGL_RENDER_ALL);

 
 
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
 
 /*
 optionally save frame as file
 make comparison moves>=frame to ensure frames are only saved for successful moves.
 use moves<frame to make sure that no frames are ever saved
*/

 if(moves<frame)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
 

  
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
