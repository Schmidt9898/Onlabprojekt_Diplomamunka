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
		







if __name__ == "__main__":
	print("generate case files")

	results_folder = sys.argv[1]
	print(results_folder)


	N = 5

	s = import_cases(results_folder + "/fornaiv.csv")
	so = {2:[],4:[],8:[],16:[]}
	for i in s:
		so[int(i[2])].append(Case(i))
		
	naiv = so
	#for soi in [2,4,8,16]:
	#	for c in naiv[soi]:
	#		print(c)

	try:
		s = import_cases(results_folder + "/cuda.csv")
		so = {2:[],4:[],8:[],16:[]}
		for i in s:
			so[int(i[2])].append(Case(i))

		for soi in [2,4,8,16]:
			so[soi] = [ s for s in so[soi] if s.time >= 0]

			so[soi].sort(key=lambda a : a.time)

			so[soi] = [ so[soi][i] for i in range(N)]
			
		cuda = so
		for soi in [2,4,8,16]:
			for i,c in enumerate(cuda[soi]):
				print(str(c)+",cuda"+str(i+1))
	except:
		pass

	#quit()
	try:
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
			for i,c in enumerate(block[soi]):
				print(str(c)+",B"+str(i+1))
	except:
		pass


	try:
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
			for i,c in enumerate(tilled[soi]):
				print(str(c)+",T"+str(i+1))
	except:
		pass

	try:
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
			for i,c in enumerate(combined[soi]):
				print(str(c)+",C"+str(i+1))
	except:
		pass

	print("-"*20)

	print("type,2,4,8,16")
	cuda = [cuda[soi][0].time for soi in [2,4,8,16]]

	naiv = [naiv[soi][0].time for soi in [2,4,8,16]]
	block = [block[soi][0].time for soi in [2,4,8,16]]
	tilled = [tilled[soi][0].time for soi in [2,4,8,16]]
	combined = [combined[soi][0].time for soi in [2,4,8,16]]
	print("cuda=",cuda)
	print("naiv=",naiv)
	print("block=",block)
	print("tilled=",tilled)
	print("combined=",combined)







