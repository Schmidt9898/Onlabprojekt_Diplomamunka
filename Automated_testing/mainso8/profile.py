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
	#old
	#command = "ncu  --launch-skip 11 --launch-count 1 --replay-mode kernel --csv --log-file {}.csv --section InstructionStats --section LaunchStats --section MemoryWorkloadAnalysis --section Occupancy --section SchedulerStats --section SourceCounters --section SpeedOfLight --section WarpStateStats {} ".format(run_name,run_name)
	#new Reguly
	command = "nv-nsight-cu-cli --launch-skip 11 --launch-count 10 --replay-mode kernel --csv --log-file {}.csv --section ComputeWorkloadAnalysis --section InstructionStats --section LaunchStats --section MemoryWorkloadAnalysis --section MemoryWorkloadAnalysis_Chart --section MemoryWorkloadAnalysis_Deprecated --section MemoryWorkloadAnalysis_Tables --section Occupancy --section SchedulerStats --section SourceCounters --section SpeedOfLight --section SpeedOfLight_HierarchicalDoubleRooflineChart --section SpeedOfLight_RooflineChart --section WarpStateStats --metrics smsp__sass_thread_inst_executed_op_fp32_pred_on.sum,smsp__sass_thread_inst_executed_op_integer_pred_on.sum,smsp__sass_thread_inst_executed_op_fadd_pred_on.sum,smsp__sass_thread_inst_executed_op_ffma_pred_on.sum,smsp__sass_thread_inst_executed_op_fmul_pred_on.sum {}".format(run_name,run_name)
	
	print(command)
	result = os.popen(command).read()
	print("R",result)


path_cases = "./ncu_profiler/best.csv"
cases = import_cases(path_cases)

case_cuda = import_cases("./ncu_profiler/bestcuda.csv")

path_bin = "./ncu_profiler/bin/"




#quit()

for case in bases:
	#break
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
	for p in params[1:-2]:
		run_name += p+"_"
	run_name = params[-1]+"_"+run_name
	#print(extra)
	run_name = path_bin + run_name + "run"
	#print(run_name)
	#continue

	#quit()
	
	#build_main(run_name,extra)
	cuda_build(run_name,extra)
	test(run_name)
	#build(extra)


for case in cases:
	#print(case)
	params = list(case)
	#so,T,x,y,z = case
	extra = params[0]
	run_name = ""
	for p in params[1:-2]:
		run_name += p+"_"
	run_name = params[-1]+"_"+run_name

	#print(extra)
	run_name = path_bin + run_name + "run"
	#print(run_name)
	#continue

	build_main(run_name,extra)
	test(run_name)
	#quit()
	#build(extra)



#global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer







#nvprof --csv --log-file result_cuda2.csv  -m global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp ./bin/run 
