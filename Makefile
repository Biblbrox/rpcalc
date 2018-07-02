all:
	gcc -gdwarf-2 -g3 -lm ./src/utils.c ./src/calc.c ./src/main.c ./src/stack.c ./deps/map/map.c -o calc.o 

clean:
	rm *.o
