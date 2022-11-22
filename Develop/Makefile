
.PHONY: all build brun clean


#flags = -Wall -fopenmp=libomp -O3 	-g -lm		-fopenmp-targets=nvptx64-nvidia-cuda	
#-fPIC külön fordítók
#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm 
#flags = -Wall -fopenmp=libomp -O3 -g

#flags = -Wall -fopenmp -O3 -fast -lm -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70 -lomptarget -g
# devito original clang -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -shared -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm

flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51

#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_70 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L/home/shared/software/clang/14.6-centos8/lib/ -lomptarget
#flags = -O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -Xopenmp-target -march=sm_80 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L/home/shared/software/clang/14.6-centos8/lib/ -lomptarget



c_files = main.c
#c_files = test.c
runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build:
	$(CC) -Xopenmp-target -march=sm_70 $(flags) $(c_files) -o $(runname) 
build_sm8:
	$(CC) -Xopenmp-target -march=sm_80 $(flags) $(c_files) -o $(runname)



run:
	./bin/run
brun: build run

8run: build_sm8 run

clean:
	rm -rf ./bin
