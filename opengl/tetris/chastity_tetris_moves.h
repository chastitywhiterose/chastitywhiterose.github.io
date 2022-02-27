/*
Functions for each operation that moves the current block.
*/



int pixel_on_grid(int x,int y)
{
 if(x<0){printf("Error: Negative X\n");return 1;}
 if(y<0){printf("Error: Negative Y\n");return 1;}
 if(x>=grid_width){printf("Error: X too high.\n");return 1;}
 if(y>=grid_height){printf("Error: Y too high.\n");return 1;}
 else{return tetris_grid[block_x+bx+(block_y+by)*grid_width];}
}

/*checks whether or not the block collides with anything on the current field*/
int tetris_check_move()
{
 int x,y; 

  by=0;
  while(by<4)
  {
   bx=0;
   while(bx<4)
   {
    if(block_array[bx+by*4]!=0)
    {

     if( pixel_on_grid(block_x+bx,block_y+by)!=0 )
     {
      printf("Error: Block in Way on Move Check.\n");

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

  printf("row %d xcount %d\n",y,xcount);

  if(xcount==16)
  {
 
   y1=y;

   
   x1=0;
   while(x1<grid_width)
   {

    printf("row %d line clear attempt.\n",y);

    tetris_grid[x1+y1*grid_width]=empty_color;

/*the following lines should not be used here. maybe another function*/
/*
    tetris_grid[x1+y1*grid_width]=tetris_grid[x1+(y1-1)*grid_width];
    tetris_grid[x1+(y1-1)*grid_width]=empty_color;
*/

    x1++;
   }
   
  

  }

 }

}



void tetris_set_block()
{
 int x,y;


  /*draw block onto grid at it's current location*/
  by=0;
  while(by<4)
  {
   bx=0;
   while(bx<4)
   {
    if(block_array[bx+by*4]!=0)
    {
      tetris_grid[block_x+bx+(block_y+by)*grid_width]=block_color; 
    }
    bx+=1;
   }
   by+=1;
  }




 /*copy another new block array into the current one*/
 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array[x+y*block_width]=block_array_i[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 /*set the initial position of the next block*/ 
 block_x=next_block_x;
 block_y=next_block_y;

 tetris_clear_lines();

}

/*all things about moving down*/
void tetris_move_down()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_y+=1;
 if(tetris_check_move()!=0){ printf("Block is finished\n");tetris_set_block();}
 /*score+=1;*/
}


/*all things about moving up*/
void tetris_move_up()
{
 /*make backup of block location*/
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_y-=1;
 tetris_check_move();
}


/*all things about moving up*/
void tetris_move_right()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_x+=1;
 tetris_check_move();
}

/*all things about moving up*/
void tetris_move_left()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_x-=1;
 tetris_check_move();
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
   block_array_backup[x+y*block_width]=block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/

 x1=block_width;
 y=0;
 while(y<block_width)
 {
  x1--;
  y1=0;
  x=0;
  while(x<block_width)
  {
   block_array[x1+y1*block_width]=block_array_backup[x+y*block_width];
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
    block_array[x+y*block_width]=block_array_backup[x+y*block_width];
    x+=1;
   }
   y+=1;
  }

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
   block_array_backup[x+y*block_width]=block_array[x+y*block_width];
   x+=1;
  }
  y+=1;
 }

 /*copy it from top to bottom to right to left(my own genius rotation trick)*/

 x1=block_width;
 y=0;
 while(y<block_width)
 {
  x1--;
  y1=0;
  x=0;
  while(x<block_width)
  {
   block_array[x+y*block_width]=block_array_backup[x1+y1*block_width];
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
    block_array[x+y*block_width]=block_array_backup[x+y*block_width];
    x+=1;
   }
   y+=1;
  }
 }

}



