#ifndef SHIP_H
#define SHIP_H

#include <iostream>

#include "SDL.h"
#include "SDL_draw.h"

using namespace std;

class ship {

public:
	ship();
	double getx();
	double gety();
	double gettheta();
	double getNoseX();
	double getNoseY();
	double getvx();
	double getvy();
	double getsize();
	int getpower();
	void setx(double);
	void sety(double);
	void setpower(int);
	void accelerate_linear(int);
	void accelerate_angular(double);
	void move();
	void teleport(int, int);
	void draw(SDL_Surface*, Uint32);
	void check_borders(int, int);


private:
	double x, y;
	double vx, vy;
	double theta;
	double omega;
	double thrust;
	double alpha;
	double size;
	double vmax;
	double omax;
	int power;

};

#endif