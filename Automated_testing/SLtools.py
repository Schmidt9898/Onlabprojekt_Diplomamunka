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
