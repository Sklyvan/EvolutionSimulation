/*  By Javi Agenjo 2013 UPF  javi.agenjo@gmail.com

	MAIN:
	 + This file creates the window and the app instance. 
	 + It also contains the mainloop
	 + This is the lowest level, here we access the system to create the opengl Context
	 + It takes all the events from SDL and redirect them to the game
*/

#include "includes.h"
#include "application.h"

#define WIDTH 1080
#define HEIGHT 1080

int main(int argc, char **argv)
{
	// Launch the application.
	Application* MyApp = new Application("Simulation World", WIDTH, HEIGHT);
    MyApp->init();
    MyApp->start();

	return 0;
}
