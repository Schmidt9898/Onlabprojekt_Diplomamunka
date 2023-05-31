import os

from SLtools import *




#calls the make file build_for_test function
def build(extra : str):
	command = "make EXTRA='{}' build".format(extra)
	print(command)
	os.popen(command).read()
def test(i):
	command = "nvprof --csv --log-file result{}.csv  -m global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer ./bin/run ".format(i)
	print(command)
	result = os.popen(command).read()
	print("R",result)


path_cases = "./cases/fornaiv.csv"
cases = import_cases(path_cases)




case_idx = 0

for i in range(case_idx,len(cases)):
	case = cases[i]
	params = list(case)
	#so,T,x,y,z = case
	extra = params[0]
	build(extra)
	test(i)


#global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer