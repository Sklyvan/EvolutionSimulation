import matplotlib.pyplot as plt
import numpy as np

EnergyFile = "../src/python_plotting/energy.txt"
SpeedFile = "../src/python_plotting/speed.txt"
PopulationFile = "../src/python_plotting/population.txt"
outputImage1, outputImage2 = "../src/python_plotting/Energy_Speed.png", "../src/python_plotting/Population.png"

# EnergyFile = "energy.txt"
# SpeedFile = "speed.txt"
# PopulationFile = "population.txt"
# outputImage1, outputImage2 = "Energy_Speed.png", "Population.png"
try:
    with open(EnergyFile, 'r') as file:
        energy = file.readlines()[0].replace('\n', '')[:-1].split(',')
        energy = np.array(list(map(float, energy)))

    with open(SpeedFile, 'r') as file:
        speed = file.readlines()[0].replace('\n', '')[:-1].split(',')
        speed = np.array(list(map(float, speed)))

    with open(PopulationFile, 'r') as file:
        population = file.readlines()[0].replace('\n', '')[:-1].split(',')
        population = np.array(list(map(int, population)))
except IndexError:
    print("Not enough data to generate Plots.")

else:
    x = list(range(len(population)))

    plt.plot(x, speed, label="Speed Mean")
    plt.plot(x, energy, label="Energy Mean")
    plt.legend()
    plt.savefig(outputImage1)
    plt.clf()

    plt.plot(x, population, label="Current Population")
    plt.legend()
    plt.savefig(outputImage2)
