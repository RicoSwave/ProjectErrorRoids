#include <iostream>
#include "ship.h"
#include "ErrorRoids.h"
#include "projectile.h"
#include "power.h"
#include <cmath>
#include <ctime>

#include "SDL.h"
#include "SDL_draw.h"

#include <deque>

#define PI M_PI

using namespace std;

/*void accelerate_ship(){
if (acc != 0){
s1.accelerate_angular( acc );
}
if (thr){
s1.accelerate_linear();
}

if(pow0.check_collision(s1.getx(),s1.gety())){
s1.setpower(pow0.getType());
pow0.set(pow0.getx(), pow0.gety(), 0);
}
}*/

int main() {

	//Initialize values

	// Player ship
	ship s1;
	// Blank ship projectile
	projectile p0;
	// Blank asteroid
	ErrorRoids a0;
	// Blank powerup
	power pow0;

	// Screen info
	int width = 800;
	int height = 600;
	int fps = 60;

	// Physics properties
	double acc = 0;
	int thr = 0;

	// Internal stuff
	int loop = 1;
	int i, j;
	Uint32 color;
	int score = 0;
	double repRadius = 0;
	int powCooldown = 600;
	int maxProjectiles = 3;

	// Use deques to contain asteroids and projectiles
	deque<projectile> proj;
	deque<ErrorRoids> ast;

	// SDL information
	SDL_Surface* screen = NULL;
	SDL_Event event;

	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

	// Set colors
	Uint32 c_white = SDL_MapRGB(screen->format, 255, 255, 255);
	Uint32 c_black = SDL_MapRGB(screen->format, 0, 0, 0);
	Uint32 c_cyan = SDL_MapRGB(screen->format, 32, 255, 255);
	Uint32 c_red = SDL_MapRGB(screen->format, 255, 32, 32);
	Uint32 c_green = SDL_MapRGB(screen->format, 32, 255, 32);

	srand(time(NULL));

	// Set powerup to null
	pow0.set(0, 0, 0);

	while (loop) {

		// Check input

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = 0;
				break;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					acc = 1;
					break;
				case SDLK_UP:
					thr = 1;
					break;
				case SDLK_LEFT:
					acc = -1;
					break;
				case SDLK_DOWN:
					s1.teleport(width, height);
					break;
				case SDLK_SPACE:
					p0.set(s1.getNoseX(), s1.getNoseY(), s1.gettheta(), s1.getvx(), s1.getvy());
					proj.push_back(p0);
					break;
				case SDLK_ESCAPE:
					loop = 0;
					break;
				}
			}
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					acc = 0;
					break;
				case SDLK_UP:
					thr = 0;
					break;
				case SDLK_LEFT:
					acc = 0;
					break;
				}
			}
		}

		// Accelerate ship

		if (pow0.check_collision(s1.getx(), s1.gety())) {
			s1.setpower(pow0.getType());
			pow0.set(pow0.getx(), pow0.gety(), 0);
		}

		// Check powerup

		if (s1.getpower() == 0 && pow0.getType() == 0) {
			powCooldown--;
			if (powCooldown <= 0) {
				pow0.set(rand() % (width - 100) + 50, rand() % (height - 100) + 50, 1);
				powCooldown = 600;
			}
		}
		else if (s1.getpower() == 1) {

			// Repulsor powerup effect
			for (i = 0; i < ast.size(); i++) {
				if (sqrt(pow(ast[i].getx() - s1.getx(), 2) + pow(ast[i].gety() - s1.gety(), 2)) <= 100) {
					double repAngle = atan((ast[i].gety() - s1.gety()) / (ast[i].getx() - s1.getx()));
					double repPower = (100 - sqrt(pow(ast[i].getx() + s1.getx(), 2) + pow(ast[i].gety() + s1.gety(), 2))) / 5000;
					if (ast[i].getx() > s1.getx()) {
						ast[i].setvx(ast[i].getvx() - repPower*cos(repAngle));
					}
					else {
						ast[i].setvx(ast[i].getvx() + repPower*cos(repAngle));
					}
					if (ast[i].gety() > s1.gety()) {
						ast[i].setvy(ast[i].getvy() - repPower*sin(repAngle));
					}
					else {
						ast[i].setvy(ast[i].getvy() + repPower*sin(repAngle));
					}
				}
			}

			// Repulsor powerup animation
			if (s1.getx() > repRadius && width - s1.getx() > repRadius && s1.gety() > repRadius && height - s1.gety() > repRadius) {
				Draw_Circle(screen, s1.getx(), s1.gety(), repRadius, c_white);
			}
			repRadius += 3;
			if (repRadius > 100) {
				repRadius = 0;
			}
			powCooldown--;
			if (powCooldown <= 0) {
				s1.setpower(0);
				powCooldown = 600;
			}
		}

		// Update ship position

		s1.accelerate_angular(acc);
		s1.accelerate_linear(thr);
		s1.move();
		s1.check_borders(width, height);
		s1.draw(screen, c_white);

		// Update projectile position

		for (i = 0; i < proj.size(); i++) {
			proj[i].move();
			if (proj[i].getx() >= width || proj[i].getx() <= 0) {
				proj.erase(proj.begin() + i);
			}
			else if (proj[i].gety() >= height || proj[i].gety() <= 0) {
				proj.erase(proj.begin() + i);
			}
			else {
				proj[i].draw(screen, c_white);
			}
		}
		if (proj.size() > maxProjectiles) {
			proj.pop_front();
		}

		// Update asteroid position

		for (i = 0; i < ast.size(); i++) {

			ast[i].move();

			for (j = 0; j < proj.size(); j++) {
				if (ast[i].check_collision(proj[j].getx(), proj[j].gety())) {
					if (ast[i].getSize() > 1) {
						a0.set(ast[i].getSize() - 1, ast[i].getx() + rand() % 40 - 20, ast[i].gety() + rand() % 40 - 20, ast[i].getvx() + (rand() % 2) - 1, ast[i].getvy() + (rand() % 2) - 1, ast[i].getColor());
						ast.push_back(a0);
						a0.set(ast[i].getSize() - 1, ast[i].getx() + rand() % 40 - 20, ast[i].gety() + rand() % 40 - 20, ast[i].getvx() + (rand() % 2) - 1, ast[i].getvy() + (rand() % 2) - 1, ast[i].getColor());
						ast.push_back(a0);
					}
					ast.erase(ast.begin() + i);
					proj.erase(proj.begin() + j);
					score++;
				}
			}

			// Check for collisions

			double s_ax = s1.getx() + s1.getsize()*cos(s1.gettheta());
			double s_ay = s1.gety() + s1.getsize()*sin(s1.gettheta());
			double s_bx = s1.getx() + s1.getsize()*cos(s1.gettheta() + 13 * PI / 16);
			double s_by = s1.gety() + s1.getsize()*sin(s1.gettheta() + 13 * PI / 16);
			double s_cx = s1.getx() + (s1.getsize() - s1.getsize() / 3)*cos(s1.gettheta() + PI);
			double s_cy = s1.gety() + (s1.getsize() - s1.getsize() / 3)*sin(s1.gettheta() + PI);
			double s_dx = s1.getx() + s1.getsize()*cos(s1.gettheta() + 19 * PI / 16);
			double s_dy = s1.gety() + s1.getsize()*sin(s1.gettheta() + 19 * PI / 16);

			if (ast[i].check_collision(s_ax, s_ay) || ast[i].check_collision(s_bx, s_by) ||
				ast[i].check_collision(s_cx, s_cy) || ast[i].check_collision(s_dx, s_dy) || ast[i].check_collision(s1.getx(), s1.gety())) {

				loop = 0;

			}

			// Loop screen elements to opposite side

			int border = 24;

			if (ast[i].getx() >= width - ast[i].getSize()*border) {
				ast[i].setx(ast[i].getSize()*border);
			}
			else if (ast[i].getx() <= ast[i].getSize()*border) {
				ast[i].setx(width - ast[i].getSize()*border);
			}

			if (ast[i].gety() >= height - ast[i].getSize()*border) {
				ast[i].sety(ast[i].getSize()*border);
			}
			else if (ast[i].gety() <= ast[i].getSize()*border) {
				ast[i].sety(height - ast[i].getSize()*border);
			}

		}

		// Add more asteroids if necessary

		if (ast.size() == 0) {

			for (i = 0; i < rand() % 3 + 3; i++) {
				switch (rand() % 2 + 1) {
				case 1:
					color = c_cyan;
					break;
				case 2:
					color = c_red;
					break;
				case 3:
					color = c_green;
					break;

				}
				int ast_x, ast_y;
				do {
					ast_x = rand() % (width - 100) + 100;
					ast_y = rand() % (height - 100) + 100;
				} while (sqrt(pow(ast_x - s1.getx(), 2) + pow(ast_y - s1.gety(), 2)) <= 75);
				a0.set(rand() % 3 + 1, ast_x, ast_y, rand() % 6 / 2 + 1, rand() % 6 / 2 + 1, color);
				ast.push_back(a0);
			}

		}

		for (i = 0; i < ast.size(); i++) {
			ast[i].draw(screen);
		}

		pow0.draw(screen, c_red);

		// Flip screen, clear, wait, rinse, repeat

		SDL_Flip(screen);

		Draw_FillRect(screen, 0, 0, width, height, c_black);

		SDL_Delay(1000 / fps);

	}

	// Ending animation

	double zoomRadius;
	int zoomPause = 1;

	for (zoomRadius = 100; zoomRadius > 0; zoomRadius--) {
		if (zoomRadius > s1.getx()) {
			zoomRadius = s1.getx() - 1;
		}
		if (zoomRadius > width - s1.getx()) {
			zoomRadius = width - s1.getx() - 1;
		}
		if (zoomRadius > s1.gety()) {
			zoomRadius = s1.gety() - 1;
		}
		if (zoomRadius > height - s1.gety()) {
			zoomRadius = height - s1.gety() - 1;
		}
		s1.draw(screen, c_white);
		for (i = 0; i < ast.size(); i++) {
			ast[i].draw(screen);
		}
		Draw_Circle(screen, s1.getx(), s1.gety(), zoomRadius, c_white);
		SDL_Flip(screen);
		if (zoomPause) {
			SDL_Delay(30 * 1000 / fps);
			zoomPause = 0;
		}
		SDL_Delay(1000 / fps);
		Draw_FillRect(screen, 0, 0, width, height, c_black);
	}

	cout << endl << "Game over!" << endl << "Score: " << score << endl << endl;

	SDL_Quit();

}