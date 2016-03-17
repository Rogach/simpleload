simpleload: main.c config.h graph.c graph.h measure.c measure.h state.c state.h
	gcc -std=gnu99 -Wall -o simpleload main.c graph.c measure.c state.c $(shell pkg-config --cflags --libs gtk+-3.0 libgtop-2.0) -lm
