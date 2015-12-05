all: read write

hack.o: hack.c hack.h
	clang -O0 -std=c11 -pedantic -g -Wall -c hack.c

main.read.o: main.read.c hack.h
	clang -O0 -std=c11 -pedantic -g -Wall -c main.read.c

main.write.o: main.write.c hack.h
	clang -O0 -std=c11 -pedantic -g -Wall -c main.write.c

read: main.read.o hack.o
	clang main.read.o hack.o -o read

write: main.write.o hack.o
	clang main.write.o hack.o -o write

clean:
	rm -f *.o
	rm read
	rm write
