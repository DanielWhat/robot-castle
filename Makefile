# Definitions
CC = g++
CFLAGS = -Wall 
FRAMEWORKS = -framework OpenGL -framework GLUT

#Default target
all: assignment_1.out

open_off.o: open_off.cpp open_off.h
	$(CC) -c $(CFLAGS) $< -o $@

assignment_1.o: assignment_1.cpp open_off.h
	$(CC) -c $(CFLAGS) $< -o $@


assignment_1.out: assignment_1.o open_off.o
	$(CC) $(CFLAGS) $^ -o $@ -lm $(FRAMEWORKS)



