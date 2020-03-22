#include "Polygon.h"

Polygon::Polygon()
{
	pointradius = 9;
	movingindex = -1;
	pointset = false;
}

Polygon::Polygon(bool pointsetz)
{
	pointradius = 9;
	if (pointsetz == false)
	{
		Point point1(200, 300);
		Point point2(300, 300);
		Point point3(250, 400);

		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);

		movingindex = -1;
		pointset = pointsetz;
	}
	else
	{
		movingindex = -1;
		pointset = pointsetz;
	}
}

Polygon::Polygon(int amount)
{
	Resettwo();
}

Polygon::Polygon(int amount, int lol)
{
	Resetthree();
}

void Polygon::Resetthree()
{
	points.clear();

	Point point1(200, 400);
	Point point2(300, 400);
	Point point3(250, 500);
	Point point4(350, 500);

	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);
	points.push_back(point4);

	movingindex = -1;
}

void Polygon::Reset()
{
	points.clear();

	if (!pointset)
	{
		Point point1(200, 300);
		Point point2(300, 300);
		Point point3(250, 400);

		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
	}

	movingindex = -1;
}

void Polygon::Resettwo()
{
	points.clear();

	Point point1(400, 300);
	Point point2(500, 300);

	points.push_back(point1);
	points.push_back(point2);

	movingindex = -1;
}

void Polygon::Update(SDL_Renderer* grender, bool mouseclicked, bool mouserightclick, bool lastconnect, bool justlines, bool justpoints)
{
	int clickindex = PointClicked();

	if (pointset)
	{
		if (mouserightclick)
		{
			AddPoint(-1);
		}
	}
	else
	{
		if (mouserightclick && clickindex != -1)
		{
			AddPoint(clickindex);
		}
	}

	if (mouseclicked)
	{
		if (movingindex == -1)
		{
			movingindex = clickindex;
		}
	}
	else
	{
		movingindex = -1;
	}
	if (movingindex != -1)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		MovePoint(x, y, movingindex);
	}
	if (pointset)
	{
		Render(grender, false, false, true);
	}
	else
	{
		Render(grender, lastconnect, justlines, justpoints);
	}
}

void Polygon::MovePoint(int x, int y, int index)
{
	points[index].x = x; points[index].y = y;
}

void Polygon::AddPoint(int index)
{
	if (pointset)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		Point pointtmp(x, y);
		points.push_back(pointtmp);
	}
	else
	{
		Point pointtmp(points[index].x + 10, points[index].y + 10);
		//points.push_back(pointtmp);
		points.insert(points.begin() + index, pointtmp);
	}
}

void Polygon::AddPointvalue(float x, float y)
{
	Point pointtmp(x, y);
	points.push_back(pointtmp);
}

int Polygon::PointClicked()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	for (int i = 0; i < points.size(); i++)
	{
		if (sqrt(pow(points[i].x - x, 2) + pow(points[i].y - y, 2)) < points[i].printradius - 3)
		{
			return i;
			break;
		}
	}
	return -1;
}

void Polygon::Render(SDL_Renderer* grender, bool lastconnect, bool justlines, bool justpoints, bool blue)
{
	if (justpoints)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i].Render(grender, pointradius);
		}
		return;
	}

	if (!justlines)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i].Render(grender, pointradius, blue);
		}
	}
	
	SDL_SetRenderDrawColor(grender, 0, 0, 0, 255);
	for (int i = 0; i < points.size(); i++)
	{
		if (i == points.size() - 1)
		{
			if (lastconnect)
				SDL_RenderDrawLine(grender, points[i].x, points[i].y, points[0].x, points[0].y);
			break;
		}
		SDL_RenderDrawLine(grender, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
	}
}

void Polygon::RenderPoints(SDL_Renderer * grender)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Render(grender, pointradius);
	}
}

void Polygon::Changepointradius(int mousewheely)
{
	float value = .5f;

	if (mousewheely < 0)
	{
		pointradius -= value;
		if (pointradius <= 0)
		{
			pointradius += value;
		}
	}
	else
	{
		pointradius += value;
	}
}

void Polygon::PrintPoints()
{
	for (int i = 0; i < points.size(); i++)
	{
		printf("(%f, %f)", points[i].x, points[i].y);
	}
}

Polygon::~Polygon()
{
	points.clear();
}
