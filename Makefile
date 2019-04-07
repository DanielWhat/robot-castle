# Definitions
CC = g++
CFLAGS = -Wall 
FRAMEWORKS = -framework OpenGL -framework GLUT

#Default target
all: assignment_1.out

open_off.o: open_off.cpp open_off.h
	$(CC) -c $(CFLAGS) $< -o $@
	
castle_rendering.o: castle_rendering.cpp castle_rendering.h open_off.h
	$(CC) -c $(CFLAGS) $< -o $@
	
cannon.o: cannon.cpp cannon.h open_off.h
	$(CC) -c $(CFLAGS) $< -o $@
	
robot.o: robot.cpp robot.h
	$(CC) -c $(CFLAGS) $< -o $@

assignment_1.o: assignment_1.cpp open_off.h castle_rendering.h cannon.h
	$(CC) -c $(CFLAGS) $< -o $@

assignment_1.out: assignment_1.o open_off.o castle_rendering.o cannon.o robot.o
	$(CC) $(CFLAGS) $^ -o $@ -lm $(FRAMEWORKS)
	
	
# Target: clean project.
.PHONY: clean
clean:
	rm *.o *.h.gch *.out



