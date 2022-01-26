CFLAGS = -Wall

all: play

play:	main.o game.o map.o
		cc main.o game.o map.o -lncursesw -ltinfo -o play

game.o: game.c game.h

main.o: main.c game.h

map.o: map.c

.c .o:
		cc -c $< $(CFLAGS)

.SUFFIXES: .c .o

clean:
		-rm *.o

distclean: clean
		-rm play

dist: distclean
		(cd ..; tar zcvf ver_0.2.tgz 0.2/)

.PHONY: clean distclean dist