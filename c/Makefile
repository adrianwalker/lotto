CC = gcc
CFLAGS = -Wall -g -Ofast
OBJECTS = lotto.o

lotto : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o lotto

lotto.o : lotto.c
	$(CC) $(CFLAGS) -c lotto.c

clean :
	rm *.o lotto
