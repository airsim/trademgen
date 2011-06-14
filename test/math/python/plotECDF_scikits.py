#!/usr/bin/env python

import numpy as np
import scikits.statsmodels.tools as smt
import matplotlib.pyplot as plt

sample = np.random.uniform(0, 1, 50)
ecdf = smt.ECDF (sample)

x = np.linspace (min(sample), max(sample))
y = ecdf (x)
plt.step(x, y)

