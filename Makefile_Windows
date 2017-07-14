
#GNU C Compiler
CC = gcc
CCFLAGS = -Wall

all: main.exe

main.exe: main.c
	$(CC) $(CCFLAGS) -o $@ main.c -ltcl86 -ltk86 -lopengl32 -lwinmm -lgdi32

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@



