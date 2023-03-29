import os
import re
import time
import sys


flag_quick = False


#reurns the current gpu temperature if error raised returns -1
def get_gpu_temperature(): 
	global flag_quick
	if flag_quick:
		return -1
	temp_str = os.popen("nvidia-smi -q -d TEMPERATURE | grep \"GPU Current\" | tail -1").read() # This command is costly (takes a long time) but important
	temp = -1
	try:
		temp = int(re.search("GPU Current Temp *: (\d*) C",temp_str).group(1))
	except AttributeError:
		pass
	return temp


def wait_for_gpu_60celsius():
	while True:
		temp_t = get_gpu_temperature()
		if temp_t>60:
			#for sec in range(5):
			#print("Temperature:",temp_t," C",end="\r")
			time.sleep(5)
			#print("60 sec cooling complete.")
		else:
			print("60 C  reached.")
			break


#reads section 0 durationtime from filename file, in case of error returns -1
def get_forward_time(filename : str):
	time = -1.0
	try:
		f = open(filename, "r")
		f_str = f.read()
		f.close()
		time = float(re.search("section 0 (\d*.\d*) s",f_str).group(1))
	except Exception:
		pass
	return time

#calls the make file build_for_test function
def build_main(extra : str):
	command = "make EXTRA='{}' build_for_test".format(extra)
	print(command)
	os.popen(command).read()
	#os.remove("./cuda_temp.cu")

def test_run(n=1):
	global flag_quick
	if flag_quick:
		return -1
	time = 0.0
	temp_file_name = "tsf.txt"
	for i in range(n):
		os.popen("./bin/run> "+temp_file_name+" 2>&1").read()
		time += float(get_forward_time(temp_file_name))
	#os.remove("./bin/run")
	return str(time/n)


def create_test_cases(type):
	cases=[]
	areas = [800]
	space_orders = [2,4,8,16]
	if type == "cuda":
		block_threads = [-1] # for cuda thread limit is defined by x*y*z
		block_parts = [2,4,8,16,32,64]

	if type == "cuda_tile":
		block_parts = [2,4,8,16,32,64]
		tile_parts = [1,2,4]
		for area in areas:
			for so in space_orders:
				for bx in block_parts:
					for by in block_parts:
						for bz in block_parts:
							if bx == by and by == bz:
								continue
							for tx in tile_parts:
								for ty in tile_parts:
									for tz in tile_parts:
										Ax,Ay,Az = (bx*tx,by*ty,bz*tz)
										size = Ax*Ay*Az
										if Ax == Ay and Ay == Az:
											continue
										if tx == ty and ty == tz:
											continue
										if size <= 512 and size >= 128:
											cases.append((area,so,bx,by,bz,tx,ty,tz,Ax,Ay,Az,size))
		return cases
	

	if type == "openmp":
		#this case we need the default cases
		block_threads = [128,256,512,1024]
		block_parts = [2,4,8,16,32,64]
	if type == "omp_tile":
		#this case we need the default cases
		block_threads = [128,256,512,1024]
		block_parts = [1,2,4,8,16,32,64]
	for area in areas:
		for so in space_orders:
			# here we need extra cases for the control cases or we do that separatly, yeah lets do that
			for t in block_threads:
				for x in block_parts:
					for y in block_parts:
						for z in block_parts:
							if x*y*z <= 1024:
								cases.append((area,so,x,y,z,t))
								#print(x,y,z,"=",x*y*z,t)
	return cases 

def export_cases(cases,path = "./cases.csv"):
	f = open(path,'w')
	for case in cases:
		#print(case)
		case = list(case)
		line = ""
		for p in case:
			line+=str(p)+","
		line = line[0:-1]
		f.write(line+"\n")
	f.flush()
	f.close()


def import_cases(path = "./cases.csv",type = "openmp"):
	if not os.path.exists(path):
		cases = create_test_cases(type)
		export_cases(cases,path)
		return cases ##early exit
	f = open(path,'r')
	lines = f.readlines()
	f.close()
	cases = []
	for line in lines:
		line = line.strip()
		params = line.split(',')
		case = [int(p) for p in params]
		case = tuple(case)
		#case = (int(params[0]),int(params[1]),int(params[2]),int(params[3]),int(params[4]),int(params[5]))
		cases.append(case)
	return cases

import os

def get_case_number(path = "./case_number.cache"):
	value = 0
	line = ""
	if os.path.exists(path): 
		f = open(path,'r')
		line = f.readline()
		f.close()
		line=line.strip()
	if line != "":
		return int(line)
	return value

def set_case_number(n,path = "./case_number.cache"):
	f = open(path,'w')
	f.write(str(n))
	f.flush()
	f.close()






