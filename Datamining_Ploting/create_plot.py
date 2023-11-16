
import matplotlib.pyplot as plt
import numpy as np
import os
import re
import time
import sys
from get_best_one import *

big_data = []
big_labels = []

def make_array(tomb,n):
	array = np.zeros(n) - 1
	if tomb != None:
		array = np.asarray(tomb)
	return array


#max_naiv = [0,0,0,0]

def create_plot(cuda,naiv,block,tilled,combined,name):

	type_ = [2,4,8,16]

	naivb = naiv

	if naiv is None:
		return
	
	max_naiv[0] = max(max_naiv[0],naiv[0])
	max_naiv[1] = max(max_naiv[1],naiv[1])
	max_naiv[2] = max(max_naiv[2],naiv[2])
	max_naiv[3] = max(max_naiv[3],naiv[3])

	#making them percentage

	#min()

	#if cuda != None:
	#	cuda = [ naivb[i]/v for i,v in enumerate(cuda)]
	#if naiv != None:
	#	naiv = [ naivb[i]/v for i,v in enumerate(naiv)]
	#if block != None:
	#	block = [ naivb[i]/v for i,v in enumerate(block)]
	#if tilled != None:
	#	tilled = [ naivb[i]/v for i,v in enumerate(tilled)]
	#if combined != None:
	#	combined = [ naivb[i]/v for i,v in enumerate(combined)]


	array = [
	make_array(cuda,4),
	make_array(naiv,4),
	make_array(block,4),
	make_array(tilled,4),
	make_array(combined,4)
		]
	
	labels = [	"cuda",
				name + " | naiv",
				name + " | block",
				name + " | tiled",
				name + " | combined"]
	big_labels.extend(labels)
	#array = np.asarray(array)

	big_data.append(array)
	#print("--->",array.shape)

	return 
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
		


def process_meases(cuda,naiv,block,tilled,combined,name,max_naiv):
	type_ = [2,4,8,16]

	naivb = naiv

	if naiv is None:
		return [],[]
	
	max_naiv[0] = max(max_naiv[0],naiv[0])
	max_naiv[1] = max(max_naiv[1],naiv[1])
	max_naiv[2] = max(max_naiv[2],naiv[2])
	max_naiv[3] = max(max_naiv[3],naiv[3])


	array = [
	make_array(cuda,4),
	make_array(naiv,4),
	make_array(block,4),
	make_array(tilled,4),
	make_array(combined,4)
		]
	
	labels = [	"cuda",
				name + " | naiv",
				name + " | block",
				name + " | tiled",
				name + " | combined"]
	#big_labels.extend(labels)
	#array = np.asarray(array)

	#big_data.append(array)
	#print("--->",array.shape)
	return array,labels


def get_data_computer(path):
	folders = os.listdir(path)
	print("get_data_computer",folders)
	max_naiv = [0,0,0,0]
	array = []
	labels = []
	for f in folders:
		fpath = path+"/"+f
		if os.path.isdir(fpath):
			print(fpath)
			pars = process_dir(fpath)
			a,l= process_meases(*pars,f,max_naiv)
			array.append(a)
			labels.extend(l)
			continue
	heatmap(array,labels,max_naiv, os.path.basename(path) )
	
def heatmap(big_data,big_labels,max_naiv,name):
	
	big_data = np.array(big_data)
	big_data = big_data.reshape (big_data.shape[0]*big_data.shape[1], 4)

	big_data_copy = big_data
	big_data = np.asarray(max_naiv) / big_data

	print(big_data.shape)
	print(big_labels)

	big_data_time_ = []
	big_data_ = []
	labels_ = []

	for i in range(len(big_data)):
		if big_data[i][0] >= 0:
			big_data_.append(big_data[i])
			labels_.append(big_labels[i])
			big_data_time_.append(big_data_copy[i])

	big_data = np.asarray(big_data_)
	big_labels = np.asarray(labels_)
	big_data_copy = np.asarray(big_data_time_)


	from matplotlib.colors import ListedColormap, LinearSegmentedColormap
	import matplotlib

	#viridis = cm.get_cmap('viridis', 12)
	#print(viridis)

	print("max",np.max(big_data))
	max_ = np.max(big_data)
	max_ = 5
	# 0 = red
	# 1 = yellow
	# max = green
	cidx_ = int (100 * max_)


	full_range = matplotlib.colormaps.get_cmap('RdYlGn')(np.linspace(0, 1, cidx_))
	badcolors = matplotlib.colormaps.get_cmap('RdYlGn')(np.linspace(0, 1, 200 ))
	goodcolors = matplotlib.colormaps.get_cmap('RdYlGn')(np.linspace(0, 1, 2*(cidx_ - 100 )))
	full_range[:100, :] = badcolors[:100, :]
	full_range[100:, :] = goodcolors[(cidx_ - 100 ):, :]
	newcmp = ListedColormap(full_range)



	#print(big_labels)
	#print(big_data)
	sos = ["so_2","so_4","so_8","so_16"]

	# Plot the heatmap
	fig, ax = plt.subplots(figsize=(10,10) )
	plt.imshow(big_data, cmap=newcmp, interpolation='nearest')
	plt.colorbar()
	plt.clim(0,max_) 
	ax.set_xticks(np.arange(len(sos)), labels=sos,rotation = 45)
	ax.set_yticks(np.arange(len(big_labels)), labels=big_labels)

	# Loop over data dimensions and create text annotations.
	for i in range(len(big_labels)):
		for j in range(len(sos)):
			if big_data[i, j] >= 0:
				text = ax.text(j, i, "%.2f" % big_data[i, j],
							ha="center", va="center", color="black")

	plt.title(name)


	fig.savefig(path + name +"_heatmap.png")



print("Creating plots")

#path = sys.argv[1]
path = "D:/Programing/Gitted/Onlabprojekt_Diplomamunka/Measurements/MSC_theses/time_performance/FinalResult/"
print(path)
folders = os.listdir(path)
#print(folders)
for f in folders:
	fpath = path+f
	if os.path.isdir(fpath):
		#print(fpath)
		get_data_computer(fpath)
		continue
		pars = process_dir(fpath)
		fig = create_plot(*pars,f)
		if fig != None:
			fig.savefig(path + f +".png")



plt.show()

exit()



#plt.show()




