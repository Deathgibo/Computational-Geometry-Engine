#pragma once
#include "Utils.h"

class Point
{
public:
	Point();
	Point(float xx, float yy, int radius = 9);

	void Render(SDL_Renderer* grender, float radius, bool blue = false);

	~Point();

	
	float x;
	float y;
	float storage;
	float printradius;
private:

};

