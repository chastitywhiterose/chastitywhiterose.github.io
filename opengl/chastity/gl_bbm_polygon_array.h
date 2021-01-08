/*
Normally I use the legacy fixed function pipeline for drawing my polygons such as the following style

glBegin(GL_POLYGON);
glVertex2f(x,y);
glEnd();

This works fine for me however I have read there are other ways of achieving the same thing that are faster. This file is my test of these methods.
*/


/*
 The next function works quite differently from my other functions in "gl_bbm_polygon.h" .
 Instead of storing the x and y coordinates in two separate arrays, they are all stored in 1 array together.
 This prepares an array that glDrawArrays can later use.
*/

GLfloat polygon_vertex_array[0x1000];

void get_polygon_points_array()
{
 double angle;
 int i=0,i1=0;
 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[i1]=polygon_cx+sin(angle)*polygon_radius; i1++;
  polygon_vertex_array[i1]=polygon_cy-cos(angle)*polygon_radius; i1++;
  i++;
 }
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,polygon_vertex_array);
}

/*draws a filled polygon using the array created with the above function.*/
void gl_bbm_polygon_array()
{
 get_polygon_points_array();
 glDrawArrays(GL_POLYGON,0,polygon_sides);
}

/*using same array,only the lines are drawn. The polygon is not filled.*/
void gl_bbm_polygon_array1()
{
 get_polygon_points_array();
 glDrawArrays(GL_LINE_LOOP,0,polygon_sides);
}

/*
 Drawing a filled star polygon is actually a lot more work than you might expect.
 This is a modified form of my points getting function that fills up an array with enough points for multiple triangles.
 The center x and y of the polygon are used as a corner of each triangle as well as two of the corners based on the step variable.
*/
void get_polygon_points_array_star()
{
 double angle;
 int i=0,i1=0,ai=0;

 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;

  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=polygon_cx+sin(angle)*polygon_radius; ai++;
  polygon_vertex_array[ai]=polygon_cy-cos(angle)*polygon_radius; ai++;

  angle=2*PI*i1/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=polygon_cx+sin(angle)*polygon_radius; ai++;
  polygon_vertex_array[ai]=polygon_cy-cos(angle)*polygon_radius; ai++;
  
  polygon_vertex_array[ai]=polygon_cx; ai++;
  polygon_vertex_array[ai]=polygon_cy; ai++;
  i++;
 }
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,polygon_vertex_array);
}

/*uses the array from above function to draw the filled star polygon*/
void gl_bbm_polygon_array2()
{
 get_polygon_points_array_star();
 glDrawArrays(GL_TRIANGLES,0,polygon_sides*3);
}

/*
 This one only fills the array with the points for drawing the lines of a star polygon. 
*/
void get_polygon_points_array_star_lines()
{
 double angle,x,y;
 int i=0,i1=0,ai=0;

 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;

  angle=2*PI*i/polygon_sides+polygon_radians;
  x=polygon_cx+sin(angle)*polygon_radius;
  y=polygon_cy-cos(angle)*polygon_radius;
  polygon_vertex_array[ai]=x; ai++;
  polygon_vertex_array[ai]=y; ai++;

  angle=2*PI*i1/polygon_sides+polygon_radians;
  x=polygon_cx+sin(angle)*polygon_radius;
  y=polygon_cy-cos(angle)*polygon_radius;
  polygon_vertex_array[ai]=x; ai++;
  polygon_vertex_array[ai]=y; ai++;
  
  i++;
 }
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,polygon_vertex_array);
}

/*
 star polyline. Each two vertices from array is a line.
*/
void gl_bbm_polygon_array3()
{
 get_polygon_points_array_star_lines();
 glDrawArrays(GL_LINES,0,polygon_sides*2);
}

/*set up vertices between center and corners for next function*/
void get_polygon_points_array_centerlines()
{
 double angle;
 int i=0,ai=0;

 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=polygon_cx+sin(angle)*polygon_radius; ai++;
  polygon_vertex_array[ai]=polygon_cy-cos(angle)*polygon_radius; ai++;
  
  polygon_vertex_array[ai]=polygon_cx; ai++;
  polygon_vertex_array[ai]=polygon_cy; ai++;
  i++;
 }
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2,GL_FLOAT,0,polygon_vertex_array);
}


/*only the lines are drawn. The polygon is not filled.*/
void gl_bbm_polygon_array4()
{
 get_polygon_points_array_centerlines();
 glDrawArrays(GL_LINES,0,polygon_sides*2);
}

/*only the points of corners are drawn.*/
void gl_bbm_polygon_array5()
{
 get_polygon_points_array();
 glDrawArrays(GL_POINTS,0,polygon_sides);
}

