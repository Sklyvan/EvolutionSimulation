/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <fstream>
#include "includes.h"
#include "framework.h"
#include "image.h"
#include "simulation_objects/simulation_includes.h"

class Application
{
public:
	// Window
	SDL_Window* window;
	float window_width;
	float window_height;

	Image framebuffer;

	Map simulationMap;

	int MainSpeed = 1;
	bool viewHitBoxes = false;
    // SDL_Surface* MainSpeedText;
    // TTF_Font* speedFont = TTF_OpenFont(SPEED_FONT_PATH, SPEED_FONT_SIZE);

    Food foodList[N_FOODS_UNITS];
    std::list<Creature*> creatureList;

    int CurrentCreatures = N_CREATURES;
    int CurrentFoodUnits = N_FOODS_UNITS;
    int CurrentGeneration = 0;
    int lastCreatureID;

    std::fstream energyFile;
    std::fstream speedFile;
    std::fstream populationFile;

    float EnergyMean;
    float SpeedMean;

    double elapsedDayTime;

    bool isPaused = false;
    bool isRenderActive = true;
    bool isDayTime = true;

	float application_time;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	//constructor
	Application(const char* caption, int width, int height);

	//main methods
	void init( void );
	void render( Image& framebuffer );
	void update( double dt );

	//methods for events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp( SDL_MouseButtonEvent event );

	//other methods to control the app
	void setWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		framebuffer.resize(width,height);
	}

	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
	void GenerateFood();
	void GenerateCreatures();
	void UpdateCreature(Creature* toUpdate);
	void UpdateCreatures();
	void startDay();
	void startNight();
	void generatePlot();
};

#endif
