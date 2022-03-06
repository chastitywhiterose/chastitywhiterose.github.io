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
    if(block_array[bx+by*4]!=0)
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
 if(lines_cleared==1){score+=100;}
 if(lines_cleared==2){score+=300;}
 if(lines_cleared==3){score+=500;}
 if(lines_cleared==4)
 {
  if(lines_cleared_last==4){score+=1200;}
  else{score+=800;}
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
 /*block_type++;  block_type%=7;*/
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
    if(block_array[x+y*4]!=0)
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


/*all things about moving right*/
void tetris_move_right()
{
 /*make backup of block location*/
 block_x1=block_x,block_y1=block_y;
 block_x+=1;
 tetris_check_move();
}

/*all things about moving left*/
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
 while(y<current_block_width)
 {
  x=0;
  while(x<current_block_width)
  {
   block_array_backup[x+y*block_width]=block_array[x+y*block_width];
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
 while(y<current_block_width)
 {
  x=0;
  while(x<current_block_width)
  {
   block_array_backup[x+y*block_width]=block_array[x+y*block_width];
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



void block_hold()
{
 uint32_t x=0,y=0;



/*printf("hold block width: %d\n",hold_block_width);*/

if(hold_used==0) /*just store block if nothing there*/
{

 printf("hold block used first time.\n");

 hold_block_width=current_block_width;
 hold_block_color=block_color;
 hold_block_id=current_block_id;

 y=0;
 while(y<block_width)
 {
  x=0;
  while(x<block_width)
  {
   block_array_hold[x+y*block_width]=block_array[x+y*block_width];
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

 printf("Swap with previous hold block.\n");

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
   block_array_hold[x+y*block_width]=block_array[x+y*block_width];
   block_array[x+y*block_width]=block_array_hold1[x+y*block_width];
   x+=1;
  }

  y+=1;
 }

}


 
}


