#include "Button.h"

Button::Button(int x, int y, int w, int h, SDL_Texture* textz)
{
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	text = textz;
	clicked = false;
	hovered = false;
}

void Button::Update(bool mouseclicked, SDL_Renderer* grender)
{
	if (IsHovering())
	{
		hovered = true;
		HoverFunction();
		if (mouseclicked)
		{
			clicked = true;
			ClickFunction();
		}
		else
		{
			clicked = false;
		}
	}
	else
	{
		hovered = false;
		clicked = false;
	}
	Render(grender, hovered, clicked);
}

bool Button::IsHovering()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if ((x > rect.x && x < rect.x + rect.w) && (y > rect.y && y < rect.y + rect.h))
	{
		return true;
	}
	return false;
}

void Button::ClickFunction()
{

}

void Button::HoverFunction()
{

}

void Button::Render(SDL_Renderer* grender, bool hovering, bool clicked)
{
	SDL_RenderCopy(grender, text, 0, &rect);
	if (hovering)
	{
		SDL_SetRenderDrawColor(grender, 0, 0, 0, 255);
		SDL_RenderDrawLine(grender, rect.x, rect.y, rect.x + rect.w, rect.y);
		SDL_RenderDrawLine(grender, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
		SDL_RenderDrawLine(grender, rect.x + rect.w, rect.y + rect.h, rect.x, rect.y + rect.h);
		SDL_RenderDrawLine(grender, rect.x, rect.y + rect.h, rect.x, rect.y);
	}
	if (clicked)
	{
		SDL_SetRenderDrawBlendMode(grender, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(grender, 0, 0, 255, 25);
		SDL_RenderFillRect(grender, &rect);
		SDL_SetRenderDrawBlendMode(grender, SDL_BLENDMODE_NONE);
	}
	//SDL_RenderPresent(grender);
}

Button::~Button()
{
	text = nullptr;
}
