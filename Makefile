CC = gcc
ALLEGROLIBS = `allegro-config --cflags`
CFLAGS = -I/usr/include -I./inc -Wall $(ALLEGROLIBS)
DEBUG = -pg -ggdb
OBJ = engine.o main.o
V = 1.0

#Complete and clean compilation
all: pong clean

#THE game
pong: $(OBJ)
	$(CC) $(CFLAGS) -o pong-$V $(OBJ) `allegro-config --libs`

#object files
main.o: src/main.c inc/engine.h
	$(CC) $(CFLAGS) -c -o main.o src/main.c

engine.o: src/engine.c inc/engine.h
	$(CC) $(CFLAGS) -c -o engine.o src/engine.c

#utilities
#clean: clear object files and backup files
clean:
	rm -f *.o *~ 2> /dev/null
#clear: clear all the compilation
clear: clean
	rm -f pong-$V 2> /dev/null

#tar
#create tarball
tar:
	mkdir pong-$V
	cp -rf -t pong-$V src inc snd Makefile
	tar -czvf pong-$V.tar.gz --remove-files pong-$V
