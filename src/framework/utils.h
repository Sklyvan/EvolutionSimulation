/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
#ifndef UTILS_H
#define UTILS_H

#include "includes.h"
#include "framework.h"

//General functions **************
class Application;
class Image;

//check opengl errors
bool checkGLErrors();

SDL_Window* createWindow(const char* caption, int width, int height);
void launchLoop(Application* app);

void sendFramebufferToScreen(Image* img);

//fast random generator
inline unsigned long frand(void) {          //period 2^96-1
	unsigned long t;
	static unsigned long x = 123456789, y = 362436069, z = 521288629;
	x ^= x << 16; x ^= x >> 5; x ^= x << 1;
	t = x; x = y;y = z;
	z = t ^ x ^ y;
	return z;
}

inline float randomValue() { return (frand() % 10000) / 10000.0; }
std::string getBinPath();

#endif
