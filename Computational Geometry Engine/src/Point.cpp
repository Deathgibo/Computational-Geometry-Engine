#include "Point.h"

Point::Point()
{
	x = 0;
	y = 0;
	printradius = 1;
	storage = -1;
}

Point::Point(float xx, float yy, int radius)
{
	x = xx;
	y = yy;
	storage = -1;
	printradius = radius;
}

void Point::Render(SDL_Renderer* grender, float radius, bool blue)
{

	SDL_SetRenderDrawColor(grender, 0, 0, 0, 255);
	if (blue)
		SDL_SetRenderDrawColor(grender, 0, 0, 255, 255);
	Utils::CircleBresenhamsMidPoint(grender, x, y, radius);
	SDL_SetRenderDrawColor(grender, 255, 255, 0, 255);
	if (blue)
		SDL_SetRenderDrawColor(grender, 0, 0, 255, 255);
	Utils::CircleBresenhamsMidPoint(grender, x, y, radius - 3);
}

Point::~Point()
{

}
