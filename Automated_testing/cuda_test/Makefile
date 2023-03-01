
.PHONY: all build test_build brun clean


CFLAGS := -O3 -fPIC -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51 $(CFLAGS) #-Wall

GFLAGS_ = --gpu-architecture=$(sm_VAL) --compiler-options '-fPIC' -O3 --use_fast_math $(GFLAGS) #-g -G --ptxas-options=-v


#EXTRA := -DMOREINFO 

CC := ${CC}


c_files = ../main.c
cuda_files = cuda_section0.cu

sm_VAL = ${SM_VAL}
#sm_VAL = sm_80
#sm_VAL = sm_70


runname = ./bin/run
#$(CXX)
#$(CC)

#--use_fast_math


all: init build

init: 
	mkdir -p bin

build: cuda
	$(CC) -fPIC ./bin/lib -Xopenmp-target -march=$(sm_VAL) $(CFLAGS) $(c_files) -o $(runname) -lstdc++ -L/home/shared/software/cuda/11.6/lib64 -lcudart $(EXTRA) -I ./ -I ../

cuda:
	nvcc $(GFLAGS_) -o ./bin/lib -c $(cuda_files) $(EXTRA) -I ./ -I ../

build_for_test: init build

run:
	./bin/run
brun: build run

clean:
	rm -rf ./bin

print:
	@echo $(CFLAGS)
	@echo $(GFLAGS_)
	@echo $(CC)
	@echo $(c_files)
	@echo $(cuda_files)
	@echo $(sm_VAL)
	@echo $(runname)
	@echo $(EXTRA)