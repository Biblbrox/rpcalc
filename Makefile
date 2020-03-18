all:
	gcc -c -g -gdwarf-2 ./src/utils.c ./src/vars.c ./src/calc.c ./src/main.c ./src/stack.c ./deps/map/map.c
	gcc -o calc utils.o ./deps/ll/ll.o calc.o vars.o main.o stack.o map.o -lm 
	rm *.o

clean:
	rm calc 
