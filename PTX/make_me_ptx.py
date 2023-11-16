
import os
print("I make you PTX")

# copy here the source to be up to date

#mkdir ./src -p
#cp ../Automated_testing/mainso8/mainso.cu ./src/mainso.cu
#cp ../Automated_testing/mainso8/mainso8.c ./src/mainso8.c


print(os.popen("mkdir ./src -p").read())
print(os.popen("rm -rf ./output").read())
print(os.popen("mkdir -p ./output/cuda ./output/clang").read())
print(os.popen("cp ../Automated_testing/mainso8/mainso.cu ./src/mainso.cu").read())
print(os.popen("cp ../Automated_testing/mainso8/mainso8.c ./src/mainso8.c").read())

#ret = os.popen(command).read()

cmd_build0 = "clang -S -Xclang -fopenmp-targets=nvptx64 -emit-llvm -Xclang -O3 -o output.ll ./src/mainso8.c" 
cmd_build1 = "llc -march=nvptx -mcpu=sm_70 output.ll -o "

#clang_build = "nvcc -ptx -Xcompiler -fopenmp ./src/mainso8.c -o "

path = "./input/clang.csv"

f = open(path,'r')
lines = f.readlines()
f.close()
cases = []
for line in lines:
	#break
	line = line.strip()
	#print(line)
	g_name = line.replace(" ","_").replace("=","").replace("-D","") + ".ptx"
	g_name = "./output/clang/"+g_name
	print(g_name)
	os.popen(cmd_build0 + " {}".format(line)).read()
	os.popen(cmd_build1 + g_name).read()
	#cmd = clang_build + g_name + " {}".format(line)
	#print(cmd)
	#res = os.popen(cmd).read()
	#print(res)
	#quit()

cmd_build_cuda = "nvcc -ptx ./src/mainso.cu -o "
path = "./input/cuda.csv"

f = open(path,'r')
lines = f.readlines()
f.close()
cases = []
for line in lines:
	line = line.strip()
	#print(line)
	g_name = line.replace(" ","_").replace("=","").replace("-D","") + ".ptx"
	g_name = "./output/cuda/"+g_name
	print(g_name)
	os.popen(cmd_build_cuda + " {} {}".format(g_name,line)).read()



	#cases.append(line)
exit(0)




