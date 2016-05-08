GLFLAGS = -lGL -lGLU -lglut -lSOIL
CFLAGS = -Wall
CVERSION = -std=c++98
SOUND = -lSDL -lSDL_mixer

all: clean snake

snake:
	$(info Compiling...)
	g++ -o snake `sdl-config --cflags --libs` main.cpp $< $(SOUND) $(CVERSION) $(CFLAGS) $(GLFLAGS) -DUSE_BUFFERS
	
clean:
	$(info Removing objects...)
	rm -f snake
	rm -f snake-gcw