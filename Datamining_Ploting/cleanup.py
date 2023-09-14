import os
import re
import time
import sys



def cleanup_csvs(path):
	files = os.listdir(path)
	
	for f in files:
		if ".csv" in f:
			file_ = path + "/" + f
			print(file_)


		
		# Using readlines()
		file1 = open(file_, 'r')
		Lines = file1.readlines()
		file1.close()
		cleaned = []
		
		for line in Lines:
			if not "==PROF==" in line:
				cleaned.append(line)
				#print(line)


		# writing to file
		file1 = open(file_, 'w')
		file1.writelines(cleaned)
		file1.close()

	



if __name__ == "__main__":
	cleanup_csvs(sys.argv[1])

