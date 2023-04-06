
.PHONY: all build brun clean



flags = -Xopenmp-target -march=$(sm_VAL) -O3 -fPIC -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm -L $(CLANG_HOME)/lib -lomptarget -fopenmp-version=51 # -g -Wall


sm_VAL = ${SM_VAL}

CC := ${CC}

c_files = ../main.c
#c_files = test.c
runname = ./bin/run
#$(CXX)
#$(CC)

all: init build

init: 
	mkdir -p bin

build:
	$(CC) $(flags) $(c_files) $(EXTRA) -o $(runname) -I ./ -I ../

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

