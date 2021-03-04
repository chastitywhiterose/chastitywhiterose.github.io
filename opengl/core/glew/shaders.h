/*because setting up the shaders is such a gigantic part of a modern OpenGL program. I have it in this separate file to be reused.*/

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

void make_shaders()
{

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

}
