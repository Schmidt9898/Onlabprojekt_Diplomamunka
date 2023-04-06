#	This script performs a measurements with the defined parameters.
#	Uses make to compile files defined with compiler parameters
#	Files used are defined in the makefile and the source files
#	
#	the script creates a bin folder for the runnables but every 
#	new state it gets deleted
#	
#	may create tsf.txt and result csv-s
#	
#	this sript works in this folder, threre fore can be used paralel


import os
import re
import time
import sys

sys.path.append('../')

from SLtools import *

cases = import_cases(path = "./tile_cases.csv",type = "omp_tile")

case_number = len(cases)
case_count = get_case_number(path = "./tile_case_number.cache")

print("Cases: " , case_number)
print("Start case id: " , case_count)


measurement_name = "800_OpenMP_tille"
measurement_csv = measurement_name + "_results.csv"
measurement_summary = measurement_name + "_summary.log"


#so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature


f = None
if os.path.exists(measurement_csv):
	pass
	#f = open(measurement_csv, "a")
else:
	f = open(measurement_csv, "w")
	f.write("so,thread_limit,size,x,y,z,execution time,begin temperature,end temperature\n")
	f.close()

os.popen("make clean").read()
os.popen("make init").read()

start_time = time.time()
#if needed do controll cases here

for area in [800]:
	for so in [2,4,8,16]:

		begin_temp = get_gpu_temperature()
		print("temperature",begin_temp)
		extra = "-DFORNAIV -DF{}_{} -DTHREADLIMIT={}".format(area,so,-1)
		build_main(extra)
		forward_time = 	test_run()
		end_temp = get_gpu_temperature()
		print("Default forward took:",forward_time,"temperature",end_temp)
		f = open(measurement_csv, "a")
		f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,-1,area,-1,-1,-1,forward_time,begin_temp,end_temp))
		f.close()
		ellapsed = time.time()-start_time

#quit()

for i in range(case_count,len(cases)):
	case = cases[i]
	area,so,x,y,z,thread_limit = case
	print("Case: %d/%d" % (case_count,case_number))
	begin_temp = get_gpu_temperature()
	print("temp:",begin_temp,"C")
	extra = "-DFORTILLED -DF{}_{} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={} -DTHREADLIMIT={}".format(area,so,x,y,z,thread_limit)
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
	set_case_number(case_count,path = "./tile_case_number.cache")
	#quit()
	#break

print("----------finished?------------")




