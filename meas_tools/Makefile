
all: init Timer CTimer build_validate

init: create_bin
create_bin: bin
	mkdir bin

Timer:
	$(CXX) -c Timer.cpp -Ofast -o ./bin/Timer.o
CTimer:
	$(CC) -c Timer.c -Ofast -o ./bin/CTimer.o
build_validate:
	$(CC) byte_validation.c -Ofast -o ./bin/byte_valid

clean:
	rm -r ./bin
