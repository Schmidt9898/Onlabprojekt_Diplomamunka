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


	#0,Command line profiler metrics,smsp__sass_thread_inst_executed_op_fadd_pred_on.sum,inst,3361106880.0,0.0
	#1,Command line profiler metrics,smsp__sass_thread_inst_executed_op_ffma_pred_on.sum,inst,3361106880.0,0.0
	#2,Command line profiler metrics,smsp__sass_thread_inst_executed_op_fmul_pred_on.sum,inst,2688885504.0,0.0
	#3,Command line profiler metrics,smsp__sass_thread_inst_executed_op_fp32_pred_on.sum,inst,10755542016.0,0.0


	fadd,ffma,fmul,fp32 = (None,None,None,None)
	duration = 0
	drop_these = []
	for ind in data2.index:
		if "smsp__sass_thread_inst_executed_op_fadd_pred_on.sum" == data2['Metric Name'][ind]:
			fadd = int(data2['AverValue'][ind])
			drop_these.append(ind)
		if "smsp__sass_thread_inst_executed_op_ffma_pred_on.sum" == data2['Metric Name'][ind]:
			ffma = int(data2['AverValue'][ind])
			drop_these.append(ind)
		if "smsp__sass_thread_inst_executed_op_fmul_pred_on.sum" == data2['Metric Name'][ind]:
			fmul = int(data2['AverValue'][ind])
			drop_these.append(ind)
		if "smsp__sass_thread_inst_executed_op_fp32_pred_on.sum" == data2['Metric Name'][ind]:
			fp32 = int(data2['AverValue'][ind])
			#drop_these.append(ind)
		if "Duration" == data2['Metric Name'][ind]:
			duration = float(data2['AverValue'][ind])
		#print(data2['Metric Name'][ind], data2['AverValue'][ind])

	data2.loc[drop_these[0]] = ['Calculated','fadd + fmul + 2*ffma','flop',fadd + fmul + 2*ffma, 0] 		
	data2.loc[drop_these[1]] = ['Calculated','flops/duration','Gflop/second',float(fadd + fmul + 2*ffma) / duration , 0] 		#/ (duration/(10**9)) / (10**9)
	data2 = data2.drop(drop_these[2:])







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

