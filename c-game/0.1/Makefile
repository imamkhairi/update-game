CFLAGS = -Wall

all: play

play:	main.o game.o
		cc main.o game.o -lncursesw -ltinfo -o play

game.o: game.c game.h

main.o: main.c game.h

.c .o:
		cc -c $< $(CFLAGS)

.SUFFIXES: .c .o

clean:
		-rm *.o

distclean: clean
		-rm play

dist: distclean
		(cd ..; tar zcvf ver_0.1.tgz 0.1/)

.PHONY: clean distclean dist