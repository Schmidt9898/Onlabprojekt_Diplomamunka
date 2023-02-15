
.PHONY: all build brun clean


flags = -O3 -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51


c_files = main.c

#sm_VAL = sm_80
sm_VAL = sm_70


runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build: cuda
	$(CC) -fPIC ./bin/lib -Xopenmp-target -march=$(sm_VAL) $(flags) $(c_files) -o $(runname) -lstdc++ -L/home/shared/software/cuda/11.6/lib64 -lcudart #-g


cuda:
	nvcc --ptxas-options=-v --gpu-architecture=$(sm_VAL) --compiler-options '-fPIC' -o ./bin/lib -c cuda_section0.cu -O3 #-g -G

run:
	./bin/run
brun: build run

clean:
	rm -rf ./bin
