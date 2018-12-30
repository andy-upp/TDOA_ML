import numpy as np
import matplotlib.pyplot as plt
import csv
import math
import sys

figname = sys.argv[1]

f = open("converge.csv", 'r')
data = csv.reader(f)
conv_x, conv_y = [], []
for cell in data:
    conv_x.append(round(float(cell[0]), 4))
    conv_y.append(round(float(cell[1]), 4))
f.close()

f1 = open("unconv_norm.csv", 'r')
data1 = csv.reader(f1)
unconv_n_x, unconv_n_y = [], []
for cell in data1:
    unconv_n_x.append(round(float(cell[0]), 4))
    unconv_n_y.append(round(float(cell[1]), 4))
f1.close()

f2 = open("unconv_other.csv", 'r')
data2 = csv.reader(f2)
unconv_o_x, unconv_o_y = [], []
for cell in data2:
    unconv_o_x.append(round(float(cell[0]), 4))
    unconv_o_y.append(round(float(cell[1]), 4))
f2.close()

f3 = open("config_record.csv", 'r')
data3 = csv.reader(f3)
config_x, config_y = [], []
for cell in data3:
    config_x.append(round(float(cell[0]), 4))
    config_y.append(round(float(cell[1]), 4))

f4 = open("convToOtherRecord.csv", 'r')
data4 = csv.reader(f4)
conv_other_x, conv_other_y = [], []
for cell in data4:
    tmpX = round(float(cell[0]), 4)
    tmpY = round(float(cell[1]), 4)
    if tmpX**2 + tmpY**2 < 255:
        conv_other_x.append(round(float(cell[0]), 4))
        conv_other_y.append(round(float(cell[1]), 4))

plt.figure(figsize=(12, 9))
plt.plot(conv_x, conv_y, 'g+')
plt.plot(unconv_n_x, unconv_n_y, 'kx')
plt.plot(unconv_o_x, unconv_o_y, 'bx')
plt.plot(config_x[0], config_y[0], 'r*', markersize=13)
plt.plot(conv_other_x, conv_other_y, "k*", markersize=13)
plt.plot(config_x[1:], config_y[1:], 'ro', markersize=13)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Convergency Map")
plt.legend(loc=1, bbox_to_anchor=(1, 1),labels=("converge", "diverge",
            "converge to other point", "real target", "Wrong Conv", 
            "receivers"))
plt.grid(True)
# plt.show()
plt.savefig(figname)



