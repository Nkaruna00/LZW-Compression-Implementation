CC=g++ -std=c++14 -W -Wall

LINK=-O2 -lm
SRC=main.cpp lzw.cpp
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

lzw: 	$(OBJ)
	$(CC) $(OBJ) $(LINK) -o $@

clean:
	rm *.o lzw
