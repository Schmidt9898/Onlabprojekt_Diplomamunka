#this script will do the cuda measurements for the parameters of this folder

import os
import re
import time
import sys

sys.path.append('../')

from SLtools import *
#from SLtools import get_gpu_temperature
#from SLtools import get_forward_time
#from SLtools import build_main
#from SLtools import test_run



#2,4,8,16,32,64
#128,256,512,1024

cases = import_cases(path = "./cases.csv",type = "cuda_tile")

case_number = len(cases)
case_count = get_case_number(path = "./case_number.cache")

print("Cases: " , case_number)
print("Start case id: " , case_count)

#temp_file_name = "temp_script_file.txt"

#creating names for files
measurement_name = "800_CUDA"
measurement_csv = measurement_name + "_results.csv"
measurement_summary = measurement_name + "_summary.log"

#so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature

# do this in the slurm job script
#os.popen("module list >> {}".format(measurement_summary)).read()
#os.popen("date >> {}".format(measurement_summary)).read()

f = None
if os.path.exists(measurement_csv):
	pass
	#f = open(measurement_csv, "a")
else:
	f = open(measurement_csv, "w")
	f.write("so,thread_limit,size,x,y,z,execution time,begin temperature,end temperature\n")
	f.close()

#init folders
os.popen("make clean").read()
os.popen("make init").read()

start_time = time.time()

#if needed do controll cases here


for i in range(case_count,len(cases)):
	case = cases[i]
	area,so,bx,by,bz,tx,ty,tz,x,y,z,thread_limit = case
	print("Case: %d/%d" % (case_count,case_number))
	begin_temp = get_gpu_temperature()
	print("temp:",begin_temp,"C")
	extra = "-DF{}_{} //-Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={} -Dtilesize_x={} -Dtilesize_y={} -Dtilesize_z={} -Dteamsize_x={} -Dteamsize_y={} -Dteamsize_z={} -DTHREADLIMIT={}".format(area,so,bx,by,bz,tx,ty,tz,x,y,z,thread_limit)
	#print(extra)
	build_main(extra)
	forward_time = 	test_run()
	end_temp = get_gpu_temperature()
	#print("forward took:",forward_time,"temperature",end_temp)
	
	f = open(measurement_csv, "a")
	f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,thread_limit,area,x,y,z,forward_time,begin_temp,end_temp))
	f.close()
	
	ellapsed = time.time()-start_time
	
	#print(ellapsed/60,"m ellapsed,",(ellapsed/case_count)*(case_number-case_count)/60,"m left")
	
	wait_for_gpu_60celsius()
	print("----------------------------------")
	case_count+=1
	set_case_number(case_count)
	#quit()
	#break

print("----------finished?------------")






