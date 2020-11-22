linux_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL -lGLU -lglut -lm &&./main
win_opengl:
	gcc -Wall -ansi -pedantic main.c -o main -lOpenGL32 -lglu32 -lfreeglut -lm && ./main
win_opengl_static:
	gcc -Wall -ansi -pedantic main.c -o main -D FREEGLUT_STATIC -lfreeglut_static -lwinmm -lgdi32 -lOpenGL32 -lglu32 -lm && ./main
	strip main.exe
