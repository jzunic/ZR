import numpy as np
import matplotlib.pyplot as plt

class Konstante:
    def __init__(self):
        self.G = 6.67408 * 10**(-11)
        self.M_s = 7.35 * 10**22
        self.M_z = 5.9742 * 10**24
        self.v_okomito = 29783
        self.udaljenost_s_z = 384000e3
        self.godina = 50 * 24 * 3600

class Tijelo:
    def __init__(self, p, v, m):
        self.m = m
        self.p = [p]
        self.a = []
        self.v = [v]

class Gibanje:
    def __init__(self, tijelo1, tijelo2, vrijeme_gibanja, dt):
        self.konstante = Konstante()
        self.tijelo1 = tijelo1
        self.tijelo2 = tijelo2
        self.vrijeme_gibanja = vrijeme_gibanja
        self.dt = dt
        self.t = [0]

    def __move(self):
        while self.t[-1] < self.vrijeme_gibanja:
            self.tijelo1.a.append(-self.konstante.G * self.tijelo2.m * (self.tijelo1.p[-1] - self.tijelo2.p[-1]) / (np.linalg.norm(self.tijelo1.p[-1] - self.tijelo2.p[-1]))**3)
            print(self.tijelo1.a[-1])
            self.tijelo1.v.append(self.tijelo1.v[-1] + self.tijelo1.a[-1] * self.dt)
            self.tijelo1.p.append(self.tijelo1.p[-1] + self.tijelo1.v[-1] * self.dt)
            
            self.tijelo2.a.append(-self.konstante.G * self.tijelo1.m * (self.tijelo2.p[-1] - self.tijelo1.p[-1]) / (np.linalg.norm(self.tijelo2.p[-1] - self.tijelo1.p[-1]))**3)
            print(self.tijelo2.a[-1])
            self.tijelo2.v.append(self.tijelo2.v[-1] + self.tijelo2.a[-1] * self.dt)
            self.tijelo2.p.append(self.tijelo2.p[-1] + self.tijelo2.v[-1] * self.dt)

            self.t.append(self.t[-1] + self.dt)

    def plot(self):
        self.__move()

        x_1 = []
        y_1 = []
        x_2 = []
        y_2 = []

        for x, y, in zip(self.tijelo1.p, self.tijelo2.p):
            x_1.append(x[0])
            y_1.append(x[1])
            x_2.append(y[0])
            y_2.append(y[1])

        fix, axs = plt.subplots()
        axs.plot(x_1, y_1)
        axs.plot(x_2, y_2)
        axs.set_aspect("equal", "box")

        plt.show()
