#pragma once
#include "Point.h"
#include "Utils.h"
#include <glm\glm.hpp>
#include <SDL.h>
#include <vector>
#include <stdlib.h>

class TriangleMesh
{
public:
	
	TriangleMesh();
	void AddVertices(float x, float y);
	void Addtriangle(Point p1, Point p2, Point p3);
	void Addtriangle(int p1, int p2, int p3);
	int FindPointIndex(float x, float y);
	void PrintInfo();
	void Draw(SDL_Renderer* grender);
	void DrawTriangle(SDL_Renderer* grender, Point one, Point two, Point three);
	void Reset();

	std::vector<glm::vec3> tindex;
	std::vector<Point> vertices;
private:
};