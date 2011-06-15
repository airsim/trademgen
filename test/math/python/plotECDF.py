#!/usr/bin/env python

from scipy.stats import cumfreq
import matplotlib.pyplot as plt
import numpy as np

a = np.array ( [9, 1] )
num_bins = 1000

counts, bin_edges = np.histogram (a, bins=num_bins, normed=True)
cdf = np.cumsum (counts)
scale = 1.0 / cdf[-1]
ncdf = scale * cdf
plt.plot (bin_edges[1:], ncdf)

plt.show()

