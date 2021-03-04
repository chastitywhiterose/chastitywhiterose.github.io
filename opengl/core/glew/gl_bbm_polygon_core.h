/*
 A version of my polygon functions that work only with core version 3.3 of OpenGL.
 This version does not even have functions like glBegin,glEnd,glVertex2f, etc.
This file is specifically only designed to be used in a program which has very specific shaders and the VAO and VBO set up in just a certain way. Each vertex is defined by 5 floats. The first two are for the X and Y coordinates and the next three are the red,green,and blue color values for that coordinate.
I don't understand all of the code and will go back to https://open.gl/ to learn more about how it works.
*/

/*array to store the data in containing coordinates and colors.*/
GLfloat polygon_vertex_array[0x1000];


/*
The following function creates the coordinates for the polygons but converts them to the normalized coordinates that opengl expects. The original opengl programs assumed an orthographic matrix such that the top left corner was X=0,Y=0 and the bottom right corner was X=width,Y=height. Modern OpenGL doesn't have this luxury since the "glOrtho" function from legacy OpenGL was removed from the API entirely. Instead I am left with the defaults that all coordinates are within the range -1 to 1. This also puts the center of the window as 0,0. This invalidated all my legacy OpenGL code but I came up with a workaround for this. If we divide the orthographic coordinates by the width and height we get the normalized coordinates. But adjusting the center was done but subtracting 0.5 from both X and Y.
*/
void get_polygon_points_core()
{
 double angle;
 int i=0,i1=0;
 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[i1]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; i1++;
  polygon_vertex_array[i1]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; i1++;
  polygon_vertex_array[i1]=color_r; i1++; /*red float*/
  polygon_vertex_array[i1]=color_g; i1++; /*green float*/
  polygon_vertex_array[i1]=color_b; i1++; /*blue float*/
  i++;
 }
}


/*
this function does help get a regular polygon on the screen except that it may be stretched depending on the width to height ratio of the window.
this was not intended behavior but it was the start of my experience with modern opengl and so I will preserve it because it worked for drawing something.
*/

void get_polygon_points_core1()
{
 double angle;
 int i=0,i1=0;
 while(i<polygon_sides)
 {
  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[i1]=/*polygon_cx+*/sin(angle)/**polygon_radius*/; i1++;
  polygon_vertex_array[i1]=/*polygon_cy-*/cos(angle)/**polygon_radius*/; i1++;
  polygon_vertex_array[i1]=color_r; i1++; /*red float*/
  polygon_vertex_array[i1]=color_g; i1++; /*green float*/
  polygon_vertex_array[i1]=color_b; i1++; /*blue float*/
  i++;
 }
}

/*filled regular polygon based on all the global variables*/
void gl_polygon()
{
 get_polygon_points_core();
 glBufferData(GL_ARRAY_BUFFER, sizeof(polygon_vertex_array), polygon_vertex_array, GL_DYNAMIC_DRAW);
 glDrawArrays(GL_TRIANGLE_FAN,0,polygon_sides);
}

/*lines only, polygon is not filled*/
void gl_polygon1()
{
 get_polygon_points_core();
 glBufferData(GL_ARRAY_BUFFER, sizeof(polygon_vertex_array), polygon_vertex_array, GL_DYNAMIC_DRAW);
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
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=color_r; ai++;
  polygon_vertex_array[ai]=color_g; ai++;
  polygon_vertex_array[ai]=color_b; ai++;

  angle=2*PI*i1/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=color_r; ai++;
  polygon_vertex_array[ai]=color_g; ai++;
  polygon_vertex_array[ai]=color_b; ai++;
  
  polygon_vertex_array[ai]=polygon_cx/width-0.5; ai++;
  polygon_vertex_array[ai]=polygon_cy/height-0.5; ai++;
  polygon_vertex_array[ai]=color_r; ai++;
  polygon_vertex_array[ai]=color_g; ai++;
  polygon_vertex_array[ai]=color_b; ai++;
  i++;
 }
}

/*same but use red,green,blue for the 3 corners of each triangle*/
void get_polygon_points_array_star_rgb()
{
 double angle;
 int i=0,i1=0,ai=0;

 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;

  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=0; ai++;

  angle=2*PI*i1/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=0; ai++;
  
  polygon_vertex_array[ai]=polygon_cx/width-0.5; ai++;
  polygon_vertex_array[ai]=polygon_cy/height-0.5; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=1; ai++;
  i++;
 }
}

/*same but use cyan,magenta,yellow for the 3 corners of each triangle*/
void get_polygon_points_array_star_cmy()
{
 double angle;
 int i=0,i1=0,ai=0;

 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;

  angle=2*PI*i/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=1; ai++;

  angle=2*PI*i1/polygon_sides+polygon_radians; 
  polygon_vertex_array[ai]=(polygon_cx+sin(angle)*polygon_radius*2)/width-0.5; ai++;
  polygon_vertex_array[ai]=(polygon_cy+cos(angle)*polygon_radius*2)/height-0.5; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=0; ai++;
  polygon_vertex_array[ai]=1; ai++;
  
  polygon_vertex_array[ai]=polygon_cx/width-0.5; ai++;
  polygon_vertex_array[ai]=polygon_cy/height-0.5; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=1; ai++;
  polygon_vertex_array[ai]=0; ai++;
  i++;
 }
}

void (*get_polygon_points_array_star_func)()=get_polygon_points_array_star;

/*
filled star polygon. Uses the setup function above.
*/
void gl_polygon2()
{
 get_polygon_points_array_star_func();
 glBufferData(GL_ARRAY_BUFFER, sizeof(polygon_vertex_array), polygon_vertex_array, GL_DYNAMIC_DRAW);
 glDrawArrays(GL_TRIANGLES,0,polygon_sides*3);
}

/*
uses GL_LINE_LOOP to draw the lines of the many triangles but not fill them.
*/
void gl_polygon3()
{
 get_polygon_points_array_star();
 glBufferData(GL_ARRAY_BUFFER, sizeof(polygon_vertex_array), polygon_vertex_array, GL_DYNAMIC_DRAW);
 glDrawArrays(GL_LINE_LOOP,0,polygon_sides*3);
}

/*
I finally learned how to use a function pointer!
This function pointer allows me to change which function is currently being called each frame by various keypresses while the program is running. It defaults to the filled polygon function above but can be changed to anything else!
*/
void (*polyfunc)()=gl_polygon2;
