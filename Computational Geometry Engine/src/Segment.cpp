#include "Segment.h"

Segment::Segment()
{
	one.x = -1; one.y = 0;
	two.x = 0; two.y = 0;
	DealWithUpper();
}

Segment::Segment(float x1, float y1, float x2, float y2)
{
	one.x = x1; one.y = y1;
	two.x = x2; two.y = y2;
	DealWithUpper();
}

void Segment::DealWithUpper()
{
	if (one.y > two.y)
	{
		glm::vec2 tmp = one;
		one = two;
		two = tmp;
	}
	if (one.y == two.y)
	{
		if (one.x > two.x)
		{
			glm::vec2 tmp = one;
			one = two;
			two = tmp;
		}
	}
}

void Segment::Changeposition(bool upper, float x, float y)
{
	if (upper)
	{
		one.x = x; one.y = y;
	}
	else
	{
		two.x = x; two.y = y;
	}
}

void Segment::Print(SDL_Renderer* grender)
{
	Utils::CircleBresenhamsMidPoint(grender, one.x, one.y, 5);
	Utils::CircleBresenhamsMidPoint(grender, two.x, two.y, 5);
	SDL_RenderDrawLine(grender, one.x, one.y, two.x, two.y);
}

Segment::~Segment()
{

}
