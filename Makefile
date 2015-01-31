simpleload: main.c config.h graph.c graph.h measure.c measure.h
	gcc -std=gnu99 -Wall -o simpleload main.c graph.c measure.c $(shell pkg-config --cflags --libs gtk+-3.0)
