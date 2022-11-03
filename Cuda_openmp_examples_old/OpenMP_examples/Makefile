

.PHONY: all build brun clean


flags = -Wall -fopenmp=libomp -O3 	-g		-fopenmp-targets=nvptx64-nvidia-cuda									-DLOMP -DL1D
#flags = -Wall -fopenmp=libomp -O3 -fast -lm -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70 -lomptarget -DLOMP -DL1D



#flags = -Wall -fopenmp -O3 -fast -lm -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70 -lomptarget -g
#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm 

#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm

c_files = mainoptim.c
#c_files = test.c
runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build:
	#echo $(flags)
	$(CC) $(flags) $(c_files) -o $(runname)
	#chmod +x ./bin/run

preproc:
	$(CC) $(flags) -E $(c_files) -o $(runname).preproc.c

run:
	./bin/run
brun: build run

clean:
	rm -rf ./bin
