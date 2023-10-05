import os
import re
import time
import sys


flag_quick = False
#flag_test = True


def get_forward_time(filename : str):
	time = -1.0
	try:
		f = open(filename, "r")
		f_str = f.read()
		f.close()
		time = float(re.search("Kernel \d (\d*.\d*) sec",f_str).group(1))
	except Exception:
		pass
	return time

#calls the make file build_for_test function
def build_main(runname : str,extra : str,is_test = False):
	command = "make runname='{}' EXTRA='{}' build_for_test".format(runname,extra)
	print(command)
	if is_test:
		return
	os.popen(command).read()
	#os.remove("./cuda_temp.cu")

def measure(runname : str,n=1,is_test = False):
	if is_test:
		return -1
	time = 0.0
	temp_file_name = "{}_out.log".format(runname)
	for i in range(n):
		os.popen("timeout 10s {} > {} 2>&1".format(runname,temp_file_name) ).read()
		time += float(get_forward_time(temp_file_name))
	#os.remove("./bin/run")
	return str(time/n)


def create_test_cases(type):
	cases=[]
	areas = [800]
	space_orders = [2,4,8,16]
	block_threads = [-1]
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

	if type == "FORBLOCKTILLED":
		count_num = 0
		block_parts = [2,4,8,16,32,64]
		tile_parts = [1,2,4,8]
		block_threads = [128,256,512,1024]
		for area in areas:
			for so in space_orders:


				for bx in block_parts:
					for by in block_parts:
						for bz in block_parts:

							if bx == by and by == bz:
							#	continue
								pass

							for tx in tile_parts:
								for ty in tile_parts:
									for tz in tile_parts:
										if 1 == tx and 1 == ty and 1 == tz: # cubes are no good
											continue


										if bx < tx or by < ty or bz < tz: # cant be smaler than the tile
											continue
											pass

										for t in block_threads:

											block_size = bx*by*bz

											size = t*tx*ty*tz
											if size > 1024:
												continue

											if size != block_size:
												#print("wrong")
												continue
												pass
											#if Ax == Ay and Ay == Az:
											#	continue
											#if tx == ty and ty == tz:
											#	continue

											#if size <= 512 and size >= 128:
											#print((area,so,bx,by,bz,tx,ty,tz,size))
											#cases.append((area,so,bx,by,bz,tx,ty,tz,size))
											count_num+=1
											if count_num % 5 == 0:
												cases.append(("-D{} -DF{}_{} -DTHREADLIMIT={} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={} -Dtilesize_x={} -Dtilesize_y={} -Dtilesize_z={}".format(type,area,so,size,bx,by,bz,tx,ty,tz),area,so,size,bx,by,bz,tx,ty,tz))
												#cases.append((area,so,bx,by,bz,tx,ty,tz,size))


		#print("len",len(cases))
		#quit()
		return cases

	if type == "FORNAIV":
		threads = 128
		for area in areas:
			for so in space_orders:
				cases.append(("-DFORNAIV -DF{}_{} -DTHREADLIMIT={}".format(area,so,threads),area,so,threads))
		return cases






	if type == "FORBLOCKED":
		#this case we need the default cases
		block_threads = [128,256,512,1024]
		block_parts = [2,4,8,16,32,64]
		for area in areas:
			for so in space_orders:

				for t in block_threads:

					for x in block_parts:
						for y in block_parts:
							for z in block_parts:

								if x*y*z <= 2 * t:
									#cases.append((area,so,x,y,z,t))
									#count_num+=1
									#if count_num % 5 == 0:
									cases.append(("-D{} -DF{}_{} -DTHREADLIMIT={} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={}".format(type,area,so,t,x,y,z),area,so,t,x,y,z,x*y*z))

									#print(x,y,z,"=",x*y*z,t)

		return cases

	if type == "FORTILLED":
		#this case we need the default cases
		block_threads = [128,256,512,1024]
		block_parts = [1,2,4,8,16,32,64]
		count_num = 0
		for area in areas:
			for so in space_orders:

				for t in block_threads:

					for x in block_parts:
						for y in block_parts:
							for z in block_parts:

								if x*y*z*t <= 2048:
									#cases.append((area,so,x,y,z,t))
									#count_num+=1
									#if count_num % 5 == 0:
									cases.append(("-D{} -DF{}_{} -DTHREADLIMIT={} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={}".format(type,area,so,t,x,y,z),area,so,t,x,y,z,x*y*z*t))
								else:
									pass
									#print(("-D{} -DF{}_{} -DTHREADLIMIT={} -Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={}".format(type,area,so,t,x,y,z),area,so,t,x,y,z,x*y*z*t))

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


def import_cases(path = "./cases.csv"):
	f = open(path,'r')
	lines = f.readlines()
	f.close()
	cases = []
	for line in lines:
		#print(line)
		line = line.replace("[","").replace("]","").replace("'","")
		#print(line)
		#quit()
		line = line.strip()
		params = line.split(',')
		#case = [int(p) for p in params]
		case = tuple(params)
		#case = (int(params[0]),int(params[1]),int(params[2]),int(params[3]),int(params[4]),int(params[5]))
		cases.append(case)
	return cases

import os
import sys

class Case:
	def __init__(self,s):
		#print(len(s))
		self.list = s
		self.param = s[0]
		#self.param = s[1]
		self.so = int(s[2])
		self.threads = int(s[3])
		self.time = float(s[4])
		if len(s) > 5:
			self.x = int(s[4])
			self.y = int(s[5])
			self.z = int(s[6])
			self.size = int(s[7])
			self.time = float(s[8])
		if len(s) > 9:
			self.time = float(s[11])

	def __str__(self):
		val = ""
		for i in range(0,len(self.list)):
			val += str(self.list[i]) + ","
		val = val[0:-1]
		return val
		return self.param + "," + str(self.time)








if __name__ == "__main__":
	print("generate case files")
	results_folder = sys.argv[1]
	print(results_folder)
	selector='all'
	if len(sys.argv)>2:
		selector = sys.argv[2]

	N = 5

	if selector == 'all' or 'naiv' in selector:
		s = import_cases(results_folder + "/fornaiv.csv")
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))

		naiv = so
		for soi in [2,4,8,16]:
			for c in naiv[soi]:
				print(c)


	if selector == 'all' or 'blocked' in selector:
		s = import_cases(results_folder + "/forblocked.csv")
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))
		
		for soi in [2,4,8,16]:
			so[soi] = [ s for s in so[soi] if s.time >= 0]
		
			so[soi].sort(key=lambda a : a.time)
		
			so[soi] = [ so[soi][i] for i in range(N)]
		
		block = so
		for soi in [2,4,8,16]:
			for c in block[soi]:
				print(c)
	

	if selector == 'all' or 'tilled' in selector:
		s = import_cases(results_folder + "/fortilled.csv")
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))
	
		for soi in [2,4,8,16]:
			so[soi] = [ s for s in so[soi] if s.time >= 0]
	
			so[soi].sort(key=lambda a : a.time)
	
			so[soi] = [ so[soi][i] for i in range(5)]
	
		tilled = so


		for soi in [2,4,8,16]:
			for c in tilled[soi]:
				print(c)



	if selector == 'all' or 'blocktilled' in selector:
		s = import_cases(results_folder + "/forblocktilled.csv")
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))
	
		for soi in [2,4,8,16]:
			so[soi] = [ s for s in so[soi] if s.time >= 0]
	
			so[soi].sort(key=lambda a : a.time)
	
			so[soi] = [ so[soi][i] for i in range(5)]
	
		combined = so
	
		for soi in [2,4,8,16]:
			for c in combined[soi]:
				print(c)
	


