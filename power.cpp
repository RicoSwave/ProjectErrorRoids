#include "power.h"
#include <cmath>

#define PI M_PI

using namespace std;

power::power() {

	int x = 0;
	int y = 0;
	int type = 1;
	int count = 0;

}

// Function to change properties
void power::set(int nx, int ny, int ntype) {

	int x = nx;
	int y = ny;
	int type = ntype;

}

// Get functions
int power::getx() {

	return x;

}

int power::gety() {

	return y;

}

int power::getType() {

	return type;

}

// Check if ship has collided with powerup
int power::check_collision(double nx, double ny) {

	if (!type) {
		return 0;
	}

	if (sqrt(pow(x - nx, 2) + pow(y - ny, 2)) <= 30) {
		return 1;
	}

	return 0;

}

// Draw function
void power::draw(SDL_Surface* screen, Uint32 color) {

	if (type) {

		int size = 10;

		double ax = x + size*cos(0);
		double ay = y + size*sin(0);
		double bx = x + size*cos(PI / 3);
		double by = y + size*sin(PI / 3);
		double cx = x + size*cos(2 * PI / 3);
		double cy = y + size*sin(2 * PI / 3);
		double dx = x + size*cos(PI);
		double dy = y + size*sin(PI);
		double ex = x + size*cos(4 * PI / 3);
		double ey = y + size*sin(4 * PI / 3);
		double fx = x + size*cos(5 * PI / 3);
		double fy = y + size*sin(5 * PI / 3);

		Draw_Line(screen, ax, ay, bx, by, color);
		Draw_Line(screen, bx, by, cx, cy, color);
		Draw_Line(screen, cx, cy, dx, dy, color);
		Draw_Line(screen, dx, dy, ex, ey, color);
		Draw_Line(screen, ex, ey, fx, fy, color);
		Draw_Line(screen, fx, fy, ax, ay, color);

		size = 14;

		ax = x + size*cos(0);
		ay = y + size*sin(0);
		bx = x + size*cos(PI / 3);
		by = y + size*sin(PI / 3);
		cx = x + size*cos(2 * PI / 3);
		cy = y + size*sin(2 * PI / 3);
		dx = x + size*cos(PI);
		dy = y + size*sin(PI);
		ex = x + size*cos(4 * PI / 3);
		ey = y + size*sin(4 * PI / 3);
		fx = x + size*cos(5 * PI / 3);
		fy = y + size*sin(5 * PI / 3);

		// Adds rotation effect
		if (count <= 15) {

			Draw_Line(screen, ax, ay, bx, by, color);
			Draw_Line(screen, cx, cy, dx, dy, color);
			Draw_Line(screen, ex, ey, fx, fy, color);

		}
		else {

			Draw_Line(screen, fx, fy, ax, ay, color);
			Draw_Line(screen, dx, dy, ex, ey, color);
			Draw_Line(screen, bx, by, cx, cy, color);

			if (count >= 30) {
				count = 0;
			}

		}

		count++;
	}
}