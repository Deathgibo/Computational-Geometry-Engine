#include "VoronoiMesh.h"

VoronoiMesh::VoronoiMesh()
{

}

int VoronoiMesh::ClosestSite(Point p)
{
	float smallest = 10000;
	int index = -1;
	for (int i = 0; i < sites.size(); i++)
	{
		Point tmp(sites[i].pos.x - p.x, sites[i].pos.y - p.y);
		float magnitude = sqrt(pow(tmp.x, 2) + pow(tmp.y, 2));
		if (magnitude < smallest)
		{
			smallest = magnitude;
			index = i;
		}
		else if (magnitude == smallest)
		{
			printf("on border\n");
			return -1;
		}
	}
	if (index == -1)
		printf("closest site error\n");

	return index;
}

int VoronoiMesh::ClosestSitewithoutone(Point p, int indexz, int index2)
{
	float smallest = 10000000;
	int index = -1;
	for (int i = 0; i < sites.size(); i++)
	{
		Point tmp(sites[i].pos.x - p.x, sites[i].pos.y - p.y);
		float magnitude = sqrt(pow(tmp.x, 2) + pow(tmp.y, 2));
		if (magnitude < smallest && i != indexz && i !=index2)
		{
			smallest = magnitude;
			index = i;
		}
		else if (magnitude == smallest && i != indexz)
		{
			printf("on border\n");
			return -1;
		}
	}
	if (index == -1)
		printf("closest site error\n");

	return index;
}

void VoronoiMesh::Removevpoints(int index)
{
	vpoints.erase(vpoints.begin() + index);

	for (int i = 0; i < vedges.size(); i++)
	{
		if (vedges[i].p1index == index || vedges[i].p2index == index)
		{
			Removevedge(i);
			i--;
		}
		else
		{
			if (vedges[i].p1index > index)
			{
				vedges[i].p1index--;
			}
			if (vedges[i].p2index > index)
			{
				vedges[i].p2index--;
			}
		}
	}
}
void VoronoiMesh::Removevedge(int index)
{
	/*remove edges neighboring sites references to the edge*/
	for (int s = 0; s < sites[vedges[index].s1index].edgeindices.size(); s++)
	{
		if (sites[vedges[index].s1index].edgeindices[s] == index)
		{
			sites[vedges[index].s1index].edgeindices.erase(sites[vedges[index].s1index].edgeindices.begin() + s);
		}
	}
	for (int s = 0; s < sites[vedges[index].s2index].edgeindices.size(); s++)
	{
		if (sites[vedges[index].s2index].edgeindices[s] == index)
		{
			sites[vedges[index].s2index].edgeindices.erase(sites[vedges[index].s2index].edgeindices.begin() + s);
		}
	}

	vedges.erase(vedges.begin() + index);
	/*lower indices references for all sites*/
	for (int i = 0; i < sites.size(); i++)
	{
		for (int j = 0; j < sites[i].edgeindices.size(); j++)
		{
			if (sites[i].edgeindices[j] > index)
			{
				sites[i].edgeindices[j]--;
			}
			else if (sites[i].edgeindices[j] == index)
			{
				printf("ERROR remove edge!\n");
			}
		}
	}
}
void VoronoiMesh::Addvpoints(float x, float y)
{
	Point tmp(x, y);
	vpoints.push_back(tmp);
}
void VoronoiMesh::Addvedge(int point1, int point2, int site1, int site2)
{
	edge tmp;
	tmp.p1index = point1; tmp.p2index = point2; tmp.s1index = site1; tmp.s2index = site2;
	vedges.push_back(tmp);
}

void VoronoiMesh::Addsite(Point position)
{
	site tmp;
	tmp.pos = position;
	sites.push_back(tmp);
}

void VoronoiMesh::SiteAddEdge(int siteindex, int edgeindex)
{
	sites[siteindex].edgeindices.push_back(edgeindex);
}

void VoronoiMesh::Draw(SDL_Renderer* grender)
{
	for (int i = 0; i < vpoints.size(); i++)
	{
		vpoints[i].Render(grender, 3.0f);
	}
	//convexhull.Render(grender);
	SDL_SetRenderDrawColor(grender, 0, 0, 0, 0);
	for (int i = 0; i < vedges.size(); i++)
	{
		SDL_RenderDrawLine(grender, vpoints[vedges[i].p1index].x, vpoints[vedges[i].p1index].y,
			vpoints[vedges[i].p2index].x, vpoints[vedges[i].p2index].y);
	}
	for (int i = 0; i < sites.size(); i++)
	{
		sites[i].pos.Render(grender, 5.0f);
	}
}

