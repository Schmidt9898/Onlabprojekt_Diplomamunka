import os

#calls the make file build_for_test function
def build(extra : str):
	command = "make EXTRA='{}' bintel".format(extra)
	print(command)
	os.popen(command).read()
def test(i):
	command = "nvprof --csv --log-file result{}.csv  -m global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer ./bin/run ".format(i)
	print(command)
	result = os.popen(command).read()
	print("R",result)



cases =[
		(8,256,16,	4,	16	),
		(8,256,8,	8,	16	),
		(8,256,16,	2,	32	),
		(8,256,8,	2,	64	),
		(8,256,8,	4,	32	),
		(8,256,16,	1,	64	),
		(8,256,16,	8,	8	),
		(8,256,4,	4,	64	),
		(8,256,16,	1,	32	)]

for i,case in enumerate(cases):
	so,T,x,y,z = case
	build('-Dblocksize_x={} -Dblocksize_y={} -Dblocksize_z={}'.format(x,y,z))
	test(i)



#global_hit_rate,gld_requested_throughput,gst_requested_throughput,gld_throughput,gst_throughput,dram_read_throughput,dram_write_throughput,l2_read_throughput,l2_write_throughput,flop_count_sp,inst_executed,inst_issued,dram_read_bytes,dram_write_bytes,inst_integer