
#DEVITO_LOGGING=DEBUG DEVITO_JIT_BACKDOOR=1 DEVITO_PLATFORM=nvidiaX DEVITO_ARCH=clang DEVITO_LANGUAGE=openmp python3 ./runme4.py >optout_so4 2>&1


import numpy as np
import os
import sys
import time
def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)
sizes = [[2, 16, 64, 256],
[4, 8, 32, 256],
[4, 4, 32, 256],
[4, 2, 32, 256],
[4, 8, 64, 512],
[4, 8, 64, 256],
[4, 2, 32, 128],
[8, 16, 8, 128],
[8, 16, 8, 256],
[8, 16, 16, 256],
[8, 8, 16, 512],
[8, 8, 16, 256],
[8, 4, 32, 256],
[8, 8, 32, 256],
[8, 4, 64, 256],
[8, 4, 32, 512],
[8, 8, 8, 256],
[8, 4, 64, 128],
[16, 2, 64, 256],
[16, 2, 64, 512],
[16, 4, 32, 256],
[16, 8, 16, 256],
[16, 16, 8, 256],
[16, 16, 8, 512],
[16, 4, 32, 128]]
filename = "/tmp/devito-jitcache-uid543800013/ee27e2a9bb4ecca90b1ba29fbc4c3e6b94bb47ea.c"
ctr = 0
for idx in sizes:
	ctr += 1
	i = idx[0]
	j = idx[1]
	k = idx[2]
	t = idx[3]
	print('---------------------------------',i,j,k,t,flush=True)
	os.popen("sed -i 's/blocksize_x =.*/blocksize_x = "+str(i)+";/g' "+filename)
	os.popen("sed -i 's/blocksize_y =.*/blocksize_y = "+str(j)+";/g' "+filename)
	os.popen("sed -i 's/blocksize_z =.*/blocksize_z = "+str(k)+";/g' "+filename)
	os.popen("sed -i 's/thread_limit(.*/thread_limit("+str(t)+")/g' "+filename)
	os.popen("nvidia-smi -q -d TEMPERATURE").read()
	if np.mod(ctr,2) == 0:
		os.popen("CUDA_VISIBLE_DEVICES=1 python3 ./acoustic_example.py -so 4 -d 400 400 400 -opt \"('advanced', {'linearize': True})\"").read()
		os.popen("CUDA_VISIBLE_DEVICES=0 python3 ./acoustic_example.py -so 4 -d 600 600 600 -opt \"('advanced', {'linearize': True})\"").read()
		os.popen("CUDA_VISIBLE_DEVICES=1 python3 ./acoustic_example.py -so 4 -d 800 800 800 -opt \"('advanced', {'linearize': True})\"").read()
	else:
		os.popen("CUDA_VISIBLE_DEVICES=0 python3 ./acoustic_example.py -so 4 -d 400 400 400 -opt \"('advanced', {'linearize': True})\"").read()
		os.popen("CUDA_VISIBLE_DEVICES=1 python3 ./acoustic_example.py -so 4 -d 600 600 600 -opt \"('advanced', {'linearize': True})\"").read()
		os.popen("CUDA_VISIBLE_DEVICES=0 python3 ./acoustic_example.py -so 4 -d 800 800 800 -opt \"('advanced', {'linearize': True})\"").read()
	print(os.popen("nvidia-smi -q -d TEMPERATURE").read(),flush=True)
	time.sleep(60)
`