void VoronoiMesh::PrintInfo()
{
	for (int i = 0; i < vpoints.size(); i++)
	{
		printf("(%f, %f) ", vpoints[i].x, vpoints[i].y);
	}
	printf("\n");
	for (int i = 0; i < vedges.size(); i++)
	{
		printf("edge%d: (%d)-(%d) (%d)-(%d)\n", i, vedges[i].p1index, vedges[i].p2index, vedges[i].s1index, vedges[i].s2index);
	}
	printf("\n");
}

void VoronoiMesh::CheckStatus()
{
	printf("CheckStatus begin\n");
	for (int i = 0; i < sites.size(); i++)
	{
		if (sites[i].edgeindices.size() == 0)
		{
			printf("CheckStatus: Error in sites edge count zero\n");
		}
		for (int j = 0; j < sites[i].edgeindices.size(); j++)
		{
			if (sites[i].edgeindices[j] < 0)
			{
				printf("CheckStatus: Error in sites edge indices negative\n");
			}
			if (sites[i].edgeindices[j] >= vedges.size())
			{
				printf("CheckStatus: Error in sites edge indices oversized\n");
			}
		}
	}
	for (int i = 0; i < vedges.size(); i++)
	{
		if (vedges[i].p1index < 0 || vedges[i].p2index < 0)
		{
			printf("CheckStatus: Error in edges point indices negative\n");
		}
		if (vedges[i].p1index >= vpoints.size() || vedges[i].p2index >= vpoints.size())
		{
			printf("CheckStatus: Error in edges point indices oversized\n");
		}
	}
	printf("CheckStatus done...\n");
}

void VoronoiMesh::CleanUp()
{
	sites.clear();
	vedges.clear();
	vpoints.clear();
	//look for all points that aren't in edges
}

void VoronoiMesh::MaintainConvex(Point p)
{

	/*bool visible = false;
	bool switched = false;
	float ydiff, xdiff;
	int index1 = -1;
	int index2 = -1;
	for (int j = 0; j < convexhull.points.size(); j++)
	{
		ydiff = convexhull.points[(j + 1) % convexhull.points.size()].y - convexhull.points[j].y;
		xdiff = convexhull.points[(j + 1) % convexhull.points.size()].x - convexhull.points[j].x;
		Point r(1, (-xdiff / ydiff));

		Point v1 = pointsubract(p, convexhull.points[j]);
		int index = 0;
		if (j == 0) {
			index = convexhull.points.size() - 1;
		}
		else {
			index = j - 1;
		}
		Point v2 = pointsubract(convexhull.points[index], convexhull.points[j]);
		float value1 = dotproduct(r, v1);
		float value2 = dotproduct(r, v2);

		if ((value1 < 0 && value2 < 0) || (value1 > 0 && value2 > 0))//invisible
		{
			if (switched == true)
			{
				index2 = j;
				convexhull.points.erase(convexhull.points.begin() + index1 + 1, convexhull.points.begin() + index2);
				convexhull.points.push_back(p);
				break;
			}
		}
		else//visible
		{
			if (visible == false)
			{
				visible = true;
				switched = true;
				index1 = j;
			}
		}
		*/
		/*shouldn't reach here end catch error means its edge 0 and 1*/
		/*if ((j == convexhull.points.size() - 1))
		{
			index2 = 0;
			convexhull.points.erase(convexhull.points.begin() + index1 + 1, convexhull.points.begin() + convexhull.points.size() - 1);
			convexhull.points.insert(convexhull.points.begin() + index1 + 1, p);
		}
	}*/
}

Point VoronoiMesh::pointsubract(const Point& p1, const Point& p2)
{
	Point tmp(p1.x - p2.x, p1.y - p2.y);
	return tmp;
}

float VoronoiMesh::dotproduct(Point p1, Point p2)
{
	return p1.x*p2.x + p1.y*p2.y;
}

VoronoiMesh::~VoronoiMesh()
{

}
