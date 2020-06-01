output: main.o shell.o
	g++ main.o shell.o -o output

main.o: main.cpp
	g++ -c main.cpp

shell.o: shell.cpp shell.h
	g++ -c shell.cpp
	
clean:
	rm *.o output