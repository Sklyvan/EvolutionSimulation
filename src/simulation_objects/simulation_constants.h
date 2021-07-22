#ifndef SIMULATIONWORLD_SIMULATION_CONSTANTS_H
#define SIMULATIONWORLD_SIMULATION_CONSTANTS_H

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define SPEED_FONT_PATH "../res/fonts/Absender.ttf"
#define SPEED_FONT_SIZE 12

#define RANDOM_NORMAL_SCRIPT "../src/data_generation/RandomNormal.py"
#define RANDOM_MULTIVARIATE_NORMAL_SCRIPT "../src/data_generation/RandomMultivariateNormal.py"

#define INPUT_NORMAL "../src/data_generation/RandomNormal_InputData.txt"
#define OUTPUT_NORMAL "../src/data_generation/RandomNormal_GaussianData.txt"

#define INPUT_MULTIVARIATE_NORMAL "../src/data_generation/RandomMultivariate_InputData.txt"
#define OUTPUT_MULTIVARIATE_NORMAL "../src/data_generation/RandomMultivariate_GaussianData.txt"

#define MAP_FILE_PATH "../res/map_export/SimulationMap.bin"

#define PLOT_SCRIPT "../src/python_plotting/Energy_Speed_Population.py"
#define ENERGY_FILE_PATH "../src/python_plotting/energy.txt"
#define POPULATION_FILE_PATH "../src/python_plotting/population.txt"
#define SPEED_FILE_PATH "../src/python_plotting/speed.txt"

#define GROUND_COLOR Color(33, 33, 33)
#define FOOD_COLOR Color(130, 255, 165)
#define CREATURE_COLOR Color(232, 85, 85)

#define N_FOODS_UNITS 200
#define FOOD_EXPANSION 25

#define MIN_TIME_SPEED 1
#define MAX_TIME_SPEED 100
#define DAY_DURATION 10.0

#define MAX_CHANGE_DIRECTION_COEF 5

#define N_CREATURES 400
#define MAX_ENERGY 10
#define MAX_SPEED 10
#define ENERGY_DECREASE_COEF 0.001
#define HOLD_DIRECTION 8

#define FOOD_SCREEN_SIZE 2
#define CREATURE_SCREEN_SIZE 5

#endif //SIMULATIONWORLD_SIMULATION_CONSTANTS_H
