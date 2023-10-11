
import matplotlib.pyplot as plt
import numpy as np
import os
import re
import time
import sys
from get_best_one import *



def create_plot(cuda,naiv,block,tilled,combined,name):

	type_ = [2,4,8,16]

	naivb = naiv

	if naiv is None:
		return

	#making them percentage

	if cuda != None:
		cuda = [ naivb[i]/v for i,v in enumerate(cuda)]
	if naiv != None:
		naiv = [ naivb[i]/v for i,v in enumerate(naiv)]
	if block != None:
		block = [ naivb[i]/v for i,v in enumerate(block)]
	if tilled != None:
		tilled = [ naivb[i]/v for i,v in enumerate(tilled)]
	if combined != None:
		combined = [ naivb[i]/v for i,v in enumerate(combined)]


	#cuda= [0.117462, 0.12976, 0.141029, 0.160823]
	#naiv= [0.212398, 0.256558, 0.394012, 1.515548]
	#block= [0.10506, 0.124851, 0.173042, 0.223866]
	#tilled= [0.095508, 0.156318, 0.226211, 0.243047]
	#combined= [0.08588, 0.111534, 0.179704, 0.227965]

	fig, ax = plt.subplots()
	if naiv != None:
		ax.plot(type_, naiv,'o-',label='Naiv',color="blue")
	if cuda != None:
		ax.plot(type_, cuda,'o-',label='Cuda',color="green")
	if block != None:
		ax.plot(type_, block,'o-',label='Block',color="red")
	if tilled != None:
		ax.plot(type_, tilled,'o-',label='Tile',color="yellow")
	if combined != None:
		ax.plot(type_, combined,'o-',label='Combined',color="purple")

	ax.set(xlabel='Space order', ylabel='speedup',
		title=name)
	#ax.set_ylim(0,0.8)
	ax.grid()
	ax.legend()

	#fig.savefig("test.png")
	return fig
		



print("Creating plots")


path = sys.argv[1]
#path = "D:/Programing/Gitted/Onlabprojekt_Diplomamunka/Measurements/MSC_theses/time_performance/results_other/Komondor/"
print(path)
folders = os.listdir(path)
print(folders)
for f in folders:
	fpath = path+f
	if os.path.isdir(fpath):
		print(fpath)
		pars = process_dir(fpath)
		fig = create_plot(*pars,f)
		if fig != None:
			fig.savefig(path + f +".png")




#plt.show()




