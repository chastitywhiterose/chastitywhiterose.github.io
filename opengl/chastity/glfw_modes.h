/*
This file contains the various different modes of operations of my program. The idea is that I can have several different sub programs which operate differently depending on which key I press.
*/

/*
This is the starting mode that displays information about a spinning polygon.
*/
void mode_f1()
{
 /*set size and position of polygon*/
 polygon_cx=width/2;
 polygon_cy=height/2;
 polygon_radius=height/3;
  /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 /*This allows the XOR operation when drawing the polygon. However it's important to note that in the loop below GL_COLOR_LOGIC_OP is disabled when drawing text but then enabled before drawing the polygon. The text does not draw at all when logical operations are enabled.*/
 glLogicOp(GL_XOR);
 
 framelimit=fps*60*60;

  glColor3f(1.0,1.0,1.0); /*color of the polygon*/
 
  /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

  if(text_switch)
  {
   glDisable(GL_COLOR_LOGIC_OP);
   glRasterPos2i(fontsize,fontsize);
   sprintf(text,"%d",frame_displayed);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(width-fontsize*5,fontsize);
   sprintf(text,"%d:%02d:%02d",hours,minutes,seconds);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(fontsize,fontsize*3);
   sprintf(text,"%d",polygon_sides);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);
 
   glRasterPos2i(fontsize,fontsize*4);
   sprintf(text,"%d",polygon_step);
   ftglRenderFont(font,text,FTGL_RENDER_ALL);


 glRasterPos2i(width*5/16,fontsize);
 ftglRenderFont(font,"OpenGL Polygons",FTGL_RENDER_ALL);

 glRasterPos2i(fontsize*2,height-fontsize);
 ftglRenderFont(font,"An animation by Chastity White Rose!", FTGL_RENDER_ALL);
 
 glEnable(GL_COLOR_LOGIC_OP);
 }
 
 polyfunc();
 
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
 
 if(frame_displayed%360==0)
 {
  /*
   This section increments the step of the star polygon. If the step would result in an invalid star polygon being drawn, the step is reset to 1 and the number of points incremented. The genius of this is that it will cycle through every possible regular polygon both convex and star!
  */
  
  polygon_step+=1;
  if(polygon_step>=(polygon_sides/2)+polygon_sides%2)
  {
   polygon_sides+=1;
   polygon_step=1;
  }

 }
 
  
  polygon_radians+=PI/180;
  
  /*check for keypresses or other events*/
  glfwPollEvents();
 
  /*
  glfwseconds=glfwGetTime();
  glfwseconds1=glfwseconds+1.0/fps;
  while(glfwseconds<glfwseconds1)
  {
   glfwseconds=glfwGetTime();
  }
  */
  
 }
 
}

/*
This is checkerboard mode. It draws a checkerboard before the polygon every frame but no text.
*/
void mode_f2()
{
 rectsize=80;
 /*set size and position of polygon*/
 polygon_cx=width/2;
 polygon_cy=height/2;
 polygon_radius=height/2;
  /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 /*This allows the XOR operation when drawing the polygon. However it's important to note that in the loop below GL_COLOR_LOGIC_OP is disabled when drawing text but then enabled before drawing the polygon. The text does not draw at all when logical operations are enabled.*/
 glLogicOp(GL_XOR);
 
 framelimit=fps*60*60;

 glEnable(GL_COLOR_LOGIC_OP);
 
  glColor3f(1.0,1.0,1.0); /*color of the polygon*/
 
  /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

 /*gl_bbm_checker();*/
 polyfunc();
 
 glFlush();
  

 /*optionally save frame as file*/
 if(0)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
 
 
 /*
  This section increments the step of the star polygon. If the step would result in an invalid star polygon being drawn, the step is reset to 1 and the number of points incremented. The genius of this is that it will cycle through every possible regular polygon both convex and star!
 */
 
 /*
 frame_displayed++;
 if(frame_displayed%360==0)
 {

  polygon_step+=1;
  if(polygon_step>=(polygon_sides/2)+polygon_sides%2)
  {
   polygon_sides+=1;
   polygon_step=1;
  }
 }
 */
 
  
  polygon_radians+=PI/180;
  
  /*check for keypresses or other events*/
  glfwPollEvents();
 
  /*
  glfwseconds=glfwGetTime();
  glfwseconds1=glfwseconds+1.0/fps;
  while(glfwseconds<glfwseconds1)
  {
   glfwseconds=glfwGetTime();
  }
  */
  
 }
 
}



void mode_f3()
{
 rectsize=80;
 /*set size and position of polygon*/
 polygon_cx=width/2;
 polygon_cy=height/2;
 polygon_radius=height/2;
  /*set the current polygon function to filled star*/
 polyfunc=gl_bbm_polygon2;
 
 /*This allows the XOR operation when drawing the polygon. However it's important to note that in the loop below GL_COLOR_LOGIC_OP is disabled when drawing text but then enabled before drawing the polygon. The text does not draw at all when logical operations are enabled.*/
 glLogicOp(GL_XOR);
 
 glEnable(GL_COLOR_LOGIC_OP);
 
 polygon_sides=3;
 
 framelimit=fps*60;
 
  /* Loop until the user closes the window */
 while(!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

 /*gl_bbm_checker();*/
 /*polyfunc();*/
 
 /*gl_regular_triangle();*/
 gl_regular_triangle_array();
 
 glFlush();
  

 /*optionally save frame as file*/
 if(0)
 {
  gl_bbm_save_frame(); if(frame==framelimit){glfwSetWindowShouldClose(window,GLFW_TRUE);}
 }
 
 frame_displayed++;

  polygon_radians+=PI/180;
  
  /*check for keypresses or other events*/
  glfwPollEvents();
 
  /*
  glfwseconds=glfwGetTime();
  glfwseconds1=glfwseconds+1.0/fps;
  while(glfwseconds<glfwseconds1)
  {
   glfwseconds=glfwGetTime();
  }
  */
  
 }
 
}
