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

cases = import_cases(path = "./cases.csv",type = "cuda")



case_number = len(cases)
case_count = get_case_number(path = "./case_number.cache")

#set_case_number(5)


print("Cases: " , case_number)
print("Start case id: " , case_count)




quit()

#temp_file_name = "temp_script_file.txt"


measurement_name = "800_CUDA"
measurement_csv = measurement_name + "_results.csv"
measurement_summary = measurement_name + "_summary.log"

#so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature

os.popen("module list >> {}".format(measurement_summary)).read()
os.popen("date >> {}".format(measurement_summary)).read()


f = open(measurement_csv, "w")
f.write("so,thread_limit,size,x,y,z,execution time,begin temperature,end temperature\n")
f.close()

os.popen("make clean").read()
os.popen("make init").read()

start_time = time.time()

for so in space_orders:
	
	#begin_temp = get_gpu_temperature()
	for idx in sizes:
		
		x,y,z,thread_limit = idx
		print("Case: %d/%d" % (case_count,case_number))


		case_count+=1
		begin_temp = get_gpu_temperature()
		print("temperature",begin_temp)

		extra = "-DF{}_{} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={} -DTHREADLIMIT".format(area,so,x,y,z,t)

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






