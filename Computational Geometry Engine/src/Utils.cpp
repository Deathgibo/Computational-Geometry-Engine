#include "Utils.h"


Utils::Utils()
{

}

void Utils::DrawCircleBresen(SDL_Renderer* grender, int xc, int yc, int R)
{
	for (int i = R; i > 0; i-=2)
	{
		CircleBresenhamsMidPoint(grender, xc, yc, i);
	}
}

void Utils::CircleBresenhamsMidPoint(SDL_Renderer* grender, int xc, int yc, int R)
{
	int x = 0, y = R;
	int d = 3 - 2 * R;
	SDL_RenderDrawLine(grender, x + xc, y + yc, -x + xc, y + yc);	
	SDL_RenderDrawLine(grender, -x + xc, -y + yc, x + xc, -y + yc);
	SDL_RenderDrawLine(grender, y + xc, x + yc, -y + xc, x + yc);
	SDL_RenderDrawLine(grender, -y + xc, -x + yc, y + xc, -x + yc);
	while (y >= x)
	{
		// for each pixel we will 
		// draw all eight pixels 

		x++;

		// check for decision parameter 
		// and correspondingly  
		// update d, x, y 
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		SDL_RenderDrawLine(grender, x + xc, y + yc, -x + xc, y + yc);
		SDL_RenderDrawLine(grender, -x + xc, -y + yc, x + xc, -y + yc);
		SDL_RenderDrawLine(grender, y + xc, x + yc, -y + xc, x + yc);
		SDL_RenderDrawLine(grender, -y + xc, -x + yc, y + xc, -x + yc);
	}

	/*int currentx, currenty;
	int p;

	currentx = 0;
	currenty = R;
	p = 1 - R;

	for (int i = 0; currentx <= currenty; i++)
	{
		SDL_RenderDrawLine(grender, currentx + xc, currenty + yc, -currentx + xc, currenty + yc);	
		SDL_RenderDrawLine(grender, -currentx + xc, -currenty + yc, currentx + xc, -currenty + yc);
		SDL_RenderDrawLine(grender, currenty + xc, currentx + yc, -currenty + xc, currentx + yc);
		SDL_RenderDrawLine(grender, -currenty + xc, -currentx + yc, currenty + xc, -currentx + yc);

		if (p < 0)
		{
			currentx = currentx + 1;
			currenty = currenty;
			p = p + 2 * currentx + 3;
		}
		else
		{
			currentx = currentx + 1;
			currenty = currenty - 1;
			p = p + 2 * currentx - 2 * currenty + 5;
		}
	}*/
}

void Utils::CircleScan(SDL_Renderer* grender, int xc, int yc, int R)
{
	float radius = R;
	for (float x = xc - R; x < xc + R; x++)
	{
		for (float y = yc - R; y < yc + R; y++)
		{
			if (sqrt(pow(xc - x, 2) + pow(yc - y, 2)) <= radius)
			{
				SDL_RenderDrawPoint(grender, x, y);
			}
		}
	}
}

void Utils::DrawLine(SDL_Renderer* grender, float x1, float y1, float x2, float y2)
{
	float slope = (y2 - y1) / (x2 - x1);
	float recip = -(1 / slope);
	SDL_RenderDrawLine(grender, x1, y1, x2, y2);
	SDL_RenderDrawLine(grender, x1+1, y1+recip, x2+1, y2+recip);
	SDL_RenderDrawLine(grender, x1 - 1, y1 - recip, x2 - 1, y2 - recip);
}

Utils::~Utils()
{

}
