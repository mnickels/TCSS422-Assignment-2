CC = gcc 
CFLAGS = -c -Wall 
C99FLAG = -std=c99
output: pcb.o test.o 
	$(CC) $(C99FLAG) pcb.c test.c -o output

pcb.o: pcb.c 
	$(CC) $(CFLAGS) $(C99FLAG) pcb.c 

test.o: test.c 
	$(CC) $(CFLAGS) $(C99FLAG) test.c

clean: 
	rm *.o output