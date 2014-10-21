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

BINARIES=plane

all: $(BINARIES)

clean:
	-rm *.o $(BINARIES)

plane: plane.o readBMPV2.o
	gcc $(LDFLAGS) $^ -o plane

plane.o: plane.c
	gcc -c $(CFLAGS) plane.c

readBMPV2.o: readBMPV2.c
	gcc -c $(CFLAGS) readBMPV2.c

depend:
	makedepend *.c
