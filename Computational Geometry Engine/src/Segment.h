#pragma once
#include <SDL.h>
#include "Utils.h"
#include <glm\glm.hpp>
#include <iostream>

class Segment
{
public:
	Segment();
	Segment(const Segment &e1)
	{
		one = e1.one;
		two = e1.two;
	}
	Segment(float x1, float y1, float x2, float y2);
	void Print(SDL_Renderer* grender);
	void Changeposition(bool upper, float x, float y);
	void DealWithUpper();
	~Segment();
	friend std::ostream& operator<<(std::ostream& os, const Segment& s)
	{
		os << s.one.x << " " << s.one.y << ", " << s.two.x << " " << s.two.y;
		return os;
	}
	void operator=(const Segment& e1)
	{
		one = e1.one;
		two = e1.two;
	}
	bool operator==(const Segment& e1)
	{
		if (one == e1.one && two == e1.two)
		{
			return true;
		}
		return false;
	}
	bool operator<(const Segment& e1)
	{
		if (one.x < e1.one.x)
		{
			return true;
		}
		if (one.x > e1.one.x)
		{
			return false;
		}
		if (one.x == e1.one.x)
		{
			if (one.x < e1.one.x)
			{
				return true;
			}
		}
		return false;
	}
	bool operator>(const Segment& e1)//switch this
	{
		if (one.x > e1.one.x)
		{
			return true;
		}
		if (one.x < e1.one.x)
		{
			return false;
		}
		if (one.x == e1.one.x)
		{
			if (one.x > e1.one.x)
			{
				return true;
			}
		}
		return false;
	}

	glm::vec2 one;
	glm::vec2 two;

private:
};

