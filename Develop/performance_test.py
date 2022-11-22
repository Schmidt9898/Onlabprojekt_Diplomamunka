import os
import re
import time



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
print(len(sizes))

flags = "-O3 -g -fPIC -Wall -Wno-unused-result -Wno-unused-variable -ffast-math -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -lm"



start_time = time.time()
#sizes = [(4, 8, 32, 256)]
area = 800 #[336,560,720]
space_orders = [2,8,16]


case_number = len(sizes)*len(space_orders)
case_count = 1 

temp_file_name = "temp_script_file.txt"
measurement_filename = str(area)+"_measurement.csv"

#so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature


def get_temperature():
	temp_str = os.popen("nvidia-smi -q -d TEMPERATURE | grep \"GPU Current\" | tail -1").read()
	temp = -1
	try:
		temp = int(re.search("GPU Current Temp *: (\d*) C",temp_str).group(1))
	except AttributeError:
		pass
	return temp

def get_forward_time():
	time = "error"
	f = open(temp_file_name, "r")
	f_str = f.read()
	f.close()
	try:
		time = re.search("section 0 (\d*.\d*) s",f_str).group(1)
	except AttributeError:
		pass
	return time

def build_main(so,thread_limit=-1,x=-1,y=-1,z=-1):
	#print(os.environ['CC']+"" )
	parameters = " -DL_THREAD_LIMIT=%d -DBLOCKSIZE_X=%d -DBLOCKSIZE_Y=%d -DBLOCKSIZE_Z=%d " % (thread_limit,x,y,z)
	if thread_limit == -1:
		parameters = "" 
	command = os.environ['CC'] + " -Xopenmp-target -march=sm_80 " + flags + " -L "+os.environ['CLANG_HOME']+"/lib -lomptarget"+" ./temp_main.c -o ./temp_run -DSO"+str(so)+parameters
	print(command)
	os.popen(command).read()
	os.remove("./temp_main.c")
	#os.popen("./temp_run> "+temp_file_name+" 2>&1").read()

def test_run(n=1):
	time = 0.0
	for i in range(n):
		os.popen("./temp_run> "+temp_file_name+" 2>&1").read()
		time = float(get_forward_time())
	os.remove("./temp_run")
	return str(time/n)

	



f = open(measurement_filename, "w")
f.write("so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature\n")
f.close()


for so in space_orders:
	include_file = "#include \"./src_resource/forward_"+str(area)+"_"+str(so)
	tilled_include_file = include_file+"_tilled.c\""
	include_file = include_file + ".c\""

	print(include_file)
	print(tilled_include_file)

	f = open("perf_main.c", "r")
	file = include_file+"\n"+f.read()
	f.close()
	f = open("temp_main.c","w")
	f.write(file)
	f.close()
	
	
	begin_temp = get_temperature()
	
	#measurement unchanged 
	build_main(so)
	forward_time = 	test_run()


	end_temp = get_temperature()
	print("forward took:",forward_time,"temperature",end_temp)

	f = open(measurement_filename, "a")
	f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,-1,area,-1,-1,-1,forward_time,begin_temp,end_temp))
	f.close()


	for idx in sizes:
		
		x,y,z,thread_limit = idx
		print("Case: %d/%d" % (case_count,case_number))
		ellapsed = time.time()-start_time
		print(ellapsed/60,"s ellapsed,",(ellapsed/case_count)*(case_number-case_count)/60,"s left")


		case_count+=1
		
		begin_temp = get_temperature()
		print("temperature",begin_temp)

		f = open("perf_main.c", "r")
		file = tilled_include_file+"\n"+f.read()
		f.close()
		f = open("temp_main.c","w")
		f.write(file)
		f.close()



		build_main(so,thread_limit,x,y,z)
		forward_time = 	test_run()

		end_temp = get_temperature()

		print("forward took:",forward_time,"temperature",end_temp)
		f = open(measurement_filename, "a")
		f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,thread_limit,area,x,y,z,forward_time,begin_temp,end_temp))
		f.close()
		while True:
			temp_t = get_temperature()
			if temp_t>60:
				#for sec in range(5):
				#print("Temperature:",temp_t," C",end="\r")
				time.sleep(5)
				#print("60 sec cooling complete.")
			else:
				print("60 C  reached.")
				break
		print("----------------------------------")



print("----------finished?------------")






