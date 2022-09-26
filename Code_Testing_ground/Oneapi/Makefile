.PHONY: build rebuild run test clean 

build:
	icx -I ../../Smart_Stopper ./main.c  -o run -Wall -O3 -std=c11 -qopenmp -xHOST

run: 
	./run

clean:
	rm ./run

rebuild: clean build

