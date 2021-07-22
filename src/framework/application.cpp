#include "application.h"
#include "utils.h"
#include "image.h"
#include <time.h>
#include <unistd.h>
#include "simulation_objects/simulation_includes.h"
#include "Python.h" // For parallel work with Python.

bool isDead(Creature* toCheck) { return !toCheck->isAlive; }

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);

	// Setting application icon.
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (Icon_64.bytes_per_pixel == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
    #else // Little endian, like x86.
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (Icon_64.bytes_per_pixel == 3) ? 0 : 0xff000000;
    #endif

    SDL_Surface* appIcon = SDL_CreateRGBSurfaceFrom((void*)Icon_64.pixel_data, Icon_64.width, Icon_64.height, Icon_64.bytes_per_pixel*8, Icon_64.bytes_per_pixel*Icon_64.width, rmask, gmask, bmask, amask);
    SDL_SetWindowIcon(window, appIcon);
    SDL_FreeSurface(appIcon);

	framebuffer.resize(w, h);
}

// Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
    srand(time(NULL)); // Since we are doing random stuff, we create a new Random Seed every execution.
    this->simulationMap = Map(framebuffer.width, framebuffer.height); // Creating the map object.
    this->simulationMap.initialize(); // Setting the map to empty.
    this->creatureList.clear();
    this->lastCreatureID = 0;
    this->GenerateCreatures();
    this->startDay();
    this->energyFile.open(ENERGY_FILE_PATH, std::ios::out | std::ios::trunc | std::ios::ate);
    this->speedFile.open(SPEED_FILE_PATH, std::ios::out | std::ios::trunc | std::ios::ate);
    this->populationFile.open(POPULATION_FILE_PATH, std::ios::out | std::ios::trunc | std::ios::ate);
    this->energyFile << "0" << ","; this->speedFile << "0" << ","; this->populationFile << "0" << ",";
}

