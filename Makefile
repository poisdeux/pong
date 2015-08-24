CC = gcc
ALLEGROLIBS = `allegro-config --cflags`
CFLAGS = -I/usr/include -I./inc -Wall $(ALLEGROLIBS)
DEBUG = -pg -ggdb
OBJ = engine.o main.o
V = 1.1

#Complete and clean compilation
all: pong 

#THE game
pong: $(OBJ)
	$(CC) $(CFLAGS) -o pong-$V $(OBJ) -lm `allegro-config --libs`

#object files
main.o: src/main.c inc/engine.h
	$(CC) $(CFLAGS) -c -o main.o src/main.c

engine.o: src/engine.c inc/engine.h
	$(CC) $(CFLAGS) -c -o engine.o src/engine.c

#utilities
#clean: clear object files and backup files
clean:
	git clean -f -x

#tar
#create tarball
tar:
	mkdir pong-$V
	cp -rf -t pong-$V src inc snd Makefile
	tar -czvf pong-$V.tar.gz --remove-files pong-$V
