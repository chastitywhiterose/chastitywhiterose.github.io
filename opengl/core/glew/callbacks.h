/*callbacks for opengl programs*/

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
 if(action==GLFW_PRESS || action==GLFW_REPEAT)
 {
  switch(key)
  {
   case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window,GLFW_TRUE);
   break;

     /*keys 0 to 7 change the polygon to my favorite colors*/
   case GLFW_KEY_0:
    color_r=0,color_g=0,color_b=0;
   break;
   case GLFW_KEY_1:
    color_r=0,color_g=0,color_b=1;
   break;
   case GLFW_KEY_2:
    color_r=0,color_g=1,color_b=0;
   break;
   case GLFW_KEY_3:
    color_r=0,color_g=1,color_b=1;
   break;
   case GLFW_KEY_4:
    color_r=1,color_g=0,color_b=0;
   break;
   case GLFW_KEY_5:
    color_r=1,color_g=0,color_b=1;
   break;
   case GLFW_KEY_6:
    color_r=1,color_g=1,color_b=0;
   break;
   case GLFW_KEY_7:
    color_r=1,color_g=1,color_b=1;
   break;


   case GLFW_KEY_Q:
    polyfunc=gl_polygon;
   break;
   case GLFW_KEY_W:
    polyfunc=gl_polygon1;
   break;
   case GLFW_KEY_E:
    polyfunc=gl_polygon2;
   break;
   case GLFW_KEY_R:
    polyfunc=gl_polygon3;
   break;

   case GLFW_KEY_A:
    get_polygon_points_array_star_func=get_polygon_points_array_star;
   break;
   case GLFW_KEY_S:
    get_polygon_points_array_star_func=get_polygon_points_array_star_rgb;
   break;
   case GLFW_KEY_D:
    get_polygon_points_array_star_func=get_polygon_points_array_star_cmy;
   break;

   /*can move the polygon anywhere with keyboard*/
   case GLFW_KEY_H:
    polygon_cx-=1;
   break;
   case GLFW_KEY_J:
    polygon_cy+=1;
   break;
   case GLFW_KEY_K:
    polygon_cy-=1;
   break;
   case GLFW_KEY_L:
    polygon_cx+=1;
   break;


   case GLFW_KEY_Z:
    polygon_change=0;
    polygon_sides=3;
   break;
   case GLFW_KEY_X:
    polygon_change=1;
   break;
  }
 }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
 double x,y;
 glfwGetCursorPos(window,&x,&y);
 /*printf("mouse x=%f,y=%f\n",x,y);*/
 if(action == GLFW_PRESS)
 {
  if(button==GLFW_MOUSE_BUTTON_LEFT)
  {
   polygon_cx=x;
   polygon_cy=y;
  }
  if(button==GLFW_MOUSE_BUTTON_MIDDLE)
  {
   polygon_radius=x+y;
  }
  if(button==GLFW_MOUSE_BUTTON_RIGHT)
  {
   
  }
 }
}

void cursor_position_callback(GLFWwindow *window,double x,double y)
{
 if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS)
 {
  polygon_cx=x;
  polygon_cy=y;
 }
 if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_PRESS)
 {
  polygon_radius=x+y;
 }
}

void framebuffer_size_callback(GLFWwindow* window, int newwidth, int newheight)
{
 /*make sure the viewport matches the new window dimensions; note that width and 
  height will be significantly larger than specified on retina displays.*/
 width=newwidth;
 height=newheight;
 glViewport(0, 0, width, height);
polygon_cx=width/2;
polygon_cy=height/2;
if(height<width){polygon_radius=height/3;}
else{polygon_radius=width/3;}
}
