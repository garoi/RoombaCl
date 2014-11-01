CC = g++
CFLAGS = -I/usr/include/pccts/
roomba: roomba.c
	$(CC) -w -o roomba roomba.c scan.c err.c $(CFLAGS)
	rm -f *.o scan.c err.c parser.dlg tokens.h mode.h
	./roomba < exemple.txt
roomba.c: roomba.g
	antlr -gt roomba.g
	dlg -ci parser.dlg scan.c
clean:
	rm -f *.o roomba roomba.c scan.c err.c parser.dlg tokens.h mode.h