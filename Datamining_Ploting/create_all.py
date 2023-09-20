
import os
import sys
from cleanup import cleanup_csvs,get_csvs
from cleanup import reduce_make_averdev


if __name__ == "__main__":


	root = sys.argv[1]

	scr_path = root + "/bin" 
	out_path = root + "/temp"
	if not os.path.exists(out_path):
		os.mkdir(out_path)
	


	cleanup_csvs(scr_path,out_path)

	csvs = get_csvs(out_path)

	for csv in csvs :
		reduce_make_averdev(csv)
		
	for csv in csvs :
		reduce_make_averdev(csv)
