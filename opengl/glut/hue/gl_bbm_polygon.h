/*
This is a part of my code for making convex regular polygons. It has been copied and modified from my original BBM library. This is also not the same version of this file that was used in my original published opengl spinning polygon program from September 2020. That program was written using a lot of complex features not required for basic animations. This is the new cleaned up version for my website. This file will be downloadable for everyone and so it required update comments. Consider it as 2021 and beyond.
*/


/*
 this is a copy of the above function for getting the points
 except condensed to be slightly smaller. It's hard to read though.
*/
void get_polygon_points()
{
 double angle;
 int i=0;
 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians;
  polygon_xpoints[i]=polygon_cx+sin(angle)*polygon_radius;
  polygon_ypoints[i]=polygon_cy-cos(angle)*polygon_radius;
  i++;
 }
}

/*a function pointer that points to whichever function I currently use to get the points
 for the next polygon drawn. This is for conveniently testing new functions.
*/
void (*poly_points_func)()=get_polygon_points;


/*
polygon

The polygon is filled. This is the reason I learned OpenGL. SDL lacks this feature!
*/
void gl_bbm_polygon()
{
 int i;
 poly_points_func();
 glBegin(GL_POLYGON);
 i=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  i+=1;
 }
 glEnd();
}

/*
I finally learned how to use a function pointer!
This function pointer allows me to change which function is currently being called each frame by various keypresses while the program is running. It defaults to the filled polygon function above but can be changed to anything else!
*/
void (*polyfunc)()=gl_bbm_polygon;



/*
polyline

only the lines are drawn. The polygon is not filled.
*/
void gl_bbm_polygon1()
{
 int i;
 poly_points_func();
 glBegin(GL_LINE_LOOP);
 i=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  i+=1;
 }
 glEnd();
}


/*
filled star polygon
*/
void gl_bbm_polygon2()
{
 int i,i1;
 poly_points_func();
 glBegin(GL_TRIANGLES);
 i=0; i1=0;
 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  glVertex2f(polygon_xpoints[i1],polygon_ypoints[i1]);
  glVertex2f(polygon_cx,polygon_cy);
  i+=1;
 }
 glEnd();
}


/*
star polyline

only the lines are drawn. The polygon is not filled.
*/
void gl_bbm_polygon3()
{
 int i,i1,i2;
 poly_points_func();
 glBegin(GL_LINE_LOOP);
 i=0; i1=0;i2=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i1],polygon_ypoints[i1]);
  /*printf("i1==%d\n",i1);*/
  i1=(i1+polygon_step)%polygon_sides;
  if(i1==i2){i2++;i1=i2;glEnd();glBegin(GL_LINE_LOOP);}
  i+=1;
 }
 glEnd();
}

/*
lines are drawn from the center to the corners but not from corner to corner.
*/
void gl_bbm_polygon4()
{
 int i;
 poly_points_func();
 glBegin(GL_LINES);
 i=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  glVertex2f(polygon_cx,polygon_cy);
  i+=1;
 }
 glEnd();
}

/*
Only the points of the corners are drawn.
*/
void gl_bbm_polygon5()
{
 int i;
 poly_points_func();
 glBegin(GL_POINTS);
 i=0;
 while(i<polygon_sides)
 {
  glVertex2f(polygon_xpoints[i],polygon_ypoints[i]);
  i+=1;
 }
glEnd();
}




/*
 opengl version of the checkerboard function
 uses global width and height of the window and the rectsize global defined below
 although this is a rather efficient checkerboard drawing function, it does noticably slow the polygon down compared to when the polygon is the only thing being drawn. Until I learn fast blitting methods this may be an issue. But it's still a hundred times faster than my previous SDL methods even so.
*/
int rectsize=16;
void gl_bbm_checker()
{
 int x,y,index,index1;
 float v[4];

 /*store current drawing color in here*/
 glGetFloatv(GL_CURRENT_COLOR,v);

 index=0;
 y=0;
 while(y<height)
 {
  index1=index;
  x=0;
  while(x<width)
  {
   if(index==0){glColor3f(0.0f, 0.0f, 0.0f);}
   if(index==1){glColor3f(1.0f, 1.0f, 1.0f);}
   glRecti(x,y,x+rectsize,y+rectsize);
   index^=1;
   x+=rectsize;
  }
  index=index1^1;
  y+=rectsize;
 }

 /*restore original drawing color from here*/
 glColor4fv(v);

}



/*
This header file is part of my OpenGL Polygon program. The following is a copy of the makefile I usually use when compiling it. Because it uses only my own code and OpenGL functions, it can be used with either freeglut or glfw.

I use this makefile for freeglut.

linux_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL -lGLU -lglut -lm &&./main
win_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL32 -lglu32 -lfreeglut -lm && ./main
win_opengl_static:
	gcc -Wall -ansi -pedantic main.c -o main -D FREEGLUT_STATIC -lfreeglut_static -lwinmm -lgdi32 -lOpenGL32 -lglu32 -lm && ./main

And this one for glfw

linux_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw -lOpenGL -lm && ./main
win_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw3 -lOpenGL32 -lm && ./main
win_opengl_static:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw3 -lOpenGL32 -lgdi32 -lm -static && ./main
	strip main.exe


In both cases the first rule is the flags for compiling on Linux and the others are for dynamic and static linking on Windows under my msys installation. OpenGL has changed the way I do graphics forever.

These references are valuable when I need to look up a function.

https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/
http://freeglut.sourceforge.net/docs/api.php
https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
https://www.khronos.org/opengl/wiki/OpenGL_Type
https://docs.gl/gl2/glBegin
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glReadPixels.xml
https://github.com/markkilgard/glut
*/
