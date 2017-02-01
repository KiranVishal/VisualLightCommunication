CC=$(EC535)/gumstix/oe/cross/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-pthread -I.
DEPS=
OBJ= lib/libsndfile.so audiooutput.o
PROJ=audiooutput

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJ): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) *.o $(PROJ)
