project: project.o func.o gfx.o 
	gcc project.o func.o gfx.o -o project -lX11 -lm

project.o: project.c func.h gfx.h 
	gcc -Wall -c project.c

func.o: func.c func.h gfx.h
	gcc -Wall -c func.c

clean: 
	rm -f project project.o gamefunc.o 
