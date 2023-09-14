import os
import re
import time

# Print iterations progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
	"""
	Call in a loop to create terminal progress bar
	@params:
		iteration   - Required  : current iteration (Int)
		total	   - Required  : total iterations (Int)
		prefix	  - Optional  : prefix string (Str)
		suffix	  - Optional  : suffix string (Str)
		decimals	- Optional  : positive number of decimals in percent complete (Int)
		length	  - Optional  : character length of bar (Int)
		fill		- Optional  : bar fill character (Str)
		printEnd	- Optional  : end character (e.g. "\r", "\r\n") (Str)
	"""
	percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
	filledLength = int(length * iteration // total)
	bar = fill * filledLength + '-' * (length - filledLength)
	print(f'\r{prefix} |{bar}| {percent}% {suffix}', end = printEnd)
	# Print New Line on Complete
	if iteration == total: 
		print()




sizes = [
[4, 8, 32, 256],
[4, 4, 32, 256],
[4, 2, 32, 256],
[8, 16, 8, 256],
[8, 16, 16, 256],
[8, 8, 16, 512],
[8, 8, 16, 256],
[8, 4, 32, 256],
[8, 8, 32, 256],
[8, 4, 32, 512],
[8, 8, 8, 256],
[16, 4, 32, 256],
[16, 8, 16, 256],
[16, 16, 8, 256],
[16, 16, 8, 512]]


padding = 80
areas = [336,560,720]
space_orders = [4,8,16]

case_number = len(sizes)*len(areas)*len(space_orders)
case_count = 1 

temp_file_name = "temp_file.txt"
measurement_filename = "measurement.csv"

#so,limit,size,x,y,z,execution time, begin temperature,end temperature



def get_temperature():
	temp_str = os.popen("nvidia-smi -q -d TEMPERATURE").read()
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
		time = re.search("Operator `Forward` ran in (\d*.\d*) s",f_str).group(1)
	except AttributeError:
		pass
	return time




f = open(measurement_filename, "w")
f.write("so,limit,size,x,y,z,execution time, begin temperature,end temperature\n")
f.close()


#os.environ.get('var')
#os.environ['var'] = "asd"
#printProgressBar(case_count,case_number, prefix = 'Progress:', suffix = 'Complete', length = 50)
for space_order in space_orders:
	for cur_area in areas:
		size_str = (str(cur_area)+" ")*3
		for idx in sizes:
			
			#printProgressBar(case_count,case_number, prefix = 'Progress:', suffix = 'Complete', length = 50)
			print("Case: %d/%d" % (case_count,case_number))
			case_count+=1

			os.environ['BLKS_X'] = str(idx[0])
			os.environ['BLKS_Y'] = str(idx[1])
			os.environ['BLKS_Z'] = str(idx[2])
			limit = idx[3]

			begin_temp = get_temperature()
			print("temperature",begin_temp)
			
			command = "DEVITO_LOGGING=DEBUG DEVITO_JIT_BACKDOOR=0 DEVITO_PLATFORM=nvidiaX DEVITO_ARCH=clang DEVITO_LANGUAGE=openmp python ./acoustic_example.py -so %d -d %s -opt \"('advanced', {'linearize': True,'omp_limit':%d})\" " % (space_order,size_str,limit)

			print(command)

			os.popen(command + "> "+temp_file_name+" 2>&1").read()

			end_temp = get_temperature()

			forward_time = get_forward_time()

			print("forward took:",forward_time,"temperature",end_temp)

			f = open(measurement_filename, "a")
			f.write("%d,%d,%d,%d,%d,%d,%s,%d,%d\n" % (space_order,limit,cur_area,idx[0],idx[1],idx[2],forward_time,begin_temp,end_temp))
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






