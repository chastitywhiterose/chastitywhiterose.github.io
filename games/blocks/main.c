/*this file is my attempt at forming structures to represent the various types of blocks in tetris games*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct block
{
char  title[50];
char  id;
int a[4][4][4];
};


void zero_block(struct block *b)
{
 int x,y,z;

 z=0;
 while(z<4)
 {
  y=0;
  while(y<4)
  {
   x=0;
   while(x<4)
   {
    b->a[x][y][z]=0;
    x+=1;
   }
   y+=1;
  }
  z+=1;
 }

}

void print_block(struct block b)
{
 int x,y,z;

  printf("name: %s\n",b.title);

  /*printf("id: %c\n",b.id);*/


 z=0;
 while(z<4)
{
 y=0;
 while(y<4)
 {
  x=0;
  while(x<4)
  {
   printf("%d ",b.a[x][y][z]);
   x+=1;
  }

   printf("\n");
  y+=1;
 }
  printf("\n");
  z+=1;
 }

}


int main(int argc, char **argv)
{

 struct block t,l,s;
 
 printf("LOL, Tetris block info.\n");

 strcpy( t.title, "The T Block");
 t.id='t';

 zero_block(&t);

 t.a[1][0][0]=1;
 t.a[0][1][0]=1;
 t.a[1][1][0]=1;
 t.a[2][1][0]=1;

 t.a[1][0][1]=1;
 t.a[1][1][1]=1;
 t.a[2][1][1]=1;
 t.a[1][2][1]=1;

 t.a[0][1][2]=1;
 t.a[1][1][2]=1;
 t.a[2][1][2]=1;
 t.a[1][2][2]=1;

 t.a[1][0][3]=1;
 t.a[0][1][3]=1;
 t.a[1][1][3]=1;
 t.a[1][2][3]=1;



 strcpy( l.title, "The L Block");
 l.id='l';

 zero_block(&l);

 l.a[0][0][0]=1;
 l.a[0][1][0]=1;
 l.a[1][1][0]=1;
 l.a[2][1][0]=1;


 l.a[1][0][1]=1;
 l.a[2][0][1]=1;
 l.a[1][1][1]=1;
 l.a[1][2][1]=1;

 l.a[2][2][2]=1;
 l.a[0][1][2]=1;
 l.a[1][1][2]=1;
 l.a[2][1][2]=1;

 l.a[1][0][3]=1;
 l.a[0][2][3]=1;
 l.a[1][1][3]=1;
 l.a[1][2][3]=1;


 strcpy( s.title, "The S Block");
 s.id='s';
 zero_block(&s);

 s.a[0][0][0]=1;
 s.a[1][0][0]=1;
 s.a[1][1][0]=1;
 s.a[2][1][0]=1;

 s.a[1][0][1]=1;
 s.a[0][1][1]=1;
 s.a[1][1][1]=1;
 s.a[0][2][1]=1;

 s.a[0][0][2]=1;
 s.a[1][0][2]=1;
 s.a[1][1][2]=1;
 s.a[2][1][2]=1;

 s.a[1][0][3]=1;
 s.a[0][1][3]=1;
 s.a[1][1][3]=1;
 s.a[0][2][3]=1;


 print_block(t);
 print_block(l);
 print_block(s);

 
 return 0;
}
