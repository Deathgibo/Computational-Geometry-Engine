#pragma once
#include <SDL.h>
#include <stdio.h>

class Button
{
public:
	Button(int x, int y, int w, int h, SDL_Texture* textz);

	void Update(bool mouseclicked, SDL_Renderer* grender);
	bool IsHovering();
	void ClickFunction();
	void HoverFunction();
	void Render(SDL_Renderer* grender, bool hovering = false, bool clicked = false);

	~Button();

	SDL_Rect rect;
	SDL_Texture* text;
	bool clicked;
	bool hovered;
private:
};