import os
import re
import time



#2,4,8,16,32,64
#128,256,512,1024
block_threads = [-1]
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


start_time = time.time()
#sizes = [(4, 8, 32, 256)]
area = 800 #[336,560,720]
space_orders = [16]


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
	command = "make test_build"
	print(command)
	os.popen(command).read()
	#os.remove("./cuda_temp.cu")

def test_run(n=1):
	time = 0.0
	for i in range(n):
		os.popen("./bin/run> "+temp_file_name+" 2>&1").read()
		time = float(get_forward_time())
	os.remove("./bin/run")
	return str(time/n)

	



f = open(measurement_filename, "w")
f.write("so,thread_limit,iteration,x,y,z,execution time, begin temperature,end temperature\n")
f.close()

os.popen("make clean").read()
os.popen("make init").read()


for so in space_orders:
	
	begin_temp = get_temperature()


	for idx in sizes:
		
		x,y,z,thread_limit = idx
		print("Case: %d/%d" % (case_count,case_number))



		case_count+=1
		
		begin_temp = get_temperature()
		print("temperature",begin_temp)

		define = "#define blocksize_x {}\n#define blocksize_y {}\n#define blocksize_z {}\n".format(x,y,z)

		f = open("cuda_section0.cu", "r")
		file = define+"\n"+f.read()
		f.close()
		f = open("cuda_temp.cu","w")
		f.write(file)
		f.close()



		build_main(so,thread_limit,x,y,z)
		forward_time = 	test_run()

		end_temp = get_temperature()

		print("forward took:",forward_time,"temperature",end_temp)
		f = open(measurement_filename, "a")
		f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (so,thread_limit,area,x,y,z,forward_time,begin_temp,end_temp))
		f.close()

		ellapsed = time.time()-start_time
		print(ellapsed/60,"m ellapsed,",(ellapsed/case_count)*(case_number-case_count)/60,"m left")


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






