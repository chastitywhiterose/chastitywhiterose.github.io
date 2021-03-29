#include <stdio.h>
#include <math.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int width=1280,height=720; /*size of window*/

#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
int polygon_sides=5;         /*how many sides and/or corners of polygon*/
int polygon_step=2;
float polygon_cx;          /*horizontal left/right x coordinate of center*/
float polygon_cy;         /*vertical up/down y coordinate of center*/
float polygon_radius;     /*radius or how big the polygon will be*/

float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

float color_r=1,color_g=1,color_b=1;

#include "gl_bbm_polygon_core.h"


int polygon_change=0; /*whether polygon automatically changes over time*/

int frame=0;

#include "callbacks.h"

/*other variables defined separate from code for C90 purposes*/
 GLFWwindow* window;
 unsigned int vertexShader;
 unsigned int fragmentShader;
 unsigned int shaderProgram;
 GLuint VBO, VAO;
 GLint posAttrib;
 GLint colAttrib;
 int success;
 char infoLog[512];

#include "shaders.h"

int main()
{

 glfwInit();
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
 window = glfwCreateWindow(width,height, "Chastity Modern OpenGL", NULL, NULL);
 if (window == NULL)
 {
  printf("Failed to create GLFW window\n");
  glfwTerminate();
  return -1;
 }

 glfwMakeContextCurrent(window);

glewExperimental=1;
if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
}

 /*set up the callback functions for user input*/
 glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);    
 glfwSetKeyCallback(window,key_callback);
 glfwSetMouseButtonCallback(window,mouse_button_callback);
 /*glfwSetCursorPosCallback(window, cursor_position_callback);*/

 printf("The OpenGL version is: %s\n",glGetString(GL_VERSION));

 make_shaders();

 glGenVertexArrays(1, &VAO);
 glGenBuffers(1, &VBO);
 glBindVertexArray(VAO);


 glBindBuffer(GL_ARRAY_BUFFER, VBO);
 
 /*position attributes*/
 posAttrib = glGetAttribLocation(shaderProgram, "position");
 glEnableVertexAttribArray(posAttrib);
 glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,5*sizeof(float), 0);

 /*color attributes*/
 colAttrib = glGetAttribLocation(shaderProgram, "color");
 glEnableVertexAttribArray(colAttrib);
 glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,5*sizeof(float), (void*)(2*sizeof(float)));

 glViewport(0,0,width,height);
 glClearColor(0.0f,0.0f,0.0f,1.0f);

 glfwSwapInterval(1);

 polygon_cx=width/2;
 polygon_cy=height/2;
 polygon_radius=height/2;

 glUseProgram(shaderProgram);

polyfunc=gl_polygon;

 while (!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

  polyfunc();

  
  polygon_radians+=PI/180;

  frame++;
  if(frame%360==0)
  {
   /*
   This section increments the step of the star polygon. If the step would result in an invalid star polygon being drawn, the step is reset to 1 and the number of points incremented. The genius of   this is that it will cycle through every possible regular polygon both convex and star!
  */
  
 if(polygon_change)
 {
  polygon_step+=1;
  if(polygon_step>=(polygon_sides/2)+polygon_sides%2)
  {
   polygon_sides+=1;
   polygon_step=1;
  }
}
 }
 
  glfwSwapBuffers(window);
  glfwPollEvents();
 }
 
 glDeleteVertexArrays(1, &VAO);
 glDeleteBuffers(1, &VBO);
 glDeleteProgram(shaderProgram);

 glfwTerminate();
 return 0;
}

/*gcc -Wall -ansi -pedantic main.c -o main -lglfw -lOpenGL -lm -lGLEW && ./main*/
