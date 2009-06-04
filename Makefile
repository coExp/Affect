#################################################
#                                               #
#                   CONFIG                      #
#                                               #
#################################################

DEBUG?= -g 
CFLAGS	= -Wall -W -std=c99 
CC	= gcc
EXE	= affect

#################################################

SRC :=	 	$(wildcard *.c) 

HEADER :=  	$(wildcard *.h)
 
OBJ    := $(SRC:%.c=%.o)

.c.o: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG) $< -c -o $@

all: $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

clean:
	rm -f *.o */*.o *~ */*~ $(EXE)

debug: $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJ) -o $(EXE)


#################################################
