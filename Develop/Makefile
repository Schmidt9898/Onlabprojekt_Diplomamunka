

flags = "-Wall -fopenmp=libomp -O3 -fast -lm -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70"
c_files = "main.c"
runname = "./bin/run"
#$(CXX)
#$(CC)

all: init

init: 
	mkdir -p bin

build:
	#echo $(flags)
	$(CC) -c $(c_files) -o $(runname)



clean:
	rm -rf ./bin
