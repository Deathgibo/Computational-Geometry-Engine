#pragma once
#include "Point.h"
#include <vector>

class Polygon
{
public:
	Polygon();
	Polygon(bool pointsetz);
	Polygon(int amount);
	Polygon(int amount,int lol);

	void Update(SDL_Renderer* grender, bool mouseclicked, bool mouserightclick, bool lastconnect = true, bool justlines = false, bool justpoints = false);
	void Reset();
	void Resettwo();
	void Resetthree();
	void MovePoint(int x, int y, int index);
	void AddPoint(int index);
	void AddPointvalue(float x, float y);
	void Render(SDL_Renderer* grender, bool lastconnect = true, bool justlines = false, bool justpoints = false, bool blue = false);
	void RenderPoints(SDL_Renderer* grender);
	void Changepointradius(int mousewheely);
	int PointClicked();
	void PrintPoints();

	~Polygon();

	std::vector<Point> points;
	int movingindex;
	bool pointset;
	float pointradius;
private:
};

