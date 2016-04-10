#include "projectile.h"
#include <cmath>

using namespace std;

projectile::projectile() {

	x = 0;
	y = 0;
	theta = 0;
	power = 10.0;

}

double projectile::getx() {

	return x;

}

double projectile::gety() {

	return y;

}

// Used when firing to get ship position
void projectile::set(double nx, double ny, double ntheta, double nvx, double nvy) {

	x = nx;
	y = ny;
	theta = ntheta;
	vx = nvx;
	vy = nvy;

}

// Updates position using built-in velocity variables
void projectile::move() {

	x += power*cos(theta) + vx;
	y += power*sin(theta) + vy;

}

// Draw projectile to screen
void projectile::draw(SDL_Surface* screen, Uint32 color) {

	Draw_FillRect(screen, x - 1, y - 1, 3, 3, color);

}