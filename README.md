# Simulating The Evolution

This is a C++ program that simulates the [Natural Selection](https://en.wikipedia.org/wiki/Natural_selection) phenomenon, described by [Charles Darwin](https://en.wikipedia.org/wiki/Charles_Darwin) in "[The Origin of the Species](https://en.wikipedia.org/wiki/On_the_Origin_of_Species)".

The simulation occurs inside a 2D world, the time inside the world is formed by days, and every day divided in two parts: **Day** & Night.
When the day begins, all the creatures start moving and searching for food untill they find a food unit. When the creature finds a food unit decides (Depending on a random atribute assigned to every creature) if it goes to search another one or goes back home. After finding a second food unit, it always goes back home. If a creature is moving and the energy of if goes to 0, the creature dies.

When the night begins, all the creatures that are outside the house die. And all the creatures at home who have eaten two food units, reproduce.

### The Creature

Every creature inheritances from the ScreenObject class. And contains some attributes:

- (Float) **Speed**: Number of pixels that moves at each update.

- (Float) **Second Unit Probability**: Probability to search for a second food unit.

- (Float) **Energy**: Initial energy. energy(t) = ENERGY_DECREASE_COEF · t · Speed.

- (Float) **Current Energy**: Energy in a moment t.

- (Integer) **Eaten Foods**: Number of eaten food units after going out. (0, 1, 2)

- (Creature) **Created By**: Creature father, can be NULL at first generation.

- (Creature List) **Childs**: Creature childs.

- (Integer) **Direction**: UP, DOWN, LEFT or RIGHT.

- (Integer) **Hold Direction Counter**: To avoid changing too much the direction.

- (Integer) **Change Direction Coefficient**: To change the direction randomly.

- (Boolean) **isAlive**

- (Boolean) **isAtHome**

- (Boolean) **isGoingHome**

- (int[2]) **Home Location**: X and Y home location.

### Constants:

When running the simulation there's some constants which you can modify to view alternative simulations and results. 

You can find this file at: **src/simulation_objects/simulation_constants.h**

- GROUND_COLOR: RGB Color, defines the background color.

- FOOD_COLOR: RGB Color, defines the food color.

- CREATURE_COLOR: RGB Color, defines the creature color.

- N_FOODS_UNITS: Number of food units generated every time.

- FOOD_EXPANSION: Separation between foods.

- DAY_DURATION: Duration of the days (In seconds).

- MAX_CHANGE_DIRECTION_COEF: Max value for Change Direction Coefficient.

- N_CREATURES: Initial number of creatures.

- MAX_ENERGY: Max value for Energy.

- MAX_SPEED: Max value for Speed.

- ENERGY_DECREASE_COEF: Value used on the energy(t).

- HOLD_DIRECTION: How many iterations the direction isn't changed.

### Food Generation:

The food is generated following a Gaussian Distribution. Since we are in a 2D map, we're using [Multivariate Normal Distribution]([Multivariate normal distribution - Wikipedia](https://en.wikipedia.org/wiki/Multivariate_normal_distribution)), N(μ, Σ), where μ is the mean, the center of the screen, and Σ is the variance, generated by screen dimensions and FOOD_EXPANSION value. C++ writes in a .txt the Mean, Sigma and number of random data points from the 2D Gaussian, then, a Python script using NumPy calculates all the data points and write it to a .txt file. Finally, C++ reads the data points and use it.

### Instalation:

The whole program has been compiled on Linux, so there's no guarantee about working on Windows or MacOS.

```shell
sudo apt upgrade
sudo apt-get install cmake make gcc
sudo apt-get install -y libsdl2-dev libglew-dev freeglut3-dev
```

```shell
sudo apt-get install python3-dev
sudo apt install mlocate # Tool to locate files on the computer.
locate Python.h
```

After locate Python.h, you should get a location similar to:

**/usr/include/python3.8/Python.h** The Python version can be 3.7, 3.9 or 3.10.

Now you can link the SDL and Python, for the Python link use the location found on **locate Python.h** output.

```shell
sudo ln -s /usr/include/SDL2/SDL_ttf.h /usr/local/include/SDL2/
sudo ln -sv /usr/include/python3.8/* /usr/include/
```

### Using the Application Features:

When running the application, the simulation starts automatically. You can interact with those keys:

- Arrow Up: Increase the simulation speed.

- Arrow Down: Decrease the simulation speed.

- Plus Key: Set the simulation speed to the maximum.

- Minus Key: Set the simulation speed to the minimum.

- R Key: Restarts the simulation.

- H Key: Show the creatures hitboxes (For development).

- TAB Key: Pauses the simulation.

- ESC Key: Exits the application and **generates plots**.

### Plotting Results:

This feature is still on development, and more plots are going to be implemented. You can find the plots and the data at src/python_plotting. Those plots are generated **only** when the application is terminated with the ESC key.

Actually, you can see two diferent plots:

- Population Graph: Shows the number of creatures for every day.

- Speed & Energy: Shows the energy mean and speed for all creatures every day.

<img src="https://github.com/Sklyvan/SimulationWorld/blob/main/res/TwoPlots.png" title="" alt="TwoPlots.png" data-align="center">
