

#git clone https://github.com/llvm/llvm-project.git

#cd llvm-project

module load gcc/11.2.0 gpu/cuda/11.6 cmake/3.18.5 make/4.3
cd /home/schmidtl/llvm-project
mkdir build
cd build
cmake ../llvm/ -DCMAKE_BUILD_TYPE=Release -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=sm_70,sm_80 -DLIBOMPTARGET_NVPTX_COMPUTE_CAPABILITIES=70,80 -DLLVM_TARGETS_TO_BUILD="X86;NVPTX" -DLLVM_ENABLE_PROJECTS="clang;openmp"
make -j$(nproc)


cd ..
mkdir build2
cd build2
CC=../build/bin/clang CXX=../build/bin/clang++ cmake ../llvm/ -DCMAKE_BUILD_TYPE=Release -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=sm_70,sm_80 -DLIBOMPTARGET_NVPTX_COMPUTE_CAPABILITIES=70,80 -DLLVM_TARGETS_TO_BUILD="X86;NVPTX" -DLLVM_ENABLE_PROJECTS="clang;openmp" -DCMAKE_INSTALL_PREFIX=/home/shared/software/clang/15.3
make -j$(nproc)
make install


echo "Build is done"