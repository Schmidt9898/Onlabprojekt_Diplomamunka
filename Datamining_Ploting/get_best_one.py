import os
import re
import time
import sys


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
		



def process_file(filename):
	try:
		N = 1
		s = import_cases(filename)
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))
		for soi in [2,4,8,16]:
			so[soi] = [ s for s in so[soi] if s.time >= 0]
			so[soi].sort(key=lambda a : a.time)
			so[soi] = [ so[soi][i] for i in range(N)]
		return so
	except:
		return None
	
		



def process_dir(path):


	naiv = process_file(path + "/fornaiv.csv")

	cuda = process_file(path + "/cuda.csv")

	block = process_file(path + "/forblocked.csv")

	tilled = process_file(path + "/fortilled.csv")

	combined = process_file(path + "/forblocktilled.csv")


	print("type,2,4,8,16")
	cuda = [cuda[soi][0].time for soi in [2,4,8,16]] if cuda != None else None
	naiv = [naiv[soi][0].time for soi in [2,4,8,16]] if naiv != None else None
	block = [block[soi][0].time for soi in [2,4,8,16]] if block != None else None
	tilled = [tilled[soi][0].time for soi in [2,4,8,16]] if tilled != None else None
	combined = [combined[soi][0].time for soi in [2,4,8,16]] if combined != None else None
	print("cuda=",cuda)
	print("naiv=",naiv)
	print("block=",block)
	print("tilled=",tilled)
	print("combined=",combined)

	return cuda,naiv,block,tilled,combined









if __name__ == "__main__":
	print("generate case files")

	path = sys.argv[1]
	print(path)

	folders = os.listdir(path)
	print(folders)

	for f in folders:
		fpath = path+f
		if os.path.isdir(fpath):
			print(fpath)
			process_dir(fpath,f)








