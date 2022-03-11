linux_glfw_ftgl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw -lOpenGL -lm `pkg-config --cflags --libs ftgl` && ./main
win_glfw_ftgl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL32 -lm `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs ftgl` && ./main
