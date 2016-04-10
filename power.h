#ifndef POWER_H
#define POWER_H

#include "SDL.h"
#include "SDL_draw.h"

using namespace std;

class power {

public:
	power();
	int getx();
	int gety();
	int getType();
	void set(int, int, int);
	int check_collision(double, double);
	void draw(SDL_Surface*, Uint32);

private:
	int x, y;
	int type;
	int count;

};

#endif
