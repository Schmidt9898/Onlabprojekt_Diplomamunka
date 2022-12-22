
.PHONY: all build brun clean


flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51


c_files = main.c




runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build:
	$(CC) -fPIC ./bin/lib -Xopenmp-target -march=sm_70 $(flags) $(c_files) -o $(runname) 
build_sm8:
	$(CC) -fPIC ./bin/lib -Xopenmp-target -march=sm_80 $(flags) $(c_files) -o $(runname) -v -lstdc++ -L/home/shared/software/cuda/11.6/lib64 -lcudart


cuda:
	nvcc --ptxas-options=-v --gpu-architecture=sm_80 --compiler-options '-fPIC' -o ./bin/lib -c cuda_section0.cu


run:
	./bin/run
brun: build run

8run: build_sm8 run

clean:
	rm -rf ./bin
