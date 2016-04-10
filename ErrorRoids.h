#pragma once

#ifndef ASTEROID_H
#define ASTEROID_H

using namespace std;

#include "SDL.h"
#include "SDL_draw.h"

class ErrorRoids {		//the asteroids

public:
	ErrorRoids();
	int getSize();
	double getx();
	double gety();
	double getvx();
	double getvy();
	Uint32 getColor();
	void setx(double);
	void sety(double);
	void setvx(double);
	void setvy(double);
	void move();
	void set(int, double, double, double, double, Uint32);
	void draw(SDL_Surface*);
	int check_collision(double, double);

private:
	int size;
	double x, y;
	double vx, vy;
	double theta;
	double omega;
	Uint32 color;
};

#endif