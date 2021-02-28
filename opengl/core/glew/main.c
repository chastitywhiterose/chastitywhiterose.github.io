#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int width=1280,height=720; /*size of window*/

#define PI 3.14159265358979323846

/*declare variables for attributes of the polygon*/
int polygon_sides=3;         /*how many sides and/or corners of polygon*/
int polygon_step=1;
float polygon_cx;          /*horizontal left/right x coordinate of center*/
float polygon_cy;         /*vertical up/down y coordinate of center*/
float polygon_radius;     /*radius or how big the polygon will be*/

float polygon_radians=0;           /*rotation of the polygon*/
float polygon_radians_step=PI/180; /*how much the polygon rotates each frame*/

float color_r=1,color_g=1,color_b=1;

#include "gl_bbm_polygon_core.h"

int frame=0;

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
 if(action==GLFW_PRESS || action==GLFW_REPEAT)
 {
  switch(key)
  {
   case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window,GLFW_TRUE);
   break;
   case GLFW_KEY_Q:
    polyfunc=gl_polygon;
   break;
   case GLFW_KEY_W:
    polyfunc=gl_polygon1;
   break;
  }
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

const char *vertexShaderSource = "#version 330 core\n"
"in vec2 position;\n"
"in vec3 color;\n"
"out vec3 Color;\n"
"void main()\n"
"{\n"
 "Color=color;\n"
"gl_Position=vec4(position,0.0,1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"in vec3 Color;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"outColor=vec4(Color,1.0);\n"
"}\n\0";

/*variables defined separate from code for C90 purposes*/
 GLFWwindow* window;
 unsigned int vertexShader;
 unsigned int fragmentShader;
 unsigned int shaderProgram;
 GLuint VBO, VAO;
 GLint posAttrib;
 GLint colAttrib;
 int success;
 char infoLog[512];

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

 glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);    
 glfwSetKeyCallback(window,key_callback);

 printf("The OpenGL version is: %s\n",glGetString(GL_VERSION));

vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 glCompileShader(vertexShader);



 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 if (!success)
 {
  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
  printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
 }
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 glCompileShader(fragmentShader);

 glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 if (!success)
 {
  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
  printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
 }
 
shaderProgram = glCreateProgram();
 glAttachShader(shaderProgram, vertexShader);
 glAttachShader(shaderProgram, fragmentShader);
 glLinkProgram(shaderProgram);

 glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 if(!success)
 {
  glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n",infoLog);
 }
 glDeleteShader(vertexShader);
 glDeleteShader(fragmentShader);

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

 while (!glfwWindowShouldClose(window))
 {
  glClear(GL_COLOR_BUFFER_BIT);

  /*polyfunc();*/
  gl_polygon2();
  
  polygon_radians+=PI/180;

  frame++;
  if(frame%360==0)
  {
   /*
   This section increments the step of the star polygon. If the step would result in an invalid star polygon being drawn, the step is reset to 1 and the number of points incremented. The genius of   this is that it will cycle through every possible regular polygon both convex and star!
  */
  
  polygon_step+=1;
  if(polygon_step>=(polygon_sides/2)+polygon_sides%2)
  {
   polygon_sides+=1;
   polygon_step=1;
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
