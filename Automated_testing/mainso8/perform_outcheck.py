
import os
print("Output check")


def logthis(str : str):
	f = open("validity.log",'a')
	f.write(str+"\n")
	f.close()


bases = [
	"-DFORNAIV -DF800_2 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_4 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_8 -DTHREADLIMIT=128",
	"-DFORNAIV -DF800_16 -DTHREADLIMIT=128"
]


path = "./outcheck/profilercase.csv"

f = open(path,'r')
lines = f.readlines()
f.close()
cases = []
for line in lines:
	line = line.strip()
	params = line.split(',')
	case = tuple(params)
	cases.append(case)


param = "-DEXPORTDATA "


for par in bases:
	break
	#print(par)
	pars = par.split(" ")
	#pars[0][2:] +"_"+ 
	name = pars[1][2:]
	#print(name)
	#quit()
	#build
	command = "make runname='{}' EXTRA='{}' build_for_test".format("./outcheck/run",param+par)
	os.popen(command).read()
	#print(command)

	#run
	command = "./outcheck/run && mv ./out.bin ./outcheck/output_sample/"+name+".bin" 
	print(command)
	ret = os.popen(command).read()
#quit()

for par,time in cases:
	#print(par)
	pars = par.split(" ")
	name = pars[1][2:]
	print(name)
	#quit()
	#build
	command = "make runname='{}' EXTRA='{}' build_for_test".format("./outcheck/run",param+par)
	os.popen(command).read()

	#run
	command = "./outcheck/run" 
	print(os.popen(command).read())
	
	#check validity
	command = "/home/schmidtl/Onlabprojekt/meas_tools/bin/byte_valid ./out.bin ./outcheck/output_sample/" + name+".bin " 
	print(command)
	ret = os.popen(command).read()
	sol = ""
	if "PASSED" in ret:
		sol = par + " --> PASSED"
	else:
		sol = par + " --> FAILED"

		par + " --> PASSED"

	print(ret)
	print(sol)
	logthis(sol)
	#quit()
	#print(command)





