## Apollo 11 lunar landing simulation

Clone the repository and compile the `rocket.cpp`, `planet.cpp`, `systemfinal.cpp` and `mainfinal.cpp` using a C++ compiler eg. gcc

```
g++ -std=c++23 mainfinal.cpp systemfinal.cpp rocket.cpp planet.cpp -o simulation
```

Run the simulation

```
./simulation
```

The result of the simulation will be txt files with the position and velocity values for the rocket, Earth and the Moon. To visualize the data I suggest using a program like gnuplot. Due to the sheer amount of data points, I visualized sections of the mission at a time instead of the entire thing at once.
