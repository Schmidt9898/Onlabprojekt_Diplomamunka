
.PHONY: all build brun clean



#flags = -Xopenmp-target -march=$(sm_VAL) -Ofast -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lomptarget # -g -Wall
#flags = -Xopenmp-target -march=$(sm_VAL) -O3 -fPIC -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51 # -g -Wall
flags = -Ofast -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lomptarget -Xopenmp-target -march=sm_80 -fopenmp-version=51 -lomptarget -lm -L $(CLANG_HOME)/lib
flags_intel = -DLOMP -Ofast -ffast-math -lomptarget -fiopenmp -fopenmp-targets=spir64 -fopenmp-version=51 #-march=sm_70 -DL1D
# -fopenmp -qopenmp
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
	mkdir -p bin/forblocked
	mkdir -p bin/forblocktilled
	mkdir -p bin/fornaiv
	mkdir -p bin/fortilled


build: init
	$(CC) $(flags) $(c_files) $(EXTRA) -o $(runname)

bintel: init
	icx $(flags_intel) $(c_files) $(EXTRA) -o $(runname)

prep:
	$(CC) $(flags) $(c_files) $(EXTRA) -E -o prep.c -I ./ -I ../

build_for_test: init build

build_for_intel: init bintel

run:
	./bin/run

clean:
	rm -rf ./bin

clear_results:
	rm -rf ./results
	mkdir -p ./results


print:
	@echo $(flags)
	@echo $(CC)
	@echo $(c_files)
	@echo $(sm_VAL)
	@echo $(runname)
	@echo $(EXTRA)

