import os
import re
import time
import sys
import argparse

from SLtools import *



parser = argparse.ArgumentParser(
                    prog='MainSoN measurer',
                    description='Reads the cases from ./cases folder than compiles and runs the measurements. Cases can be interrupted and continued,saved in cache. Output is a csv file containing the proper measured values.',
                    epilog='Run with caution, may take several ours to complete.')

parser.add_argument('meas_type')  # positional argument, casefile to work with
parser.add_argument('-t', '--test',
                    action='store_true')  # on/off flag

args = parser.parse_args()
print(args.meas_type,args.test)


path_cache = "./bin/" + args.meas_type + ".cache"
path_cases = "./cases/" + args.meas_type + ".csv"
path_bin = "./bin/" + args.meas_type 
path_result = "./results/" + args.meas_type + ".csv"

print(path_cache,path_cases,path_bin )

#is_test = args.test

case_idx = get_case_number(path = path_cache)
cases = import_cases(path_cases)


print(case_idx)

for i in range(case_idx,len(cases)):
	case = cases[i]
	params = list(case)
	#so,T,x,y,z = case
	extra = params[0]
	run_name = ""
	for p in params[1:-1]:
		run_name += p+"_"
	#print(extra)
	run_name = path_bin + "/" + run_name + "run"
	#print(run_name)

	build_main(run_name,extra,args.test)
	forward_time = measure(run_name,1,args.test)
	print(forward_time)

	f = open(path_result, "a")
	f.write("%s,%s\n" % (params,forward_time))
	f.close()
	
	if args.test:
		continue
	case_idx+=1
	set_case_number(case_idx,path_cache)


