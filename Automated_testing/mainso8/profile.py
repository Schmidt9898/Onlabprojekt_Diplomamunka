import os

from Case_build import *

# I spent a day making this line
#ncu  --launch-skip 11 --launch-count 1 --replay-mode application --csv --log-file ezmi.log --section InstructionStats --section LaunchStats --section MemoryWorkloadAnalysis --section Occupancy --section SchedulerStats --section SourceCounters --section SpeedOfLight --section WarpStateStats ./bin/fornaiv/800_2_run

bases = [
	"-DFORNAIV -DF800_2 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_4 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_8 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_16 -DTHREADLIMIT=128"
]


def test(run_name):
	command = "ncu  --launch-skip 11 --launch-count 1 --replay-mode kernel --csv --log-file {}.csv --section InstructionStats --section LaunchStats --section MemoryWorkloadAnalysis --section Occupancy --section SchedulerStats --section SourceCounters --section SpeedOfLight --section WarpStateStats {} ".format(run_name,run_name)
	print(command)
	result = os.popen(command).read()
	print("R",result)


path_cases = "./ncu_profiler/best.csv"
cases = import_cases(path_cases)

case_cuda = import_cases("./ncu_profiler/bestcuda.csv")

path_bin = "./ncu_profiler/bin/"




#quit()

for case in bases:
	extra = case
	run_name = path_bin + case[12:19].strip()
	print(run_name)

	build_main(run_name,extra)
	test(run_name)

	#quit()

idx = 1
for case in case_cuda:
	#print(case)
	params = list(case)
	#so,T,x,y,z = case
	extra = params[0]
	run_name = ""
	for p in params[1:-1]:
		run_name += p+"_"
	#print(extra)
	run_name = path_bin + str(idx)+"_cuda_" +run_name + "run"
	print(run_name)
	#quit()
	
	#build_main(run_name,extra)
	cuda_build(run_name,extra)
	test(run_name)
	#build(extra)
	idx+=1
	if idx > 5: 
		idx = 1


for case in cases:
	#print(case)
	params = list(case)
	#so,T,x,y,z = case
	extra = params[0]
	run_name = ""
	for p in params[1:-1]:
		run_name += p+"_"
	#print(extra)
	run_name = path_bin + run_name + "run"
	#print(run_name)
	build_main(run_name,extra)
	test(run_name)
	#quit()
	#build(extra)



#global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer







#nvprof --csv --log-file result_cuda2.csv  -m global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp ./bin/run 
