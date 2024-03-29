/*chastity tetris*/

/*Part 1: Declaring variables and constants.*/

#define tetris_array_size 0x1000
uint32_t tetris_grid[tetris_array_size];
uint32_t tetris_grid_backup[tetris_array_size];

uint32_t grid_width=10,grid_height=20;

/*details of main block*/
uint32_t main_block_array[16];
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
   main_block_array[x+y*block_width]=p[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 next_block_x=(grid_width-current_block_width)/2;
 next_block_y=0;

 block_x=next_block_x;
 block_y=next_block_y;



}

void chastetris_info()
{
 printf("Welcome to the game \"%s\" by Chastity White Rose\n",gamename);
 printf("Email: chastitywhiterose@gmail.com for any questions!\n\n");

 printf("This game was written in the C programming language by Chastity White Rose.\nThe inspiration came from a talk with River Black Rose. There are two versions.\n");
 printf("\"Long Boi\" containing only I blocks and\n\"Chaste Tris\" when all 7 Tetrominoes are present\n");
 printf("And of course this game is heavily inspired by Tetris created by Alexey Pajitnov in 1984\n\n");

 printf("Because I love playing Tetris, I used all my programming skills to make a simple game that I can play with a few features that I wish other Tetris games had.\n");
 printf("There is infinite time to place the blocks because there is no gravity.\n");

 printf("Additionally, you can press the '.' key to save the game at any time and then press ',' to reload to the previously saved state.\n");
 printf("This means my game is the only Tetris game that lets you go back in time to fix mistakes!\n\n");
}


void chastetris()
{
 uint32_t pixel,r,g,b;
 uint32_t x=0,y=0;
 uint32_t *p=tetris_grid;

 int block_size=height/grid_height;
 int grid_offset_x=block_size; /*how far from the left size of the window the grid display is*/

 next_block_x=(grid_width-current_block_width)/2;
 next_block_y=0;

 /*printf("block_size==%d\n",block_size);*/

 chastetris_info();

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
    if(main_block_array[bx+by*4]!=0)
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



/*Part 2: Functions that modify the block data or Tetris grid itself. */

/*
Functions for each operation that moves the current block.
*/



int pixel_on_grid(int x,int y)
{
 if(x<0){/*printf("Error: Negative X\n");*/return 1;}
 if(y<0){/*printf("Error: Negative Y\n");*/return 1;}
 if(x>=grid_width){/*printf("Error: X too high.\n");*/return 1;}
 if(y>=grid_height){/*printf("Error: Y too high.\n");*/return 1;}
 else{return tetris_grid[block_x+bx+(block_y+by)*grid_width];}
}

/*checks whether or not the block collides with anything on the current field*/
int tetris_check_move()
{
 int x,y;

 moves_tried++; /*move attempted*/


  by=0;
  while(by<4)
  {
   bx=0;
   while(bx<4)
   {
    if(main_block_array[bx+by*4]!=0)
    {

     if( pixel_on_grid(block_x+bx,block_y+by)!=0 )
     {
      /*printf("Error: Block in Way on Move Check.\n");*/

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
       tetris_grid[x+y*grid_width]=tetris_grid_backup[x+y*grid_width];
       x+=1;
      }
      y+=1;
     }

      return 1;
     }

    }
    bx+=1;
   }
   by+=1;
  }

 moves++; /*move successful*/
 return 0;

}


void tetris_clear_screen()
{
 int x,y;
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   tetris_grid[x+y*grid_width]=empty_color;
   x+=1;
  }
  y+=1;
 }
}


void tetris_clear_lines()
{
 int x,y,xcount,x1,y1;

 lines_cleared=0;

 y=grid_height;
 while(y>0)
 {
  y-=1;

  xcount=0;
  x=0;
  while(x<grid_width)
  {
   if(tetris_grid[x+y*grid_width]!=empty_color){xcount++;}
   x+=1;
  }

  /*printf("row %d xcount %d\n",y,xcount);*/

  if(xcount==grid_width)
  {
 
   y1=y;

   /*printf("row %d line clear attempt.\n",y);*/

   x1=0;
   while(x1<grid_width)
   {
    tetris_grid[x1+y1*grid_width]=empty_color;

    x1++;
   }
   
  
   lines_cleared++;
  }

 }


 lines_cleared_total+=lines_cleared;

 /*printf("this line clear: %d\n",lines_cleared);
 printf("total lines cleared: %d\n",lines_cleared_total);*/

 /*scoring section*/
 if(lines_cleared==1)
 {
  if(last_move_spin==1)
  {
   if(back_to_back==1){score+=1200;}
   else{score+=800;back_to_back=1;}
  }
  else
  {
   score+=100;back_to_back=0;
  }
 }
 if(lines_cleared==2)
 {
  if(last_move_spin==1)
  {
   if(back_to_back==1){score+=1800;}
   else{score+=1200;back_to_back=1;}
  }
  else
  {
   score+=300;back_to_back=0;
  }
 }
 if(lines_cleared==3){score+=500;back_to_back=0;}
 if(lines_cleared==4)
 {
  if(back_to_back==1){score+=1200;}
  else{score+=800;back_to_back=1;}
 }

 if(lines_cleared!=0)
 {
  lines_cleared_last=lines_cleared;
 }

}


