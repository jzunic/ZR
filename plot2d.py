import matplotlib.pyplot as plt
import numpy as np

def readlines(file_name):
    with open(file_name) as f:
        lines = f.readlines()

    for i in range(len(lines)):
        for j in range(len(lines[i])):
            number = ""

            if lines[i][j] == "\n":
                for k in range(len(lines[i])-1):
                    number += lines[i][k]
                lines[i] = number

    lines = [float(line) for line in lines]

    x_list, y_list = [], []

    for i in range(0, len(lines), 3):
        x_list.append(lines[i])
        y_list.append(lines[i+1])

    return x_list, y_list

earth_x, earth_y = readlines("earth_cm.txt")[0], readlines("earth_cm.txt")[1]
moon_x, moon_y = readlines("moon_cm.txt")[0], readlines("moon_cm.txt")[1]

for i in range( len(earth_x)):
    print(earth_x[i], earth_y[i])


plt.plot(earth_x, earth_y)
plt.plot(moon_x, moon_y)
plt.axis("equal")
plt.show()
