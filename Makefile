all:
	gcc -c -gdwarf-2 -g3 ./src/utils.c ./src/vars.c ./src/calc.c ./src/main.c ./src/stack.c ./deps/map/map.c
	gcc -o calc utils.o ./deps/ll.c/ll.o calc.o vars.o main.o stack.o map.o -lm 
	rm *.o

clean:
	rm *.o
