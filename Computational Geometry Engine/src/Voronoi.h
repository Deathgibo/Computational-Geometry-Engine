#pragma once
#include <glm\glm.hpp>
#include <vector>

class Voronoi
{
public:
	void Render(SDL_Renderer* grender)
	{
		for (int i = 0; i < vvertex.size(); i++)
		{
			Utils::CircleBresenhamsMidPoint(grender, vvertex[i].x, vvertex[i].y, 3);
		}
		for (int i = 0; i < vedges.size(); i++)
		{
			if (vedges[i].x != -1 && vedges[i].y != -1)
			{
				SDL_RenderDrawLine(grender, vvertex[vedges[i].x].x, vvertex[vedges[i].x].y, vvertex[vedges[i].y].x, vvertex[vedges[i].y].y);
			}
		}
	}
	void AddVertex(float x, float y)
	{
		glm::dvec2 tmp = glm::dvec2(x, y);
		vvertex.push_back(tmp);
	}
	void CreateEmptyEdge()
	{
		glm::uvec2 tmp = glm::uvec2(-1, -1);
		vedges.push_back(tmp);
	}
	void AddVertextoEdge(int eindex, int vindex)
	{
		if (vedges[eindex][0] == -1)
		{
			vedges[eindex][0] = vindex;
		}
		else if (vedges[eindex][1] == -1)
		{
			vedges[eindex][1] = vindex;
		}
		else
		{
			std::cout << "addvertextoedge ERROR: edge has 2 points already" << std::endl;
		}
	}
	int Getlastvertexindex()
	{
		return vvertex.size() - 1;
	}
	int Getlastedgeindex()
	{
		return vedges.size() - 1;
	} 
	void Clear()
	{
		vvertex.clear();
		vedges.clear();
	}
	std::vector<glm::dvec2> vvertex;
	std::vector<glm::uvec2> vedges;//tuple of indices edge = (1,2) corresponding to vpoints[1],vpoints[2]
private:
};