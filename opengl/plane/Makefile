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

plane : plane.o 
	g++ $(LDFLAGS) $^ -o plane

plane.o: plane.c
	g++ -c $(CFLAGS) plane.c

depend:
	makedepend *.c
