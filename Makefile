GLFLAGS = -lGL -lGLU -lglut -lSOIL
CFLAGS = -Wall
CVERSION = -std=c++98

all: clean snake

snake:
	$(info Compiling...)
	g++ -o snake main.cpp $< $(CVERSION) $(CFLAGS) $(GLFLAGS) -DUSE_BUFFERS
	
clean:
	$(info Removing objects...)
	rm -f snake
	rm -f snake-gcw