// Render one frame
void Application::render(Image& frameBuffer)
{
    if (this->isRenderActive)
    {
        frameBuffer.fill(GROUND_COLOR);
        for (unsigned int x=0; x<this->framebuffer.width; x++)
        {
            for (unsigned int y=0; y<this->framebuffer.height; y++)
            {
                CellInformation* currentCell = this->simulationMap.getCell(x, y);
                if (!currentCell->isEmpty)
                {
                    if (currentCell->thereIsFood())
                    {
                        Food* currentFood = currentCell->foodOnIt;
                        if (currentFood->isVisible) frameBuffer.drawCircle(currentFood->x, currentFood->y, FOOD_SCREEN_SIZE, FOOD_COLOR);
                    }
                    if (currentCell->thereIsCreature())
                    {
                        Creature* currentCreature = currentCell->creatureOnIt;
                        if (currentCreature->isVisible)
                        {
                            frameBuffer.drawRectangle(currentCreature->x, currentCreature->y, CREATURE_SCREEN_SIZE, CREATURE_SCREEN_SIZE, CREATURE_COLOR, true);
                            if (this->viewHitBoxes)
                            {
                                Color boxColor = (currentCreature->isGoingHome) ? Color::YELLOW : Color::WHITE;
                                frameBuffer.drawRectangle(currentCreature->HitBox.x, currentCreature->HitBox.y, currentCreature->HitBox.width, currentCreature->HitBox.height, boxColor, false);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Called after render
void Application::update(double seconds_elapsed)
{
    if (!this->isPaused)
    {
        if (this->isDayTime) // If simulation is not paused and we are in day time, we can update the creatures.
        {
            for (int t=0; t<this->MainSpeed; t++) this->UpdateCreatures();
        }
        this->elapsedDayTime += seconds_elapsed*this->MainSpeed;
        // printf("Elapsed Time: %f\n", this->elapsedDayTime);
        if (this->elapsedDayTime >= DAY_DURATION)
        {
            this->startNight();
            this->startDay();
        }
    }

	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE])
	{

	}
}

// Keyboard press event
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch(event.keysym.scancode)
	{
	    case SDL_SCANCODE_UP:
	        if (this->MainSpeed < MAX_TIME_SPEED) this->MainSpeed++;
	        break;
	    case SDL_SCANCODE_DOWN:
	        if (this->MainSpeed > MIN_TIME_SPEED) this->MainSpeed--;
	        break;
	    case SDL_SCANCODE_KP_PLUS:
	        this->MainSpeed = MAX_TIME_SPEED;
	        break;
	    case SDL_SCANCODE_KP_MINUS:
	        this->MainSpeed = MIN_TIME_SPEED;
	        break;
	    case SDL_SCANCODE_R:
	        this->init();
	        break;
	    case SDL_SCANCODE_H:
	        this->viewHitBoxes = !this->viewHitBoxes;
	        break;
	    case SDL_SCANCODE_TAB: // TAB Key stops the updating methods.
	        this->isPaused = !this->isPaused;
	        break;
		case SDL_SCANCODE_ESCAPE: // ESC Key exits the application.
		    this->simulationMap.saveMap(MAP_FILE_PATH);
		    printf("Generating & Saving Plots...\n");
            this->energyFile.close();
            this->speedFile.close();
            this->populationFile.close();
            this->generatePlot();
			exit(0); 
			break;
	}
}

void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

void Application::GenerateFood()
{
    /* We are going to generate the food position following a Gaussian Distribution.
     * Since we are in a 2D map, we're using Multivariate Normal Distribution: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
     * N(μ, Σ), where μ is the mean, the center of the screen, and Σ is the variance, generated by screen dimensions and FOOD_EXPANSION value.
     * C++ writes in a .txt the Mean, Sigma and number of random data points from the 2D Gaussian, then, a Python script using NumPy calculates
     * all the data points and write it to a .txt file. Finally, C++ reads the data points and use it.
    */

    std::ofstream inputData(INPUT_MULTIVARIATE_NORMAL, std::ofstream::out); // This file is going to contain Mu, Sigma and Size.
    std::string Mu[2] = {std::to_string((int)this->window_width/2), std::to_string((int)this->window_height/2)}; // Mean as string.
    std::string Sigma[4] = {"0", std::to_string((int)this->window_height * FOOD_EXPANSION), std::to_string((int)this->window_width * FOOD_EXPANSION), "0"}; // Covariance as string.
    std::string Size = std::to_string(N_FOODS_UNITS); // Number of random data points from the 2D Gaussian.

    // Writing data to file.
    inputData << Mu[0] << ","; inputData << Mu[1] << "\n";
    inputData << Sigma[0] << ","; inputData << Sigma[1] << ","; inputData << Sigma[2] << ","; inputData << Sigma[3] << "\n";
    inputData << Size;
    inputData.close();

    Py_Initialize();
    FILE* pythonScript = fopen(RANDOM_MULTIVARIATE_NORMAL_SCRIPT, "r"); // Opening Python script.
    PyRun_AnyFile(pythonScript, RANDOM_MULTIVARIATE_NORMAL_SCRIPT); // Running it with Python3.
    fclose(pythonScript);

    std::fstream outputData; // File data containing random 2D Gaussian data.
    outputData.open(OUTPUT_MULTIVARIATE_NORMAL, std::ios::in); // File containing data generated by Python.
    std::string line, part1, part2;
    int index = 0;
    while(getline(outputData, line)) // Reading each file line.
    {
        int separation = line.find(","); // Since each line is a 2D Gaussian dot splitted by a comma.
        for (int i=0; i<separation; i++) part1.append(1, line.at(i));
        for (int i=separation+1; i<line.size(); i++) part2.append(1, line.at(i));
        int x = std::stoi(part1); int y = std::stoi(part2);
        if (x > 0 && y > 0 && x < this->window_width && y < this->window_height)
        {
            this->foodList[index] = Food(index, x, y, rand()%10);
            this->simulationMap.setFoodTo(x, y, &this->foodList[index]);
            index++;
        }
        part1.clear(); part2.clear();
    }
    outputData.close();

    this->CurrentFoodUnits = index; // Updating the number of food units, because some of them have been removed.
}

void Application::GenerateCreatures()
{
    for (int i=0; i<N_CREATURES; i++)
    {
        bool toHorizontal, toDown;
        (rand()%2 == 1) ? toHorizontal = true : toHorizontal = false;
        (rand()%2 == 1) ? toDown = true : toDown = false;
        int speed = (rand()%(MAX_SPEED-1))+1; int energy = (rand()%(MAX_ENERGY-1))+1;
        float probability = (float)rand() / (float)RAND_MAX;
        Creature* newCreature;
        if (toHorizontal && toDown) // Down
        {
            newCreature = new Creature(i, rand()%this->simulationMap.w, this->simulationMap.h-1-CREATURE_SCREEN_SIZE, speed, probability, energy, DOWN, (rand()%MAX_CHANGE_DIRECTION_COEF)+1);
            this->creatureList.push_back(newCreature);
        }
        else if (toHorizontal && !toDown) // Up
        {
            newCreature = new Creature(i, rand()%this->simulationMap.w, 0, speed, probability, energy, UP, (rand()%MAX_CHANGE_DIRECTION_COEF)+1);
            this->creatureList.push_back(newCreature);
        }
        else if (!toHorizontal && toDown) // Left Side
        {
            newCreature = new Creature(i, this->simulationMap.w-1-CREATURE_SCREEN_SIZE, rand()%this->simulationMap.h, speed, probability, energy, LEFT, (rand()%MAX_CHANGE_DIRECTION_COEF)+1);
            this->creatureList.push_back(newCreature);
        }
        else if (!toHorizontal && !toDown) // Right Side
        {
            newCreature = new Creature(i, 0, rand()%this->simulationMap.h, speed, probability, energy, RIGHT, (rand()%MAX_CHANGE_DIRECTION_COEF)+1);
            this->creatureList.push_back(newCreature);
        }
        int x = newCreature->x; int y = newCreature->y;
        this->simulationMap.setCreatureTo(x, y, newCreature);
        lastCreatureID++;
    }
}

void Application::UpdateCreature(Creature *toUpdate)
{
    if (!toUpdate->isGoingHome)
    {
        int old_x = toUpdate->x; int old_y = toUpdate->y;
        toUpdate->getNextDirection(this->window_width, this->window_height);
        toUpdate->move();
        int new_x = toUpdate->x; int new_y = toUpdate->y;
        Food* touchingFood = toUpdate->isTouchingAnyFood(this->foodList, this->CurrentFoodUnits);
        if (touchingFood != NULL && !touchingFood->hasBeenEaten && toUpdate->eatenFoods < 2) // If creature is on a food unit and can eat more food.
        {
            toUpdate->eat(*touchingFood);
            if (toUpdate->eatenFoods == 1)
            {
                // Decide if eats second unit or goes home.
                srand(time(NULL)*toUpdate->ID);
                float k = (float)rand() / (float)RAND_MAX; // Random decimal value between 0 and 1.
                if (k <= toUpdate->secondUnitProb)
                {
                    // Search for second unit.
                }
                else toUpdate->goHome(this->simulationMap.w, this->simulationMap.h);
            }
            else if (toUpdate->eatenFoods == 2)
            {
                toUpdate->goHome(this->simulationMap.w, this->simulationMap.h);
            }
            touchingFood->hasBeenEaten = true;
            touchingFood->isVisible = false;
            this->simulationMap.removeFood(new_x, new_y); // Deleting the eaten food from the map.
        }

        this->simulationMap.removeCreature(old_x, old_y);
        this->simulationMap.setCreatureTo(new_x, new_y, toUpdate);
    }
    else if (!toUpdate->isAtHome)
    {
        if (toUpdate->canContinue(this->simulationMap.w, this->simulationMap.h))
        {
            int old_x = toUpdate->x; int old_y = toUpdate->y;
            toUpdate->move();
            int new_x = toUpdate->x; int new_y = toUpdate->y;
            this->simulationMap.removeCreature(old_x, old_y);
            if (this->simulationMap.hasInside(*toUpdate)) this->simulationMap.setCreatureTo(new_x, new_y, toUpdate);
        }
        else
        {
            if (toUpdate->x != toUpdate->homeLocation[0] || toUpdate->y != toUpdate->homeLocation[1])
            {
                toUpdate->x = toUpdate->homeLocation[0]; toUpdate->y = toUpdate->homeLocation[1];
                toUpdate->isAtHome = true;
            }
            else
            {
                toUpdate->isAtHome = true;
            }
        }
    }
    toUpdate->updateHitBox(); // After everything, we update the HitBox.
}

void Application::UpdateCreatures()
{
    float energyMean = -1.0, speedMean = -1.0;
    for (Creature* currentCreature : this->creatureList)
    {
        if (currentCreature->isAlive)
        {
            this->UpdateCreature(currentCreature);
            // Energy Mean
            if (energyMean == -1.0) energyMean = currentCreature->energy;
            else energyMean = (energyMean + currentCreature->energy) / 2.0;
            // Speed Mean
            if (speedMean == -1.0) speedMean = currentCreature->speed;
            else speedMean = (speedMean + currentCreature->speed) / 2.0;
        }
    }
    this->creatureList.remove_if(isDead);
    this->CurrentCreatures = this->creatureList.size();
}

void Application::startDay()
{
    this->isDayTime = true;
    this->simulationMap.clearFoodFromCells();
    this->GenerateFood();
    this->EnergyMean = -1.0; this->SpeedMean = -1.0;
    for (Creature* currentCreature : this->creatureList)
    {
        if (currentCreature->isAlive)
        {
            currentCreature->isAtHome = true;
            currentCreature->isGoingHome = false;
            currentCreature->eatenFoods = 0;
            currentCreature->homeLocation[0] = -1;
            currentCreature->homeLocation[1] = -1;

            if (this->EnergyMean == -1) this->EnergyMean = currentCreature->energy;
            else this->EnergyMean = (this->EnergyMean + currentCreature->energy) / 2.0;

            if (this->SpeedMean == -1) this->SpeedMean = currentCreature->speed;
            else this->SpeedMean = (this->SpeedMean +  currentCreature->speed) / 2.0;
        }
    }
    this->energyFile << this->EnergyMean << ","; this->speedFile << this->SpeedMean << ","; this->populationFile << this->CurrentCreatures << ",";
    printf("Energy Mean: %f | Speed Mean: %f | Creatures Number: %d \n", this->EnergyMean, this->SpeedMean, this->CurrentCreatures);
    this->CurrentGeneration++;
    this->elapsedDayTime = 0.0;
    // Send creatures to eat.
}

void Application::startNight()
{
    this->isDayTime = false;
    std::list<Creature*> newChilds;
    for (Creature* currentCreature : this->creatureList) // All the creatures that are not in home, are dead when night comes.
    {
        if (!currentCreature->isAtHome)
        {
            currentCreature->kill();
            this->simulationMap.removeCreature(currentCreature->x, currentCreature->y);
        }
        else if (currentCreature->eatenFoods == 2)
        {
            this->lastCreatureID++;
            Creature* newChild = currentCreature->reproduce(this->lastCreatureID);
            newChilds.push_back(newChild);
            this->simulationMap.setCreatureTo(newChild->x, newChild->y, newChild);
        }
    }
    this->creatureList.merge(newChilds);
    this->CurrentCreatures = this->creatureList.size();
}

void Application::generatePlot()
{
    Py_Initialize();
    FILE* pythonScript = fopen(PLOT_SCRIPT, "r"); // Opening Python script.
    PyRun_AnyFile(pythonScript, PLOT_SCRIPT); // Running it with Python3.
    fclose(pythonScript);
}

// When the app starts
void Application::start()
{
	launchLoop(this);
}
