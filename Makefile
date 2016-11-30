.PHONY: all clean

all: example

example: example.o vs_config.o

example.o: example.c vs_config.h

vs_config.o: vs_config.c vs_config.h

clean:
	$(RM) example *.o
