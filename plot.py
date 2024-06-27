import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

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

    x_list, y_list, z_list = [], [], []

    for i in range(0, len(lines), 3):
        x_list.append(lines[i])
        y_list.append(lines[i+1])
        z_list.append(lines[i+2])

    return x_list, y_list, z_list



earth_x, earth_y, earth_z = readlines("earth_generated.txt")[0], readlines("earth_generated.txt")[1], readlines("earth_generated.txt")[2]
moon_x, moon_y, moon_z = readlines("moon_generated.txt")[0], readlines("moon_generated.txt")[1], readlines("moon_generated.txt")[2]

fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.scatter(earth_x, earth_y, earth_z)
ax.scatter(moon_x, moon_y, moon_z)
ax.axis("equal")
plt.show()
