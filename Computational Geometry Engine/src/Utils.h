#pragma once
#include <SDL.h>
#include <stdio.h>

class Utils
{
public:
	Utils();

	static void DrawCircleBresen(SDL_Renderer* grender, int xc, int yc, int R);
	static void CircleBresenhamsMidPoint(SDL_Renderer* grender, int xc, int yc, int R);
	static void CircleScan(SDL_Renderer* grender, int xc, int yc, int R);
	static void DrawLine(SDL_Renderer* grender, float x1, float y1, float x2, float y2);
	~Utils();
};

