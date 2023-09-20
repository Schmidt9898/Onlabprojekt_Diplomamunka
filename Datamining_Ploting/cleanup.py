import os
import re
import time
import sys



import pandas as pd
import locale
import statistics
import numpy as np
locale.setlocale( locale.LC_ALL, 'en_US.UTF-8' )


def get_csvs(path):
	files = os.listdir(path)
	
	csvs = []
	for f in files:
		if ".csv" in f:
			csvs.append(path + "/" + f)
	return csvs


def reduce_make_averdev(path):
	#print(path)
	data = pd.read_csv(path)[['Section Name', 'Metric Name','Metric Unit', 'Metric Value']]
	#data2 = pd.read_csv(path)[['Metric Name', 'Metric Value']]

	list_ = data.values.tolist()

	tree = {}
	tree_rest = {}

	for elem in list_:
		section,name,unit,vals = elem
		val = None
		try:
			pass
			if '.' in vals: # it is a float
				val = locale.atof(vals)
			else:
				val = locale.atoi(vals)
		except:# string value
			#print("string:",vals)
			continue
			pass
		if name in tree.keys():
			tree[name].append(val)
		else:
			tree_rest[name] = (section,name,unit)
			tree[name] = [val]			
	#print(tree)

	data2 = []
	for name in tree.keys():
		values = tree[name]
		#print(name,values[0])
		aver_ =  np.average(values)
		stdev_ =  statistics.stdev(values)
		l = list(tree_rest[name])
		l.append(aver_)
		l.append(stdev_)

		#print(l)
		data2.append(l)
	data2 = pd.DataFrame(data2,columns = ['Section Name', 'Metric Name','Metric Unit', 'AverValue','stdev'])
	#print(data2)
	data2.to_csv(path)
	#quit()
	







def cleanup_csvs(path,out_path):
	files = os.listdir(path)
	
	for f in files:
		if ".csv" in f:
			infile = path + "/" + f
			outfile = out_path + "/" + f
			print(infile)
			# Using readlines()
			file1 = open(infile, 'r')
			Lines = file1.readlines()
			file1.close()
			cleaned = []
			
			for line in Lines:
				if not "==PROF==" in line:
				
					#11 idx

					parts = line.split('","')
					if parts[11] != "":
						#print(parts[11])
						cleaned.append(line)
					#print(line)


			# writing to file
			file2 = open(outfile, 'w')
			file2.writelines(cleaned)
			file2.close()


if __name__ == "__main__":
	cleanup_csvs(sys.argv[1])

