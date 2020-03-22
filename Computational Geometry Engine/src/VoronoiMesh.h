#pragma once
#include "Point.h"
#include "Polygon.h"
#include "Utils.h"
#include <glm\glm.hpp>
#include <vector>
#include <SDL.h>

class VoronoiMesh
{
public:
	VoronoiMesh();
	int ClosestSite(Point p);
	int ClosestSitewithoutone(Point p, int index, int index2);
	void Removevpoints(int index);
	void Removevedge(int index);
	void Addvpoints(float x, float y);
	void Addvedge(int point1, int point2, int site1, int site2);
	void Addsite(Point position);
	void SiteAddEdge(int siteindex, int edgeindex);
	void PrintInfo();
	void CheckStatus();
	void CleanUp();
	void MaintainConvex(Point p);
	Point pointsubract(const Point& p1, const Point& p2);
	float dotproduct(Point p1, Point p2);
	void Draw(SDL_Renderer* grender);
	~VoronoiMesh();

	struct edge;
	struct site {
		Point pos;
		std::vector<int> edgeindices;
	};
	struct edge {
		int p1index;
		int p2index;
		int s1index;
		int s2index;
	};
	//if a pushback no index change on delete find spot of deletion and deincrement those
	std::vector<site> sites;
	std::vector<edge> vedges;
	std::vector<Point> vpoints;
	Polygon convexhull;
};

