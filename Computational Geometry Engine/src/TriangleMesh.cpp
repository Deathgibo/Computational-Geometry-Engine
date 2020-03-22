#pragma once
#include "TriangleMesh.h"

TriangleMesh::TriangleMesh()
{

}
void TriangleMesh::AddVertices(float x, float y)
{
	if(FindPointIndex(x, y) == -1)
	{
		Point tmp(x, y);
		vertices.push_back(tmp);
	}
}
void TriangleMesh::Addtriangle(Point p1, Point p2, Point p3)
{
	glm::vec3 tmp;
	tmp.x = FindPointIndex(p1.x, p1.y);
	tmp.y = FindPointIndex(p2.x, p2.y);
	tmp.z = FindPointIndex(p3.x, p3.y);
	if (tmp.x == -1 || tmp.y == -1 || tmp.z == -1)
	{
		printf("triangle creation error\n"); return;
	}
	tindex.push_back(tmp);
}
void TriangleMesh::Addtriangle(int p1, int p2, int p3)
{
	glm::vec3 tmp = glm::vec3(p1, p2, p3);
	tindex.push_back(tmp);
}
int TriangleMesh::FindPointIndex(float x, float y)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		if (x == vertices[i].x && y == vertices[i].y)
		{
			return i;
		}
	}
	return -1;
}
void TriangleMesh::PrintInfo()
{
	printf("Vertices: ");
	for (int i = 0; i < vertices.size(); i++)
	{
		printf("(%.2f, %.2f, %.2f) ", vertices[i].x, vertices[i].y);
	}
	printf("\n");
	printf("Indices: ");
	for (int i = 0; i < tindex.size(); i++)
	{
		printf("(%.0f, %.0f, %.0f) ", tindex[i].x, tindex[i].y, tindex[i].z);
	}
	printf("\n");
}
void TriangleMesh::Draw(SDL_Renderer* grender)
{
	for (int i = 0; i < tindex.size(); i++)
	{
		DrawTriangle(grender, vertices[tindex[i].x], vertices[tindex[i].y], vertices[tindex[i].z]);
	}
}

void TriangleMesh::DrawTriangle(SDL_Renderer* grender, Point one, Point two, Point three)
{
	SDL_SetRenderDrawColor(grender, 0, 0, 255, 0);

	SDL_RenderDrawLine(grender, one.x, one.y, two.x, two.y);
	SDL_RenderDrawLine(grender, two.x, two.y, three.x, three.y);
	SDL_RenderDrawLine(grender, three.x, three.y, one.x, one.y);
}

void TriangleMesh::Reset()
{
	tindex.clear();
	vertices.clear();
}
