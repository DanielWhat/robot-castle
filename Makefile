# Definitions
CC = g++
CFLAGS = -w 
FRAMEWORKS = -framework OpenGL -framework GLUT

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	FRAMEWORKS = -lGL -lGLU -lglut
endif

#Default target
all: assignment_1.out

open_off.o: open_off.cpp open_off.h
	$(CC) -c $(CFLAGS) $< -o $@
	
castle_rendering.o: castle_rendering.cpp castle_rendering.h open_off.h textures.h spaceship.h particle.h
	$(CC) -c $(CFLAGS) $< -o $@
	
cannon.o: cannon.cpp cannon.h open_off.h
	$(CC) -c $(CFLAGS) $< -o $@
		
robot.o: robot.cpp robot.h
	$(CC) -c $(CFLAGS) $< -o $@
	
textures.o: textures.cpp textures.h
	$(CC) -c $(CFLAGS) $< -o $@
	
animations.o: animations.cpp animations.h robot.h spaceship.h cannon.h
	$(CC) -c $(CFLAGS) $< -o $@
	
spaceship.o: spaceship.cpp spaceship.h 
	$(CC) -c $(CFLAGS) $< -o $@
	
particle.o: particle.cpp particle.h
	$(CC) -c $(CFLAGS) $< -o $@

assignment_1.o: assignment_1.cpp open_off.h castle_rendering.h cannon.h robot.h spaceship.h textures.h
	$(CC) -c $(CFLAGS) $< -o $@

assignment_1.out: assignment_1.o open_off.o castle_rendering.o cannon.o robot.o textures.o animations.o spaceship.o particle.o
	$(CC) $(CFLAGS) $^ -o $@ -lm $(FRAMEWORKS)
	
	
# Target: clean project.
.PHONY: clean
clean:
	rm *.o *.h.gch *.out



