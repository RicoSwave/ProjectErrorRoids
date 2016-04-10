#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "SDL.h"
#include "SDL_draw.h"

using namespace std;

class projectile {

public:
	projectile();
	double getx();
	double gety();
	void set(double, double, double, double, double);
	void move();
	void draw(SDL_Surface*, Uint32);

private:
	double x, y;
	double vx, vy;
	double theta;
	double power;

};

#endif