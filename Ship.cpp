#include "ship.h"
#include <cmath>
#include <cstdlib>

#define PI M_PI

using namespace std;

ship::ship() {

	// Position
	x = 200;
	y = 200;
	// Velocity
	vx = 0;
	vy = 0;
	// Rotation
	theta = -PI / 2;
	omega = 0;
	// 'Strength' of ship thrust and rotation
	thrust = 0.5;
	alpha = 0.01;
	// Collision radius and draw size
	size = 30.0;
	// Limit speed and rotation
	vmax = 20;
	omax = 0.2;
	// Current powerup status
	power = 0;

}

// Get functions
double ship::getx() {

	return x;

}

double ship::gety() {

	return y;

}

double ship::gettheta() {

	return theta;

}

double ship::getNoseX() {

	return x + size*cos(theta);

}

double ship::getNoseY() {

	return y + size*sin(theta);

}

double ship::getvx() {

	return vx;

}

double ship::getvy() {

	return vy;

}

double ship::getsize() {

	return size;

}

int ship::getpower() {

	return power;

}

// Set functions
void ship::setx(double nx) {

	x = nx;

}

void ship::sety(double ny) {

	y = ny;

}

void ship::setpower(int npower) {

	power = npower;

}

// Accelerates forward when 'up' or 'w' pressed
void ship::accelerate_linear(int thr) {

	if (thr) {
		if (sqrt(vx*vx + vy*vy) < vmax) {
			vx += thrust*cos(theta);
			vy += thrust*sin(theta);
		}
		else {
			vx = vx*0.95;
			vy = vy*0.95;
		}
	}

}

// Angular acceleration for <-/-> or a/d
void ship::accelerate_angular(double d) {

	if (abs(omega) < omax) {
		omega += d*alpha;
	}
	else {
		omega = omega*0.95;
	}

}

// Automatically update position after time elapsed
void ship::move() {

	x += vx;
	y += vy;
	theta += omega;

}

// Teleport to a random screen location when 'down' or 's' pressed
void ship::teleport(int width, int height) {

	x = rand() % (width - 100) + 50;
	y = rand() % (height - 100) + 50;

}

// Draw to screen
void ship::draw(SDL_Surface* screen, Uint32 color) {

	double ax, ay, bx, by, cx, cy, dx, dy;

	ax = x + size*cos(theta);
	ay = y + size*sin(theta);

	bx = x + size*cos(theta + 13 * PI / 16);
	by = y + size*sin(theta + 13 * PI / 16);

	cx = x + (size - size / 3)*cos(theta + PI);
	cy = y + (size - size / 3)*sin(theta + PI);

	dx = x + size*cos(theta + 19 * PI / 16);
	dy = y + size*sin(theta + 19 * PI / 16);

	Draw_Line(screen, ax, ay, bx, by, color);
	Draw_Line(screen, bx, by, cx, cy, color);
	Draw_Line(screen, cx, cy, dx, dy, color);
	Draw_Line(screen, dx, dy, ax, ay, color);

}

void ship::check_borders(int width, int height) {
	if (x <= size) {
		x = width - size - 1;
	}
	else if (x >= width - size) {
		x = 1 + size;
	}
	if (y <= size) {
		y = height - size - 1;
	}
	else if (y >= height - size) {
		y = 1 + size;
	}
}