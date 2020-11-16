linux_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw -lOpenGL -lm && ./main
win_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw3 -lOpenGL32 -lm && ./main
win_opengl_static:
	gcc -Wall -ansi -pedantic main.c -o main -lglfw3 -lOpenGL32 -lgdi32 -lm -static && ./main
	strip main.exe
