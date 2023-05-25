import os

def get_forward_time(filename : str):
	time = -1.0
	try:
		f = open(filename, "r")
		f_str = f.read()
		f.close()
		time = float(re.search("section 0 (\d*.\d*) s",f_str).group(1))
	except Exception:
		pass
	return time


#calls the make file build_for_test function
def build(extra : str):
	command = "make EXTRA='{}' build".format(extra)
	print(command)
	os.popen(command).read()
def test(i):
	#command = "./bin/run ".format(i)
	os.popen("./bin/run> temp4time 2>&1").read()
	time = float(get_forward_time("temp4time"))
	#print(command)
	#result = os.popen(command).read()
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


