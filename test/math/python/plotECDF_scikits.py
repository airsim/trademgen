#!/usr/bin/env python

import numpy as np
import scikits.statsmodels.api as sm
#import scikits.statsmodels.tools.ECDF
from scikits.statsmodels.tools.tools import ECDF
import matplotlib.pyplot as plt

sample = np.random.uniform(0, 1, 50)
#sample = np.array ([9, 1])
ecdf = ECDF (sample)

print 'ecdf: ', str(ecdf)

fig = plt.figure()
ax = fig.add_subplot(111)
#ax.axis ((0, 10, 0, 2))
ax.plot (ecdf)

#x = np.linspace (min(sample), max(sample))
#y = ecdf (x)

#plt.step(x, y)
plt.show()

