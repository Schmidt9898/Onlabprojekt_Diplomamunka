import sweetviz as sv
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


print("Creating plot")

#plt.rcParams['text.color'] = (1,1,1)

def plot_meas(df):
	columns = ["thread_limit", "x","y", "z","execution time"]
	space_order = [2,8,16]
	thread_limits = [128,256,512,1024]
	default_time = list (df["execution time"][np.where(df.thread_limit == -1)[0]])
	df_o = df
	soi = 0 
	tli = 0
	fig = plt.figure()

	for soi in range(len(space_order)):
		for tli in range(len(thread_limits)):
			indexes = np.where(df_o.so == space_order[soi])
			if len(indexes[0]) == 0:
				continue
			df = pd.DataFrame(df_o,index=indexes[0], columns=columns)
			df['bin'] = pd.cut(df["execution time"], [0, 0.5*default_time[soi], default_time[soi],2*default_time[soi],100],labels=["excelent", "good","bad","terrible"])
			df = df.reset_index(drop=True)
			indexes = np.where(df.thread_limit == thread_limits[tli])
			df = pd.DataFrame(df,index=indexes[0], columns=df.columns)
			df = df.reset_index(drop=True)

			#indexes = np.where(df["execution time"] < 2*default_time[soi])
			#df = pd.DataFrame(df,index=indexes[0], columns=df.columns)
			#df = df.reset_index(drop=True)

			ax = fig.add_subplot(len(space_order),len(thread_limits),soi*len(thread_limits)+tli+1,projection='3d')
			#ax.set_facecolor((0,0,0))

			df["execution time"] = df["execution time"] / ( 2*default_time[soi] )
			df["execution time"] = df["execution time"].clip(0, 1)

			print(df)

			ax.set_xlabel("X")
			ax.set_ylabel("Y")
			ax.set_zlabel("Z")
			#ax.set_xscale('symlog', base=2)
			#ax.set_yscale('symlog', base=2)
			#ax.set_zscale('symlog', base=2)
		#,label = df.bin
			ax.scatter(df.x, df.y, df.z,c= -df["execution time"] ,norm = "symlog" ,cmap = "RdYlGn", marker='o', s = 40)
	plt.show( block = False)
		#plt.wait()



def plot_meas_16(df):
	columns = ["thread_limit", "x","y", "z","execution time"]
	space_order = [16]
	thread_limits = [128]
	default_time = list (df["execution time"][np.where(df.thread_limit == -1)[0]])
	df_o = df
	soi = 0 
	tli = 0
	fig = plt.figure()

	for soi in range(1):
		for tli in range(len(thread_limits)):
			indexes = np.where(df_o.so == space_order[soi])
			if len(indexes[0]) == 0:
				continue
			df = pd.DataFrame(df_o,index=indexes[0], columns=columns)
			df['bin'] = pd.cut(df["execution time"], [0, 0.5*default_time[soi], default_time[soi],2*default_time[soi],100],labels=["excelent", "good","bad","terrible"])
			df = df.reset_index(drop=True)
			indexes = np.where(df.thread_limit == thread_limits[tli])
			df = pd.DataFrame(df,index=indexes[0], columns=df.columns)
			df = df.reset_index(drop=True)

			#indexes = np.where(df["execution time"] < 2*default_time[soi])
			#df = pd.DataFrame(df,index=indexes[0], columns=df.columns)
			#df = df.reset_index(drop=True)

			ax = fig.add_subplot(len(space_order),len(thread_limits),soi*len(thread_limits)+tli+1,projection='3d')
			#ax.set_facecolor((0,0,0))

			df["execution time"] = df["execution time"] / ( 2*default_time[soi] )
			df["execution time"] = df["execution time"].clip(0, 1)

			print(df)

			ax.set_xlabel("X")
			ax.set_ylabel("Y")
			ax.set_zlabel("Z")
			ax.scatter(df.x, df.y, df.z,c= -df["execution time"] ,norm = "symlog" ,cmap = "RdYlGn", marker='o', s = 40)
	plt.show( block = False)
		#plt.wait()


#df = pd.read_csv('800_measurement_14.6.csv')
#plot_meas(df)
#df = pd.read_csv('800_measurement_15.3.csv')
#plot_meas(df)
#df = pd.read_csv('800_tileclause_measurement.csv')
#plot_meas_16(df)


df = pd.read_csv('800_tileclause_measurement.csv')
default_time = list (df["execution time"][np.where(df.thread_limit == -1)[0]])
columns = ["so","thread_limit", "x","y", "z","execution time"]

space_order = [16]
default_time = list (df["execution time"][np.where(df.thread_limit == -1)[0]])


df_o = df
table = []
for i in range(1):
	soi = i
	indexes = np.where(df_o.so == space_order[soi])
	df = pd.DataFrame(df_o,index=indexes[0], columns=columns)
	df = df.reset_index(drop=True)
	indexes = np.where(df["execution time"] <= default_time[soi])
	df = pd.DataFrame(df,index=indexes[0], columns=df.columns)
	df = df.reset_index(drop=True)
	df["speedup"] = default_time[soi] / df["execution time"]
	df = df.sort_values(by=['execution time'])
	table.append(df.head(5))
	#print(df.head(5))

df = pd.concat(table, ignore_index=True)
print(df)
df.to_csv("builtin.csv",index=False)
#plt.show()

#report = sv.analyze(df)
#report.show_html("measurement_report.html",open_browser=True)
