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
#	with other scripts for measurements


import os
import re
import time
import sys

sys.path.append('../')

from SLtools import *

#2,4,8,16,32,64
#128,256,512,1024
block_threads = [128,256,512,1024]
block_parts = [2,4,8,16,32,64]
sizes=[]
for t in block_threads:
	for x in block_parts:
		for y in block_parts:
			for z in block_parts:
				if x*y*z <= 1024:
					sizes.append((x,y,z,t))
					#print(x,y,z,"=",x*y*z,t)


#flags = "-O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm"



start_time = time.time()
#sizes = [(4, 8, 32, 256)]
area = 800 #[336,560,720]
space_orders = [2,4,8,16]


case_number = len(sizes)*len(space_orders) + 4

print("case_numbers",case_number)

case_count = 1 

measurement_name = str(area)+"_OpenMP_block"
measurement_csv = measurement_name + "_results.csv"
measurement_summary = measurement_name + "_summary.log"


#so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature


os.popen("module list > {}".format(measurement_summary)).read()
os.popen("date >> {}".format(measurement_summary)).read()


f = open(measurement_csv, "w")
f.write("so,thread_limit,size,x,y,z,execution time,begin temperature,end temperature\n")
f.close()

os.popen("make clean").read()
os.popen("make init").read()


for so in space_orders:

	case_count+=1
	begin_temp = get_gpu_temperature()
	print("temperature",begin_temp)
	extra = "-DFORNAIV -DF{}_{} -DTHREADLIMIT={}".format(area,so,t)
	#print(extra)
	build_main(extra)
	forward_time = 	test_run()
	end_temp = get_gpu_temperature()
	print("Default forward took:",forward_time,"temperature",end_temp)
	f = open(measurement_csv, "a")
	f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,-1,area,-1,-1,-1,forward_time,begin_temp,end_temp))
	f.close()
	ellapsed = time.time()-start_time

	#continue
	
	#begin_temp = get_gpu_temperature()
	for idx in sizes:
		
		x,y,z,thread_limit = idx
		print("Case: %d/%d" % (case_count,case_number))
		case_count+=1


		begin_temp = get_gpu_temperature()
		print("temperature",begin_temp)
		#FORTILLED
		extra = "-DFORBLOCKED -DF{}_{} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={} -DTHREADLIMIT={}".format(area,so,x,y,z,t)

		#print(extra)
		build_main(extra)
		forward_time = 	test_run()

		end_temp = get_gpu_temperature()

		print("forward took:",forward_time,"temperature",end_temp)
		f = open(measurement_csv, "a")
		f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,thread_limit,area,x,y,z,forward_time,begin_temp,end_temp))
		f.close()

		ellapsed = time.time()-start_time
		print(ellapsed/60,"m ellapsed,",(ellapsed/case_count)*(case_number-case_count)/60,"m left")


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
		print("----------------------------------")
		#quit()
		#break



os.popen("date >> {}".format(measurement_summary)).read()
print("----------finished?------------")



