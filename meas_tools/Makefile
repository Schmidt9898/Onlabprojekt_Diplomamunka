
all: init Timer CTimer build_validate

init: 
	mkdir -p bin
Timer:
	$(CXX) -c Timer.cpp -Ofast -o ./bin/Timer.o
CTimer:
	$(CC) -c Timer.c -Ofast -o ./bin/CTimer.o
build_validate:
	$(CC) byte_validation.c -Ofast -o ./bin/byte_valid

clean:
	rm -rf ./bin
