
.PHONY: all build brun clean



#flags = -Xopenmp-target -march=$(sm_VAL) -Ofast -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lomptarget # -g -Wall
#flags = -Xopenmp-target -march=$(sm_VAL) -O3 -fPIC -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51 # -g -Wall
flags = -DLOMP -DL1D -Ofast -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lomptarget -Xopenmp-target -march=sm_70
flags_intel = -DLOMP -DL1D -Ofast -ffast-math -fopenmp -lomptarget -qopenmp -fiopenmp -fopenmp-targets=spir64 #-march=sm_70

sm_VAL = ${SM_VAL}

CC := ${CC}

c_files = ./mainso8.c
#c_files = test.c
runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build: init
	$(CC) $(flags) $(c_files) $(EXTRA) -o $(runname)

bintel: init
	icx $(flags_intel) $(c_files) $(EXTRA) -o $(runname)

prep:
	$(CC) $(flags) $(c_files) $(EXTRA) -E -o prep.c -I ./ -I ../

build_for_test: init build

run:
	./bin/run

clean:
	rm -rf ./bin

print:
	@echo $(flags)
	@echo $(CC)
	@echo $(c_files)
	@echo $(sm_VAL)
	@echo $(runname)
	@echo $(EXTRA)

