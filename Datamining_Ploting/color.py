from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
import matplotlib
import numpy as np
#viridis = cm.get_cmap('viridis', 12)
#print(viridis)

#print("max",np.max(big_data))
#smax_ = np.max(big_data)
# 0 = red
# 1 = yellow
# max = green
cidx_ = 128 #int ((0.5) * 256)




viridis = matplotlib.colormaps.get_cmap('viridis')
a = viridis(np.linspace(0, 1, 256))
newcolors = viridis(np.linspace(0, 1, 10))
pink = np.array([248/256, 24/256, 148/256, 1])
a[:cidx_,:] = newcolors
newcmp = ListedColormap(newcolors,"laci")

