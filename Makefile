all: pi_serial.o pi_multi-thread.o
	gcc pi_serial.o -o pi_serial -lm
	gcc pi_multi-thread.o -o pi_multi-thread -lm -lpthread
pi_serial.o: pi_serial.c
	gcc -c pi_serial.c  
pi_multi-thread.o: pi_multi-thread.c
	gcc -c pi_multi-thread.c 
clean:
	rm -f *.o pi_serial
	rm -f *.o pi_multi-thread
