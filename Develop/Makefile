

#flags = -Wall -fopenmp=libomp -O3 -g
flags = -Wall -fopenmp -O3 -fast -lm -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70 -lomptarget -g

#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm

c_files = main.c
#c_files = test.c
runname = ./bin/run
#$(CXX)
#$(CC)

all: init

init: 
	mkdir -p bin

build:
	#echo $(flags)
	$(CC) $(flags) $(c_files) -o $(runname)
	chmod +x ./bin/run

run:
	./bin/run
brun: build run

clean:
	rm -rf ./bin
