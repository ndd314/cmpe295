MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL -FGLUT
	OPENGL_LIB= -framework OpenGL -framework GLUT
else
	OPENGL_INC= -I/usr/X11R6/include -I/user/local/include
	OPENGL_LIB= -I/usr/X11R6/lib -L/usr/local/lib -lGL -lGLU -lglut -lm
endif

CFLAGS= $(OPENGL_INC)
LDFLAGS= $(OPENGL_LIB)

BINARIES=2dpattern 10Visual 10-1Visual_Rohan 10-1Visual 10-2Visual 10-3Visual 10-4Visual

all: $(BINARIES)

clean:
	-rm *.o $(BINARIES)

10Visual.o: 10Visual.c
	gcc -c $(CFLAGS) 10Visual.c

2dpattern.o: 2dpattern.c
	gcc -c $(CFLAGS) 2dpattern.c

10-1Visual_Rohan: 10-1Visual_Rohan.o readBMPV2.o
	gcc $(LDFLAGS) $^ -o 10-1Visual_Rohan

10-1Visual_Rohan.o: 10-1Visual_Rohan.c
	gcc -c $(CFLAGS) 10-1Visual_Rohan.c

10-1Visual.o: 10-1Visual.c
	gcc -c $(CFLAGS) 10-1Visual.c

10-2Visual.o: 10-2Visual.c
	gcc -c $(CFLAGS) 10-2Visual.c

10-3Visual: 10-3Visual.o readBMPV2.o
	gcc $(LDFLAGS) $^ -o 10-3Visual

10-3Visual.o: 10-3Visual.c
	gcc -c $(CFLAGS) 10-3Visual.c

10-4Visual: 10-4Visual.o readBMPV2.o
	gcc $(LDFLAGS) $^ -o 10-4Visual

10-4Visual.o: 10-4Visual.c
	gcc -c $(CFLAGS) 10-4Visual.c

readBMPV2.o: readBMPV2.c
	gcc -c $(CFLAGS) readBMPV2.c

depend:
	makedepend *.c