/*lines fall down to previously cleared line spaces*/

void tetris_fall_lines()
{
 int x,y,xcount,y1;

/* printf("Time to make lines fall\n");*/

 y=grid_height;
 while(y>0)
 {
  y-=1;

  xcount=0;
  x=0;
  while(x<grid_width)
  {
   if(tetris_grid[x+y*grid_width]!=empty_color){xcount++;}
   x+=1;
  }

  /*printf("row %d xcount %d\n",y,xcount);*/

  if(xcount==0)
  {
/*   printf("row %d is empty\n",y);*/

   /*find first non empty row above empty row*/

   y1=y;
   while(y1>0)
   {
    y1--;
    xcount=0;
    x=0;
    while(x<grid_width)
    {
     if(tetris_grid[x+y1*grid_width]!=empty_color){xcount++;}
     x+=1;
    }
    if(xcount>0)
    {
/*     printf("row %d is not empty. Will copy to row %d.\n",y1,y);*/

     x=0;

     x=0;
     while(x<grid_width)
     {
      tetris_grid[x+y*grid_width]=tetris_grid[x+y1*grid_width];
      tetris_grid[x+y1*grid_width]=empty_color;
      x++;
     }
     break;
    }
   }

  }

 }

}

/*this function controls whether or not the block index changes.*/
void tetris_next_block()
{
 /*optionally increment block type for different block next time.*/
 block_type++;  block_type%=blocks_used;
}


void tetris_set_block()
{
 int x,y;


  /*draw block onto grid at it's current location*/
  y=0;
  while(y<4)
  {
   x=0;
   while(x<4)
   {
    if(main_block_array[x+y*4]!=0)
    {
      tetris_grid[block_x+x+(block_y+y)*grid_width]=block_color; 
    }
    x+=1;
   }
   y+=1;
  }



 tetris_clear_lines();

 if(lines_cleared_last>0){tetris_fall_lines();}


 tetris_next_block();
 spawn_block();


}

/*all things about moving down*/
void tetris_move_down()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_y+=1;
 if(tetris_check_move()!=0)
 {
  /*printf("Block is finished\n");*/
  tetris_set_block();
  moves++; /*moves normally wouldn't be incremented because move check fails but setting a block is actually a valid move.*/
 }
 else
 {
  last_move_spin=0;
 }

}


/*all things about moving up*/
void tetris_move_up()
{
 /*make backup of block location*/
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_y-=1;
 if(tetris_check_move()==0)
 {
  last_move_spin=1;
 }
;
}


/*all things about moving right*/
void tetris_move_right()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_x+=1;
 if(tetris_check_move()==0)
 {
  last_move_spin=1;
 }
}

/*all things about moving left*/
void tetris_move_left()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_x-=1;
 if(tetris_check_move()==0)
 {
  last_move_spin=1;
 }
}



void block_rotate_right()
{
 uint32_t x=0,y=0,x1=0,y1=0;

/*make backup of block location*/
 block_x1=block_x,block_y1=block_y;

 /*first backup block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_backup[x+y*block_width]=main_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/

 x1=current_block_width;
 y=0;
 while(y<current_block_width)
 {
  x1--;
  y1=0;
  x=0;
  while(x<current_block_width)
  {
   main_block_array[x1+y1*block_width]=block_array_backup[x+y*block_width];
   x+=1;
   y1++;
  }
  y+=1;
 }

 /*if rotation caused collision, restore to the backup before rotate.*/
 if(tetris_check_move()!=0)
 {
 
  y=0;
  while(y<block_width)
  {
   x=0;
   while(x<block_width)
   {
    main_block_array[x+y*block_width]=block_array_backup[x+y*block_width];
    x+=1;
   }
   y+=1;
  }
  
 }
 else
 {
  last_move_spin=1;
 }

}



void block_rotate_left()
{
 uint32_t x=0,y=0,x1=0,y1=0;

 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;

 /*first backup block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_backup[x+y*block_width]=main_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/

 x1=current_block_width;
 y=0;
 while(y<current_block_width)
 {
  x1--;
  y1=0;
  x=0;
  while(x<current_block_width)
  {
   main_block_array[x+y*block_width]=block_array_backup[x1+y1*block_width];
   x+=1;
   y1++;
  }
  y+=1;
 }

 /*if rotation caused collision, restore to the backup before rotate.*/
 if(tetris_check_move()!=0)
 {
 
  y=0;
  while(y<block_width)
  {
   x=0;
   while(x<block_width)
   {
    main_block_array[x+y*block_width]=block_array_backup[x+y*block_width];
    x+=1;
   }
   y+=1;
  }
 }
 else
 {
  last_move_spin=1;
 }


}



