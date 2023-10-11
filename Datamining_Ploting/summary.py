print("Summary")



import os
import re
import time
import sys


import pandas as pd



class Frame:
	def __init__(self):
		self.headers = []
		self.rows = {}

def pd2dict(data):
	#data.drop(index='nan', level=1)
	#print(data)
	#quit()
	t = Frame()
	t.headers = ['Section Name', 'Metric Name','Metric Unit', 'Base']
	for i in range(len(data)):
		if str(data['Metric Name'][i]) != "nan":
			t.rows[data['Metric Name'][i]] = list(data.iloc[i])
			#print(data.iloc[i])
	#t = list(t)
	print(len(t.rows.keys()))
	#for k in t.rows.keys():
	#	print(t.rows[k])
	return t
	pass

def dict2pd(t):
	pad = list(t.rows.values())
	data = pd.DataFrame(pad,columns = t.headers)

	print(data)
	return data

def do_summary(path):







	bases = [
		"F800_2.csv",
		"F800_4.csv",
		"F800_8.csv",
		"F800_16.csv"]

	print(bases)

	#'ID', 'Process ID', 'Process Name', 'Host Name', 'Kernel Name',   
	#'Kernel Time', 'Context', 'Stream', 'Section Name', 'Metric Name',
	#'Metric Unit', 'AverValue', 'Rule Name', 'Rule Type',
	#'Rule Description'
	F800_2 = pd.read_csv(path+"/"+"F800_2.csv")[['Section Name', 'Metric Name','Metric Unit', 'AverValue']]
	F800_4 = pd.read_csv(path+"/"+"F800_4.csv")[['Section Name', 'Metric Name','Metric Unit', 'AverValue']]
	F800_8 = pd.read_csv(path+"/"+"F800_8.csv")[['Section Name', 'Metric Name','Metric Unit', 'AverValue']]
	F800_16 = pd.read_csv(path+"/"+"F800_16.csv")[['Section Name', 'Metric Name','Metric Unit', 'AverValue']]

	#print(F800_4[['Section Name', 'Metric Name','Metric Unit', 'AverValue']])s

	F800_2 = pd2dict(F800_2)
	F800_4 = pd2dict(F800_4)
	F800_8 = pd2dict(F800_8)
	F800_16 = pd2dict(F800_16)

	def add2dict(t,data,name):
		t.headers.append(name)
		for i in range(len(data)):
			if str(data['Metric Name'][i]) != "nan":
				t.rows[data['Metric Name'][i]].append(data['AverValue'][i])
		print(len(t.rows.keys()))
		#for k in t.rows.keys():
		#	print(t.rows[k])
		return t

	files = os.listdir(path)

	for f in files:
		if f in bases:
			continue
		file_ = path + "/" + f
		data = pd.read_csv(file_)[['Section Name', 'Metric Name','Metric Unit', 'AverValue']]

		if "800_2" in f:
			#print(file_)
			add2dict(F800_2,data,f)
			#F800_2[f] = data['AverValue']
		elif "800_4" in f:
			add2dict(F800_4,data,f)
			#F800_4[f] = data['AverValue']
			pass
		elif "800_8" in f:
			add2dict(F800_8,data,f)
			#F800_8[f] = data['AverValue']
			pass
		elif "800_16" in f:
			add2dict(F800_16,data,f)
			#F800_16[f] = data['AverValue']
			pass
		#data["diff"] = data['AverValue']-data["BASE"]
		#quit()


	F800_2 = dict2pd(F800_2)
	F800_4 = dict2pd(F800_4)
	F800_8 = dict2pd(F800_8)
	F800_16 = dict2pd(F800_16)


	F800_2.to_csv("./summary_F800_2.csv")
	F800_4.to_csv("./summary_F800_4.csv")
	F800_8.to_csv("./summary_F800_8.csv")
	F800_16.to_csv("./summary_F800_16.csv")


#path = "../Measurements/MSC_theses/ncu_profile/230921/temp"

if __name__ == "__main__":
	path = sys.argv[1]
	do_summary(path)
	#cleanup_csvs(sys.argv[1])