void block_hold()
{
 uint32_t x=0,y=0;



/*printf("hold block width: %d\n",hold_block_width);*/

if(hold_used==0) /*just store block if nothing there*/
{

 /*printf("hold block used first time.\n");*/

 hold_block_width=current_block_width;
 hold_block_color=block_color;
 hold_block_id=current_block_id;

 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_hold[x+y*block_width]=main_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 tetris_next_block();
 spawn_block();

 hold_used=1;

}

else
{

 /*printf("Swap with previous hold block.\n");*/

 hold1_block_width=hold_block_width; hold1_block_color=hold_block_color; hold1_block_id=hold_block_id; /*put the hold block into temp storage*/
 hold_block_width=current_block_width; hold_block_color=block_color; hold_block_id=current_block_id;      /*place current block into the hold spot*/
 current_block_width=hold1_block_width; block_color=hold1_block_color; current_block_id=hold1_block_id;   /*make the current block be the block that was held last time*/


 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_hold1[x+y*block_width]=block_array_hold[x+y*block_width];
   block_array_hold[x+y*block_width]=main_block_array[x+y*block_width];
   main_block_array[x+y*block_width]=block_array_hold1[x+y*block_width];
   x+=1;
  }

  y+=1;
 }

}
}





uint32_t saved_tetris_grid[tetris_array_size];
int saved_moves; /*number of valid moves*/
int saved_frame;  /*current animation frame*/
int last_move_spin; /*was the last move a t spin?*/
int back_to_back; /*back to back score bonus*/

int saved_block_type;

int saved_block_array[16],saved_main_block_width,saved_block_color,saved_block_id,saved_block_x,saved_block_y; /*to store all details of main block*/

int saved_hold_block_array[16],saved_hold_block_width,saved_hold_block_color,saved_hold_block_id,saved_hold_block_x,saved_hold_block_y; /*to store all details of main block*/
int saved_hold_used;

int saved_score;
int saved_lines_cleared_total;

int save_exist=0;

/*
 a special function which saves all the importantdata in the game. This allows reloading to a previous position when I make a mistake.
*/
void tetris_save_state()
{
 int x,y;

 /*make backup of entire grid*/
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   saved_tetris_grid[x+y*grid_width]=tetris_grid[x+y*grid_width];
   x+=1;
  }
  y+=1;
 }

 /*all attributes of main block*/
 saved_block_type=block_type;
 saved_main_block_width=current_block_width;
 saved_block_color=block_color;
 saved_block_id=current_block_id;
 saved_block_x=block_x;
 saved_block_y=block_y;

 /*backup main block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   saved_block_array[x+y*block_width]=main_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }


 /*all attributes of hold block as well*/
 saved_hold_block_width=hold_block_width;
 saved_hold_block_color=hold_block_color;
 saved_hold_block_id=hold_block_id;

 /*backup hold block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   saved_hold_block_array[x+y*block_width]=block_array_hold[x+y*block_width];
   x+=1;
  }
  y+=1;
 }


 saved_moves=moves;
 saved_frame=frame;
 saved_hold_used=hold_used;
 saved_score=score;
 saved_lines_cleared_total=lines_cleared_total;


 printf("Game Saved at move %d\n",moves);
 save_exist=1;
}


/*
 a special function which loads the data previously saved. This allows reloading to a previous position when I make a mistake.
*/
void tetris_load_state()
{
 int x,y;


if(save_exist==0)
{
 printf("No save exists yet.\n");
 return;
}

 /*restore backup of entire grid*/
 y=0;
 while(y<grid_height)
 {
  x=0;
  while(x<grid_width)
  {
   tetris_grid[x+y*grid_width]=saved_tetris_grid[x+y*grid_width];
   x+=1;
  }
  y+=1;
 }

 /*all attributes of main block*/
 block_type=saved_block_type;
 current_block_width=saved_main_block_width;
 block_color=saved_block_color;
 current_block_id=saved_block_id;
 block_x=saved_block_x;
 block_y=saved_block_y;

 /*restore backup of main block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   main_block_array[x+y*block_width]=saved_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }


 /*all attributes of hold block as well*/
 hold_block_width=saved_hold_block_width;
 hold_block_color=saved_hold_block_color;
 hold_block_id=saved_hold_block_id;

 /*restore backup of hold block grid*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_hold[x+y*block_width]=saved_hold_block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 moves=saved_moves;
 frame=saved_frame;
 hold_used=saved_hold_used;
 score=saved_score;
 lines_cleared_total=saved_lines_cleared_total;


 printf("Game Loaded at move %d\n",moves);

}




