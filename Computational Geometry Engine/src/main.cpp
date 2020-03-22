#include <iostream>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include <algorithm>
#include <set>
#include <random>       // std::default_random_engine
#include <vector>
#include <map>
#include <list>
#include <string>
#include <ctime>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Button.h"
#include "Polygon.h"
#include "VoronoiMesh.h"
#include "TriangleMesh.h"
#include "Segment.h"
#include "BinaryTree.h"
#include "BinaryTreeSeg.h"
#include "FortuneEventTree.h"
#include "Voronoi.h"
#include "FortunesBeachLineTree.h"
using namespace std::chrono;
using namespace std;

/*
auto start = high_resolution_clock::now();
auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken by function: "
		<< duration.count() << " microseconds" << endl;
*/

const int screen_height = 580;
const int screen_width = 1000;

int mousex = 0;
int mousey = 0;

SDL_Window* gwindow = 0;
SDL_Surface* gsurface = 0;
SDL_Renderer* grender = 0;

SDL_Texture* resettexture;
SDL_Texture* incrementaltexture;
SDL_Texture* giftwraptexture;
SDL_Texture* grahamscantexture;
SDL_Texture* divideandconquertexture;
SDL_Texture* splittingtexture;
SDL_Texture* triangulateincrementtexture;
SDL_Texture* voronoitexture;
SDL_Texture* fortunestexture;
SDL_Texture* scanlinetexture;
SDL_Texture* addlinetexture;
SDL_Texture* addpointtexture;

struct events;
struct eventsround;
struct fortuneevent;

bool init();
bool loadmedia();
SDL_Texture* loadimage(string a);
SDL_Texture* loadimageclear(string a);
Polygon ConvexHullIncremental(Polygon& pointset);
Polygon ConvexHullGiftWrap(Polygon& pointset);
Polygon ConvexHullGrahamScan(Polygon& pointset);
void ConvexHullDivideandConquer(Polygon& convexhull, Polygon& pointset, int b, int e);
void ConvexHullDivideandConqureMerg(Polygon& convexhull, Polygon& pointset, int b, int m, int e);
Polygon ConvexHullDivideandConqureStart(Polygon& pointset);
TriangleMesh TriangulationIncremental(Polygon& pointset);
TriangleMesh TriangulationSplitting(Polygon& pointset);
VoronoiMesh VoronoiIncremental(Polygon& pointset);
Point LineIntersectionPoint(Point p1, Point p2, Point p3, Point p4);
bool IsPointInHull(Polygon& convexhull, Point p);
bool IsPointOnSegment(Point p1, Point p2, Point p);
bool Leftof(const Point& a, const Point& b, const Point& c);
int Leftofvalue(const Point& a, const Point& b, const Point& c);
float Leftoffloat(const Point& a, const Point& b, const Point& c);
float Leftoffloat(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
bool IsPointInTriangleError(Point t1, Point t2, Point t3, Point a);
int IsPointInTriangle(Point t1, Point t2, Point t3, Point a);
bool comparepoints(const Point& p1, const Point& p2);
bool PointStorageSort(const Point& p1, const Point& p2);
bool pointcompareright(const Point& p1, const Point& p2);
bool pointcomparerightback(const Point& p1, const Point& p2);
void FindIntersections(vector<Segment> segments, vector<Point>& points);
void HandleEventPoint(events p, vector<Point>& points);
void FindNewEvent(Segment sl, Segment sr, glm::vec2 p);
void FindIntersectionsround(vector<Segment> segments, vector<Point>& points);
void HandleEventPointround(eventsround p, vector<Point>& points, bool called);
void FindNewEventround(Segment sl, Segment sr, glm::vec2 p, eventsround P, vector<Point>& points, bool called);
bool SegmentsIntersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
bool OnSegment(glm::vec2 pi, glm::vec2 pj, glm::vec2 pk);
bool collinear(Point& p1, Point& p2, Point& p3);
float pointmagnitude(const Point& p);
float pointsmagnitudequick(const Point& p1, const Point& p2);
float dotproduct(Point p1, Point p2);
Point pointsubract(const Point& p1, const Point& p2);
Point pointadd(const Point& p1, const Point& p2);
Point pointmultiplyconstant(const Point& p1, float value);

Voronoi FortunesAlgorithm(Polygon& pointset);
void FortuneHandleSiteEvent(FortuneEventTree<fortuneevent>& Q, FortunesBeachLineTree& T, Voronoi& voronoi, fortuneevent tmp);
void FortuneHandleCircleEvent(FortuneEventTree<fortuneevent>& Q, FortunesBeachLineTree& T, Voronoi& voronoi, fortuneevent tmp);
glm::dvec3 CircleEventCheck(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c);
glm::dvec2 LineIntersectionPoint(glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3, glm::dvec2 p4);

void close();
int main(int argc, char* args[])
{
	srand(time(0));
	if (!init())
	{
		return 0;
	}
	if (!loadmedia())
	{
		return 0;
	}

	bool leftclick = false;
	bool leftclickhold = false;
	bool rightclick = false;
	bool voronoi = false;
	//bool segmentflow = true;
	int menunumber = 1;

	Button buttonreset(0, 0, 50, 50, resettexture);
	Button buttonincremental(51, 0, 100, 50, incrementaltexture);
	Button buttongiftwrap(152, 0, 100, 50, giftwraptexture);
	Button buttongrahamscan(253, 0, 100, 50, grahamscantexture);
	Button buttonaddpoints(354, 0, 100, 50, addpointtexture);
	Button buttondivideandconquer(354, 0, 100, 50, addpointtexture);
	Button buttonincrementaltriangulation(505, 0, 100, 50, triangulateincrementtexture);
	Button buttonsplittingtriangulation(606, 0, 100, 50, splittingtexture);
	Button buttonvoronoi(707, 0, 100, 50, voronoitexture);
	Button buttonfortunes(51, 0, 100, 50, fortunestexture);

	Button buttoncreatesegment(51, 0, 100, 50, addlinetexture);
	Button buttonsegmentintersect(152, 0, 100, 50, scanlinetexture);

	Polygon poly1(true);
	Polygon polyconvex;
	Polygon triangle;
	triangle.AddPointvalue(100, 100);
	triangle.AddPointvalue(210, 100);
	triangle.AddPointvalue(130, 155);
	TriangleMesh polytriangulation;
	VoronoiMesh voronoimesh;
	Voronoi fortunesvoronoi;

	vector<Segment> segments;
	vector<Point> pointintersects;

	bool framecounter = 0;
	bool printframe = 100;

	bool clickagain = false;

	bool leave = false;
	SDL_Event e;
	while (leave == false)
	{
		leftclick = false;
		rightclick = false;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				leave = true;
			}
			if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{

				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1: menunumber = 1; break;
				case SDLK_2: menunumber = 2; break;
				case SDLK_3: menunumber = 3; break;
				case SDLK_f: fortunesvoronoi = FortunesAlgorithm(poly1); break;
				default: break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				SDL_GetMouseState(&mousex, &mousey);
			}
			if (e.type == SDL_MOUSEWHEEL)
			{
				//cout << e.wheel.y << endl;
				//-1 is down 1 is up
				poly1.Changepointradius(e.wheel.y);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					rightclick = true;
				}
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					leftclickhold = true;
					leftclick = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_RIGHT)
				{

				}
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					leftclickhold = false;
				}
			}
		}

		/*						MAIN LOOP					*/

		SDL_SetRenderDrawColor(grender, 255, 255, 255, 0);
		SDL_RenderClear(grender);

		if (menunumber == 1)
		{
			buttonreset.Update(leftclick, grender);
			if (buttonreset.clicked)
			{
				poly1.Reset();
				fortunesvoronoi.Clear();
			}
			buttonfortunes.Update(leftclick, grender);
			if (buttonfortunes.clicked)
			{
				system("cls");
				printf("Implementing Fortunes Algorithm...\n");

				auto start = high_resolution_clock::now();

				fortunesvoronoi = FortunesAlgorithm(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			printframe = poly1.points.size() * 5;
			if (framecounter >= printframe)
			{

				poly1.Update(grender, leftclickhold, rightclick, true, false, false);
				polyconvex.pointradius = poly1.pointradius;
				polyconvex.Render(grender, true, false, false, true);
				fortunesvoronoi.Render(grender);
				framecounter = 0;
			}
		}
		else if (menunumber == 2)
		{
			buttonreset.Update(leftclick, grender);
			if (buttonreset.clicked)
			{
				segments.clear();
				pointintersects.clear();
			}
			buttoncreatesegment.Update(leftclick, grender);
			if (buttoncreatesegment.clicked)
			{
				for (int i = 0; i < 10; i++)
				{
					Segment seg(rand() % screen_width, rand() % screen_height, rand() % screen_width, rand() % screen_height);
					segments.push_back(seg);
				}
			}
			buttonsegmentintersect.Update(leftclick, grender);
			if (buttonsegmentintersect.clicked)
			{
				system("cls");
				cout << "Implementing scan line intersection algorithm" << endl;
				FindIntersections(segments,pointintersects);
				cout << "Intersections found: " << pointintersects.size() << endl;
			}
			if (rightclick)
			{
				int xx, yy;
				SDL_GetMouseState(&xx, &yy);
				if (clickagain == false)
				{
					Segment tmp(xx, yy, xx, yy);
					segments.push_back(tmp);
					clickagain = true;
				}
				else
				{
					segments[segments.size() - 1].two.x = xx;
					segments[segments.size() - 1].two.y = yy;
					segments[segments.size() - 1].DealWithUpper();
					clickagain = false;
				}
			}
			SDL_SetRenderDrawColor(grender, 0, 0, 0, 0);
			for (int i = 0; i < segments.size(); i++)
			{
				segments[i].Print(grender);
			}
			for (int i = 0; i < pointintersects.size(); i++)
			{
				Utils::CircleBresenhamsMidPoint(grender, pointintersects[i].x, pointintersects[i].y, 2);
			}
		}
		else if (menunumber == 3)
		{
			buttonreset.Update(leftclick, grender);
			if (buttonreset.clicked)
			{
				poly1.Reset();
				polyconvex.points.clear();
				polytriangulation.Reset();
				voronoimesh.CleanUp();
				voronoimesh.convexhull.Reset();
			}
			buttonincremental.Update(leftclick, grender);
			if (buttonincremental.clicked)
			{
				system("cls");
				printf("Implementing Incremental algorithm...\n");

				for (int i = 0; i < 1000; i++)
				{
					//poly1.AddPointvalue((rand() % (screen_width-70)) + 40, (rand() % (screen_height - 80)) + 50);
				}
				auto start = high_resolution_clock::now();

				polyconvex = ConvexHullIncremental(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			buttongiftwrap.Update(leftclick, grender);
			if (buttongiftwrap.clicked)
			{
				system("cls");
				printf("Implementing GiftWrapping algorithm...\n");

				for (int i = 0; i < 1000; i++)
				{
					//poly1.AddPointvalue((rand() % (screen_width-70)) + 40, (rand() % (screen_height - 80)) + 50);
				}
				auto start = high_resolution_clock::now();

				polyconvex = ConvexHullGiftWrap(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			buttongrahamscan.Update(leftclick, grender);
			if (buttongrahamscan.clicked)
			{
				system("cls");
				printf("Implementing Graham Scan algorithm...\n");

				for (int i = 0; i < 100; i++)
				{
					//poly1.AddPointvalue((rand() % (screen_width - 70)) + 40, (rand() % (screen_height - 80)) + 50);
				}
				auto start = high_resolution_clock::now();

				polyconvex = ConvexHullGrahamScan(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			buttondivideandconquer.Update(leftclick, grender);
			if (buttondivideandconquer.clicked)
			{
				for (int i = 0; i < 50; i++)
				{
					poly1.AddPointvalue((rand() % (screen_width-70)) + 40, (rand() % (screen_height - 80)) + 50);
				}
			}
			buttonincrementaltriangulation.Update(leftclick, grender);
			if (buttonincrementaltriangulation.clicked)
			{
				system("cls");
				printf("Implementing Incremental Triangulation Algorithm...\n");

				auto start = high_resolution_clock::now();

				polytriangulation = TriangulationIncremental(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			buttonsplittingtriangulation.Update(leftclick, grender);
			if (buttonsplittingtriangulation.clicked)
			{
				system("cls");
				printf("Implementing Splitting Triangulation Algorithm...\n");

				auto start = high_resolution_clock::now();

				polytriangulation = TriangulationSplitting(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			buttonvoronoi.Update(leftclick, grender);
			if (buttonvoronoi.clicked)
			{
				//voronoi = !voronoi;
				system("cls");
				printf("Implementing Voronoi Incremental Algorithm...\n");

				auto start = high_resolution_clock::now();

				voronoimesh = VoronoiIncremental(poly1);

				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "N: " << poly1.points.size() << " time: " << duration.count() << " microseconds" << endl;
			}
			printframe = poly1.points.size() * 5;
			if (voronoi)
			{
				//voronoimesh = VoronoiIncremental(poly1);
				voronoimesh = VoronoiIncremental(poly1);
				if (framecounter >= printframe)
				{
					poly1.Update(grender, leftclickhold, rightclick, true, false, false);
					voronoimesh.Draw(grender);
					framecounter = 0;
				}
			}
			else
			{
				if (framecounter >= printframe)
				{
					poly1.Update(grender, leftclickhold, rightclick, true, false, false);
					polyconvex.pointradius = poly1.pointradius;
					polyconvex.Render(grender, true, false, false, true);
					polytriangulation.Draw(grender);
					//triangle.Render(grender);
					voronoimesh.Draw(grender);
					framecounter = 0;
				}
			}
		}


		SDL_RenderPresent(grender);
		framecounter++;
	}

	return 0;

}

bool init() //initialize video, screen, render and color, lineartexture, IMG
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			success = false;
		}

		gwindow = SDL_CreateWindow("Computational Geometry Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if (gwindow == 0)
		{
			success = false;
		}
		grender = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (grender == 0)
		{
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(grender, 255, 255, 255, 255);

			int imgflags = IMG_INIT_PNG;
			if (!(IMG_Init(imgflags) & imgflags))
			{
				success = false;
			}
			//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		}
	}


	return success;
}

bool loadmedia()
{
	bool success = true;

	resettexture = loadimage("Images/reset.png");
	incrementaltexture = loadimage("Images/incremental.png");
	giftwraptexture = loadimage("Images/giftwrap.png");
	grahamscantexture = loadimage("Images/grahamscan.png");
	divideandconquertexture = loadimage("Images/divideandconqure.png");
	triangulateincrementtexture = loadimage("Images/incrementaltriang.png");
	splittingtexture = loadimage("Images/splitting.png");
	voronoitexture = loadimage("Images/voronoi.png");
	fortunestexture = loadimage("Images/fortunes.png");
	scanlinetexture = loadimage("Images/scanline.png");
	addlinetexture = loadimage("Images/addline.png");
	addpointtexture = loadimage("Images/addpoints.png");

	return success;
}

SDL_Texture* loadimage(string a)
{
	SDL_Surface* tempsurf;
	tempsurf = IMG_Load(a.c_str());
	if (tempsurf == 0)
	{
		cout << "could not load image";
	}
	SDL_Texture* temptext;
	temptext = SDL_CreateTextureFromSurface(grender, tempsurf);

	SDL_FreeSurface(tempsurf);
	return temptext;
}

SDL_Texture* loadimageclear(string a) //white
{
	SDL_Surface* tempsurf;
	tempsurf = IMG_Load(a.c_str());
	if (tempsurf == 0)
	{
		cout << "could not load image";
	}
	SDL_SetColorKey(tempsurf, SDL_TRUE, SDL_MapRGB(tempsurf->format, 255, 255, 255));
	SDL_Texture* temptext;
	temptext = SDL_CreateTextureFromSurface(grender, tempsurf);

	SDL_FreeSurface(tempsurf);
	return temptext;
}

void close()
{
	SDL_DestroyWindow(gwindow);
	gwindow = 0;
	SDL_DestroyRenderer(grender);
	grender = 0;

	SDL_Quit();
	IMG_Quit();
}
/*FORTUNES ALGORITHM START*/
glm::dvec2 LineIntersectionPoint(glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3, glm::dvec2 p4)
{
	glm::dvec2 d1 = p2 - p1;
	glm::dvec2 d2 = p4 - p3;
	if (p2.x != p1.x || p2.y != p1.y)
	{
		//d1 = glm::normalize(d1);
	}
	//d1 = glm::normalize(glm::dvec2(p2.x, p2.y) - glm::dvec2(p1.x, p1.y));
	//d2 = glm::normalize(glm::dvec2(p4.x, p4.y) - glm::dvec2(p3.x, p3.y));
	glm::dmat2x2 A;
	A[0][0] = d1.x; A[1][0] = d2.x;
	A[0][1] = d1.y; A[1][1] = d2.y;
	glm::dvec2 b = p3 - p1;
	//cout << A[0][0] << "|" << A[1][0] << endl;
	//cout << A[0][1] << "|" << A[1][1] << endl;
	if (abs(glm::determinant(A)) <= 0.01f)
	{
		cout << "parralel lines" << endl;
		glm::dvec2 tmp(-10000, -10000);
		return tmp;
	}
	glm::dvec2 x = glm::inverse(A) * b;

	glm::dvec2 intersection = glm::dvec2(p1.x, p1.y) + x.x*d1;
	return intersection;
}

glm::dvec3 CircleEventCheck(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c)
{
	//cout << "inside circle" << endl;
	glm::dvec3 tmp = glm::dvec3(-10000, -10000,-10000);
	if ((a.x == -1 && a.y == -1) || (b.x == -1 && b.y == -1) || (c.x == -1 && c.y == -1))
	{
		return tmp;
	}
	//calculate ab bisector
	glm::dvec2 midab = (a + b); midab.x = midab.x / 2.0; midab.y = midab.y / 2.0;
	glm::dvec2 dirab = b - a;
	double savey = dirab.y; dirab.y = -dirab.x; dirab.x = savey;

	//calculate bc bisector
	glm::dvec2 midbc = (b + c); midbc.x = midbc.x / 2.0; midbc.y = midbc.y / 2.0;
	glm::dvec2 dirbc = c - b;
	double savey2 = dirbc.y; dirbc.y = -dirbc.x; dirbc.x = savey2;
	
	//find circumcenter and radius
	glm::dvec2 circumcenter = LineIntersectionPoint(midab, midab + dirab, midbc, midbc + dirbc);
	if (circumcenter.x == -10000 && circumcenter.y == -10000)
	{
		return tmp;
	}
	double radius = glm::distance(circumcenter, a);
	extern double scanliney;
	//cout << "scaneliney: " << scanliney << endl;
	if (circumcenter.y + radius > scanliney)// + .001
	{
		circumcenter.y += radius;
		glm::dvec3 c(circumcenter.x, circumcenter.y, radius);
		//scanliney = circumcenter.y;
		return c;
	}
	return tmp;
}

Voronoi FortunesAlgorithm(Polygon& pointset)
{
	extern double scanliney;
	FortuneEventTree<fortuneevent> Q;
	Voronoi voronoi;
	FortunesBeachLineTree T;
	T.Q = &Q;
	T.voronoi = &voronoi;
	T.Sites = &pointset;


	//initialize Q with all sites
	for (int i = 0; i < pointset.points.size(); i++)
	{
		fortuneevent tmp;
		tmp.siteevent = true;
		tmp.site.x = pointset.points[i].x;
		tmp.site.y = pointset.points[i].y;
		tmp.radius = 0;
		tmp.leafz = NULL;
		Q.insert(tmp);
	}

	while (!Q.empty())
	{
		fortuneevent tmp = Q.poptop();
		scanliney = tmp.site.y;
		//T.printnodeinfo();
		if (tmp.siteevent == true)
		{
			//cout << "inside site event"<<endl;
			FortuneHandleSiteEvent(Q, T, voronoi, tmp);
			//cout << "outside site event"<<endl;
		}
		else
		{
			//cout << "inside circle event" << endl;
			FortuneHandleCircleEvent(Q, T, voronoi, tmp);
			//cout << "outside circle event" << endl;
		}
	}
	T.BoundVoronoi();
	//T.printnodeinfo();
	return voronoi;
}
void FortuneHandleSiteEvent(FortuneEventTree<fortuneevent>& Q, FortunesBeachLineTree& T, Voronoi& voronoi, fortuneevent tmp)
{
	if (T.empty())
	{
		beachnode newsite;
		newsite.leaf = true;
		newsite.leafsite = tmp.site;
		newsite.leafevent = NULL;
		T.insert(newsite, -1);
		return;
	}
	T.insertedleft = NULL;
	T.insertedright = NULL;
	beachnode newsite;
	newsite.leaf = true;
	newsite.leafsite = tmp.site;
	newsite.leafevent = NULL;
	voronoi.CreateEmptyEdge();
	int index = voronoi.Getlastedgeindex();
	T.insert(newsite, index);
	//T.printnodeinfo();

	//assert(T.insertedright != NULL, "inserted right doestne exist s");
	//assert(T.insertedleft != NULL, "inserted left doesnt exist s");
	
	sitetuple rightarc = T.neighborfunction(T.insertedright);
	sitetuple leftarc = T.neighborfunction(T.insertedleft);

	
	/*cout << "left:(" <<     rightarc.leftsite.x << "," << rightarc.leftsite.y << 
		    ") middle:(" << T.insertedright->value.leafsite.x << "," << T.insertedright->value.leafsite.y << 
			")right:(" <<   rightarc.rightsite.x << "," << rightarc.rightsite.y << ")" << endl;*/
	if ((rightarc.leftsite.x == -1 && rightarc.leftsite.y == -1) || (rightarc.rightsite.x == -1 && rightarc.rightsite.y == -1))
	{

	}
	else
	{
		//*sites aren't pointers just copies
		if (Leftoffloat(rightarc.leftsite, T.insertedright->value.leafsite, rightarc.rightsite) < 0)
		{
			glm::dvec3 circumcenter = CircleEventCheck(rightarc.leftsite, T.insertedright->value.leafsite, rightarc.rightsite);
			if (circumcenter.x != -10000 && circumcenter.y != -10000)
			{
				//cout << "new event022!" << circumcenter.x << " " << circumcenter.y << " " << circumcenter.z << endl;
				fortuneevent rightevent;
				assert(T.insertedright->value.leaf == true);
				rightevent.leafz = T.insertedright;
				rightevent.siteevent = false;
				rightevent.site = circumcenter;
				rightevent.radius = circumcenter.z;
				Q.insert(rightevent);
				T.insertedright->value.leafevent = Q.recentlyinsertedevent;
			}
		}
	}
	
	/*cout << "left:(" <<     leftarc.leftsite.x << "," << leftarc.leftsite.y << 
		    ") middle:(" << T.insertedleft->value.leafsite.x << "," << T.insertedleft->value.leafsite.y <<
		    ")right:(" <<   leftarc.rightsite.x << "," << leftarc.rightsite.y << ")" << endl;*/
	if ((leftarc.leftsite.x == -1 && leftarc.leftsite.y == -1) || (leftarc.rightsite.x == -1 && leftarc.rightsite.y == -1))
	{

	}
	else
	{
		if (Leftoffloat(leftarc.leftsite, T.insertedleft->value.leafsite, leftarc.rightsite) < 0)
		{
			glm::dvec3 circumcenter = CircleEventCheck(leftarc.leftsite, T.insertedleft->value.leafsite, leftarc.rightsite);
			if (circumcenter.x != -10000 && circumcenter.y != -10000)
			{
				//cout << "new event00!" << circumcenter.x << " " << circumcenter.y << " " << circumcenter.z << endl;
				fortuneevent leftevent;
				assert(T.insertedleft->value.leaf == true);
				leftevent.leafz = T.insertedleft;
				leftevent.siteevent = false;
				leftevent.site = circumcenter;
				leftevent.radius = circumcenter.z;
				Q.insert(leftevent);
				T.insertedleft->value.leafevent = Q.recentlyinsertedevent;
			}
		}
	}
}
//**error when deletion it could be [a]<-(a,a)->[a]
void FortuneHandleCircleEvent(FortuneEventTree<fortuneevent>& Q, FortunesBeachLineTree& T, Voronoi& voronoi, fortuneevent tmp)
{
	T.insertedleft = NULL;
	T.insertedright = NULL;
	//cout << "circle event site: " << tmp.site.x << "," << tmp.site.y - tmp.radius << endl;
	voronoi.AddVertex(tmp.site.x, tmp.site.y - tmp.radius);
	int vindex = voronoi.Getlastvertexindex();
	voronoi.CreateEmptyEdge();
	int eindex = voronoi.Getlastedgeindex();

	T.deletetripleevent(tmp.leafz, eindex, vindex);
	//cout << "Deleted triple";
	//assert(T.insertedright != NULL, "inserted right doestne exist c");
	//assert(T.insertedleft != NULL, "inserted left doesnt exist c");
	bool left = false;
	if (tmp.leafz->parent != NULL && (tmp.leafz->parent->left->value == tmp.leafz->value))
	{
		left = true;
	}
	T.makedeletion(tmp.leafz, left, eindex, vindex);
	//cout << "made deletion";
	//T.printnodeinfo();
	if (tmp.leafz->parent != NULL && tmp.leafz->parent->parent != NULL)
	{
		//cout << tmp.leafz->parent->parent->value.internalsites.leftsite.x << " " << tmp.leafz->parent->parent->value.internalsites.leftsite.y
			//<< ", " << tmp.leafz->parent->parent->value.internalsites.rightsite.x << " " << tmp.leafz->parent->parent->value.internalsites.rightsite.y << endl;
		assert(tmp.leafz->parent->parent->value.internalsites.leftsite != tmp.leafz->parent->parent->value.internalsites.rightsite);
	}

	sitetuple rightarc = T.neighborfunction(T.insertedright);
	sitetuple leftarc = T.neighborfunction(T.insertedleft);


	/*cout << "left:(" << rightarc.leftsite.x << "," << rightarc.leftsite.y <<
		") middle:(" << T.insertedright->value.leafsite.x << "," << T.insertedright->value.leafsite.y <<
		")right:(" << rightarc.rightsite.x << "," << rightarc.rightsite.y << ")" << endl;*/
	if ((rightarc.leftsite.x == -1 && rightarc.leftsite.y == -1) || (rightarc.rightsite.x == -1 && rightarc.rightsite.y == -1))
	{

	}
	else
	{
		//*sites aren't pointers just copies
		if (Leftoffloat(rightarc.leftsite, T.insertedright->value.leafsite, rightarc.rightsite) < 0)
		{
			glm::dvec3 circumcenter = CircleEventCheck(rightarc.leftsite, T.insertedright->value.leafsite, rightarc.rightsite);
			if (circumcenter.x != -10000 && circumcenter.y != -10000)
			{
				//cout << "new event22!" << circumcenter.x << " " << circumcenter.y << " " << circumcenter.z << endl;
				fortuneevent rightevent;
				assert(T.insertedright->value.leaf == true);
				rightevent.leafz = T.insertedright;
				rightevent.siteevent = false;
				rightevent.site = circumcenter;
				rightevent.radius = circumcenter.z;
				Q.insert(rightevent);
				T.insertedright->value.leafevent = Q.recentlyinsertedevent;
			}
		}
	}

	/*cout << "left:(" << leftarc.leftsite.x << "," << leftarc.leftsite.y <<
		") middle:(" << T.insertedleft->value.leafsite.x << "," << T.insertedleft->value.leafsite.y <<
		")right:(" << leftarc.rightsite.x << "," << leftarc.rightsite.y << ")" << endl;*/
	if ((leftarc.leftsite.x == -1 && leftarc.leftsite.y == -1) || (leftarc.rightsite.x == -1 && leftarc.rightsite.y == -1))
	{

	}
	else
	{
		if (Leftoffloat(leftarc.leftsite, T.insertedleft->value.leafsite, leftarc.rightsite) < 0)
		{
			glm::dvec3 circumcenter = CircleEventCheck(leftarc.leftsite, T.insertedleft->value.leafsite, leftarc.rightsite);
			if (circumcenter.x != -10000 && circumcenter.y != -10000)
			{
				//cout << "new event11!" << circumcenter.x << " " << circumcenter.y << " " << circumcenter.z << endl;
				fortuneevent leftevent;
				assert(T.insertedleft->value.leaf == true);
				leftevent.leafz = T.insertedleft;
				leftevent.siteevent = false;
				leftevent.site = circumcenter;
				leftevent.radius = circumcenter.z;
				Q.insert(leftevent);
				T.insertedleft->value.leafevent = Q.recentlyinsertedevent;
			}
		}
	}
}


/*FORTUNES ALGORITHM END*/

/*Start of lineintersection*/
float error = 0.1f;//0.1f;
struct segnodes {
	glm::vec2 p;
	Segment data;

	segnodes()
	{
		
	}
	segnodes(float x, float y, float x2, float y2, float px, float py)
	{
		p.x = px; p.y = py;
		data.one.x = x; data.one.y = y;
		data.two.x = x2; data.two.y = y2;
		data.DealWithUpper();
	}
	friend ostream& operator<<(ostream& os, const segnodes& e)
	{
		os << "P(" << e.p.x << ", " << e.p.y << ") , " << e.data << endl;
		return os;
	}
	bool operator==(const segnodes& e1)
	{
		if (data == e1.data)
		{
			return true;
		}
		return false;
	}
	bool operator<(const segnodes& e1)
	{
		float err = error;
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val < -err)
		{
			return false;
		}
		else if (val > err)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator>(const segnodes& e1)
	{
		float err = error;
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val > err)
		{
			return false;
		}
		else if (val < -err)
		{
			return true;
		}
		else
		{
			/*cout << "> in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return false;
			}
			else if (val2 > 0)
			{
				return true;
			}
			cout << "> error! sorting function segment and point";*/
			return false;
		}
	}
	bool operator<=(const segnodes& e1)
	{
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val < 0)
		{
			return true;
		}
		else if (val > 0)
		{
			return false;
		}
		else
		{
			cout << "in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return true;
			}
			else if (val2 > 0)
			{
				return false;
			}
			cout << "error! sorting function segment and point";
			return true;
		}
	}
	bool operator>=(const segnodes& e1)
	{
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val < 0)
		{
			return false;
		}
		else if (val > 0)
		{
			return true;
		}
		else
		{
			cout << "> in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return false;
			}
			else if (val2 > 0)
			{
				return true;
			}
			cout << "> error! sorting function segment and point";
			return true;
		}
	}
};

bool SortCSegs(segnodes a, segnodes b)
{
	float val = Leftoffloat(a.data.two, a.data.one, b.data.two);
	if (val < 0)
	{
		return true;
	}
	if (val == 0)
	{	
		cout << "Sortcsegs segment and point on same line";
		exit(0);
	}
	return false;
}

struct events {
	glm::vec2 p;
	vector<Segment*> uppersegments;

	events()
	{
		p.x = 0; p.y = 0;
	}
	events(float x, float y)
	{
		p.x = x; p.y = y;
	}
	friend ostream& operator<<(ostream& os, const events& e)
	{ 
		os << "p: (" << e.p.x << "," << e.p.y << ")";
		os << "size of uppers: " << e.uppersegments.size() << endl;
		return os;
	}
	bool operator==(const events& e1)
	{
		/*float err = .0001f;
		if (abs(p.x - e1.p.x) < err && abs(p.y - e1.p.y) < err)
		{
			return true;
		}
		return false;*/
		if (p.x == e1.p.x && p.y == e1.p.y)
		{
			return true;
		}
		return false;
	}
	bool operator<(const events& e1)
	{
		if (p.y < e1.p.y)
		{
			return true;
		}
		if (p.y == e1.p.y)
		{
			if (p.x < e1.p.x)
			{
				return true;
			}
		}
		return false;
	}
	bool operator>(const events& e1)
	{
		if (p.y > e1.p.y)
		{
			return true;
		}
		if (p.y == e1.p.y)
		{
			if (p.x > e1.p.x)
			{
				return true;
			}
		}
		return false;
	}
};

BinaryTree<events> Queue;
BinaryTreeSeg<segnodes> T;
void FindIntersections(vector<Segment> segments, vector<Point>& points)
{
	points.clear();
	if (Queue.root != NULL)
	{
		Queue.destroySubTree(Queue.root);
		Queue.root = NULL;
	}
	for (int i = 0; i < segments.size(); i++)
	{
		if (segments[i].one.y < segments[i].two.y)
		{
			events e1(segments[i].one.x, segments[i].one.y); e1.uppersegments.push_back(&segments[i]);
			events e2(segments[i].two.x, segments[i].two.y);
			Queue.insertNode(e1);
			Queue.insertNode(e2);
		}
		else if (segments[i].two.y < segments[i].one.y)
		{
			events e1(segments[i].two.x, segments[i].two.y); e1.uppersegments.push_back(&segments[i]);
			events e2(segments[i].one.x, segments[i].one.y);
			Queue.insertNode(e1);
			Queue.insertNode(e2);
		}
		else
		{
			if (segments[i].one.x < segments[i].two.x)
			{
				events e1(segments[i].one.x, segments[i].one.y); e1.uppersegments.push_back(&segments[i]);
				events e2(segments[i].two.x, segments[i].two.y);
				Queue.insertNode(e1);
				Queue.insertNode(e2);
			}
			else
			{
				events e1(segments[i].two.x, segments[i].two.y); e1.uppersegments.push_back(&segments[i]);
				events e2(segments[i].one.x, segments[i].one.y);
				Queue.insertNode(e1);
				Queue.insertNode(e2);
			}
		}
	}
	
	if (T.root != NULL)
	{
		T.destroySubTree(T.root);
		T.root = NULL;
	}

	while (Queue.numNodes() != 0)
	{
		events p = Queue.grableastvalue();
		HandleEventPoint(p,points);
		Queue.remove(p);
	}
}

/*
*/
void HandleEventPoint(events p, vector<Point>& points)
{
	vector<segnodes> U;
	vector<segnodes> L;
	vector<segnodes> C;
	for (int i = 0; i < p.uppersegments.size(); i++)
	{
		segnodes tmp(p.uppersegments[i]->one.x, p.uppersegments[i]->one.y, p.uppersegments[i]->two.x, p.uppersegments[i]->two.y,0,0);
		U.push_back(tmp);
	}
	vector<segnodes> segs = T.getlistinorder();
	for (int i = 0; i < segs.size(); i++)
	{
		float err = error;
		float val = Leftoffloat(segs[i].data.two, segs[i].data.one, p.p);
		if (-err <= val && val <= err)
		{
			if (OnSegment(segs[i].data.one, segs[i].data.two, p.p))
			{
				if (p.p == segs[i].data.two)//L
				{
					L.push_back(segs[i]);
				}
				else if (p.p == segs[i].data.one)//U
				{

				}
				else//C
				{
					C.push_back(segs[i]);
				}
			}
		}
	}
	if (L.size() + U.size() + C.size() > 1)
	{
		Point tmp(p.p.x, p.p.y,4);
		points.push_back(tmp);
	}
	for (int i = 0; i < L.size(); i++)
	{
		L[i].p = p.p;
		T.removedata(L[i]);
	}
	for (int i = 0; i < C.size(); i++)
	{
		C[i].p = p.p;
		T.removedata(C[i]);
	}

	for (int i = 0; i < C.size(); i++)
	{
		U.push_back(C[i]);
	}
	sort(U.begin(), U.end(), SortCSegs);
	for (int i = 0; i < U.size(); i++)
	{
		U[i].p = p.p;
		U[i].data.DealWithUpper();
		T.insertNode(U[i]);
	}

	if (U.empty() && C.empty())
	{
		if (!L.empty())
		{
			segnodes tmp(L[0].data.one.x, L[0].data.one.y, L[0].data.two.x, L[0].data.two.y, p.p.x, p.p.y);
			tmp.data.DealWithUpper();
			T.insertNode(tmp);
			segnodes left = T.findleftneighbor(tmp);
			segnodes right = T.findrightneighbor(tmp);
			T.removedata(tmp);
			if (right.data.one.x != -1 && left.data.one.x != -1)
				FindNewEvent(left.data, right.data, p.p);
		}
	}
	else
	{
		segnodes left = T.findleftneighbor(U[0]);
		if(left.data.one.x != -1)
			FindNewEvent(left.data, U[0].data, p.p);
		
		segnodes right = T.findrightneighbor(U[U.size() - 1]);
		if (right.data.one.x != -1)
			FindNewEvent(right.data, U[U.size() - 1].data, p.p);
	}
}

void FindNewEvent(Segment sl, Segment sr, glm::vec2 p)
{
	bool intersecting = SegmentsIntersect(sl.one, sl.two, sr.one, sr.two);
	if (intersecting)
	{
		Point p1(sl.one.x,sl.one.y);
		Point p2(sl.two.x, sl.two.y);
		Point p3(sr.one.x, sr.one.y);
		Point p4(sr.two.x, sr.two.y);
		Point intersection = LineIntersectionPoint(p1, p2, p3, p4);
		if (intersection.x != 10000)
		{
			if (intersection.y > p.y || (intersection.y == p.y && intersection.x > p.x))
			{
				events e1(intersection.x, intersection.y);
				Queue.insertNode(e1);
			}
		}
	}
}

bool SegmentsIntersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
{
	Point p11(p1.x, p1.y);
	Point p22(p2.x, p2.y);
	Point p33(p3.x, p3.y);
	Point p44(p4.x, p4.y);
	float d1 = Leftoffloat(p33, p44, p11);
	float d2 = Leftoffloat(p33, p44, p22);
	float d3 = Leftoffloat(p11, p22, p33);
	float d4 = Leftoffloat(p11, p22, p44);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
	{
		return true;
	}
	else if (d1 == 0 && OnSegment(p3, p4, p1))
	{
		return true;
	}
	else if (d2 == 0 && OnSegment(p3, p4, p2))
	{
		return true;
	}
	else if (d3 == 0 && OnSegment(p1, p2, p3))
	{
		return true;
	}
	else if (d4 == 0 && OnSegment(p1, p2, p4))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool OnSegment(glm::vec2 pi, glm::vec2 pj, glm::vec2 pk)
{
	if ((min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x)) && (min(pi.y, pj.y) <= pk.y && pk.y <= max(pi.y, pj.y)))
	{
		return true;
	}
	return false;
}
/*End of lineintersection*/

/*Start of roundline*/

bool circlecollision(glm::vec2 c1, double r1, glm::vec2 c2, double r2)
{
	double len = glm::length(c1 - c2);
	if (len < r1 + r2)
	{
		return true;
	}
	return false;
}
struct segnodesround;
bool circlesegcollision(double r, glm::vec2 c, segnodesround seg);

struct segnodesround {
	glm::vec2 p;
	double r;
	Segment data;

	segnodesround()
	{

	}
	segnodesround(float x, float y, float x2, float y2, float px, float py)
	{
		p.x = px; p.y = py;
		data.one.x = x; data.one.y = y;
		data.two.x = x2; data.two.y = y2;
		data.DealWithUpper();
	}
	friend ostream& operator<<(ostream& os, const segnodesround& e)
	{
		os << "P(" << e.p.x << ", " << e.p.y << ") , " << e.data << endl;
		return os;
	}
	bool operator==(const segnodesround& e1)
	{
		if (data == e1.data)
		{
			return true;
		}
		return false;
	}
	bool operator<(const segnodesround& e1)
	{
		if (circlesegcollision(r, p, e1))
		{
			return false;
		}
		float err = 0;
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		cout << "val: " << val << endl;
		if (val < 0)
		{
			return false;
		}
		else if (val > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator>(const segnodesround& e1)
	{
		if (circlesegcollision(r, p, e1))
		{
			return false;
		}
		float err = 0;
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		cout << "val: " << val << endl;
		if (val > 0)
		{
			return false;
		}
		else if (val < 0)
		{
			return true;
		}
		else
		{
			/*cout << "> in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return false;
			}
			else if (val2 > 0)
			{
				return true;
			}
			cout << "> error! sorting function segment and point";*/
			return false;
		}
	}
	bool operator<=(const segnodesround& e1)
	{
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val < 0)
		{
			return true;
		}
		else if (val > 0)
		{
			return false;
		}
		else
		{
			cout << "in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return true;
			}
			else if (val2 > 0)
			{
				return false;
			}
			cout << "error! sorting function segment and point";
			return true;
		}
	}
	bool operator>=(const segnodesround& e1)
	{
		float val = Leftoffloat(e1.data.two, e1.data.one, p);
		if (val < 0)
		{
			return false;
		}
		else if (val > 0)
		{
			return true;
		}
		else
		{
			cout << "> in binary tree, point is same line as segment!";
			float val2 = Leftoffloat(e1.data.two, e1.data.one, glm::vec2(p.x, p.y + 1));
			if (val2 < 0)
			{
				return false;
			}
			else if (val2 > 0)
			{
				return true;
			}
			cout << "> error! sorting function segment and point";
			return true;
		}
	}
};

bool circlesegcollision(double r, glm::vec2 c, segnodesround seg)
{
	glm::vec2 e(seg.data.one.x, seg.data.one.y);
	glm::vec2 d(seg.data.two.x - seg.data.one.x, seg.data.two.y - seg.data.one.y);
	glm::vec2 negd(-d.x, -d.y);
	glm::vec2 emc = e - c;
	double det = pow(glm::dot(d, emc), 2) - (glm::dot(d, d))*(glm::dot(emc, emc) - pow(r, 2));
	if (det < 0)
	{
		return false;
	}
	double tpos = (glm::dot(negd, emc) + sqrt(det)) / glm::dot(d, d);
	double tneg = (glm::dot(negd, emc) - sqrt(det)) / glm::dot(d, d);
	if ((tpos > 1 || tpos < 0) && (tneg > 1 || tneg < 0))
	{
		return false;
	}
	return true;
}

bool SortCSegsround(segnodesround a, segnodesround b)
{
	float val = Leftoffloat(a.data.two, a.data.one, b.data.two);
	if (val < 0)
	{
		return true;
	}
	if (val == 0)
	{
		cout << "Sortcsegs segment and point on same line";
		exit(0);
	}
	return false;
}

struct eventsround {
	glm::vec2 p;
	double r;
	vector<Segment*> uppersegments;

	eventsround()
	{
		p.x = 0; p.y = 0;
		r = 20;
	}
	eventsround(float x, float y)
	{
		p.x = x; p.y = y;
		r = 20.0;
	}
	friend ostream& operator<<(ostream& os, const eventsround& e)
	{
		os << "p: (" << e.p.x << "," << e.p.y << ")";
		os << "size of uppers: " << e.uppersegments.size() << endl;
		return os;
	}
	bool operator==(const eventsround& e1)
	{
		/*if circles colliding move it so its not colliding*/
		double len = glm::length(p - e1.p);
		if (len < r)
		{
			return true;
		}
		return false;
	}
	bool operator<(const eventsround& e1)
	{
		if (p.y < e1.p.y)
		{
			return true;
		}
		if (p.y == e1.p.y)
		{
			if (p.x < e1.p.x)
			{
				return true;
			}
		}
		return false;
	}
	bool operator>(const eventsround& e1)
	{
		if (p.y > e1.p.y)
		{
			return true;
		}
		if (p.y == e1.p.y)
		{
			if (p.x > e1.p.x)
			{
				return true;
			}
		}
		return false;
	}
};

BinaryTree<eventsround> Queueround;
BinaryTreeSeg<segnodesround> Tround;
vector<eventsround> AllCollisions;
void FindIntersectionsround(vector<Segment> segments, vector<Point>& points)
{
	//scale segments up
	AllCollisions.clear();
	points.clear();
	if (Queueround.root != NULL)
	{
		Queueround.destroySubTree(Queueround.root);
		Queueround.root = NULL;
	}
	for (int i = 0; i < segments.size(); i++)
	{
		if (segments[i].one.y < segments[i].two.y)
		{
			eventsround e1(segments[i].one.x, segments[i].one.y); e1.uppersegments.push_back(&segments[i]);
			eventsround e2(segments[i].two.x, segments[i].two.y);
			Queueround.insertNode(e1);
			Queueround.insertNode(e2);
		}
		else if (segments[i].two.y < segments[i].one.y)
		{
			eventsround e1(segments[i].two.x, segments[i].two.y); e1.uppersegments.push_back(&segments[i]);
			eventsround e2(segments[i].one.x, segments[i].one.y);
			Queueround.insertNode(e1);
			Queueround.insertNode(e2);
		}
		else
		{
			if (segments[i].one.x < segments[i].two.x)
			{
				eventsround e1(segments[i].one.x, segments[i].one.y); e1.uppersegments.push_back(&segments[i]);
				eventsround e2(segments[i].two.x, segments[i].two.y);
				Queueround.insertNode(e1);
				Queueround.insertNode(e2);
			}
			else
			{
				eventsround e1(segments[i].two.x, segments[i].two.y); e1.uppersegments.push_back(&segments[i]);
				eventsround e2(segments[i].one.x, segments[i].one.y);
				Queueround.insertNode(e1);
				Queueround.insertNode(e2);
			}
		}
	}

	if (Tround.root != NULL)
	{
		Tround.destroySubTree(Tround.root);
		Tround.root = NULL;
	}

	while (Queueround.numNodes() != 0)
	{
		eventsround p = Queueround.grableastvalue();
		HandleEventPointround(p, points, false);
		Queueround.remove(p);
	}
	//scale segments and point back down
} 

/*
*/
void HandleEventPointround(eventsround p, vector<Point>& points, bool called)
{
	vector<segnodesround> U;
	vector<segnodesround> L;
	vector<segnodesround> C;
	for (int i = 0; i < p.uppersegments.size(); i++)
	{
		segnodesround tmp(p.uppersegments[i]->one.x, p.uppersegments[i]->one.y, p.uppersegments[i]->two.x, p.uppersegments[i]->two.y, 0, 0);
		U.push_back(tmp);
	}
	vector<segnodesround> segs = Tround.getlistinorder();
	//also look at the top of queue until you find an upper that is more than radius apart
	bool leave = false;
	eventsround close;
	vector<eventsround> list = Queueround.getlistinorder();
	cout << "gotlist";
	int index = 2;
	if(list.size() > 1)
		close = list[1];
	while (close.p.y - p.p.y < close.p.r && index < list.size())
	{
		for (int i = 0; i < close.uppersegments.size(); i++)
		{
			segnodesround tmp(close.uppersegments[i]->one.x, close.uppersegments[i]->one.y, close.uppersegments[i]->two.x, close.uppersegments[i]->two.y, 0, 0);
			if (circlesegcollision(p.r, p.p, tmp))
			{
				segs.push_back(tmp);
				tmp.p = p.p; tmp.r = p.r;
				tmp.data.DealWithUpper();
				Tround.insertNode(tmp);
				close.uppersegments.erase(close.uppersegments.begin() + i);
				i--;
			}
		}
		if (close.uppersegments.size() == 0)
		{
			Queueround.remove(close);
		}
		close = list[index];
		index++;
	}
	for (int i = 0; i < segs.size(); i++)//test if segment is colliding with circle
	{
		if (circlesegcollision(p.r,p.p,segs[i]))
		{
			//circlecollision(p.p,p.r,segs[i].data.two,1)
			if (p.p == segs[i].data.two)//L
			{
				L.push_back(segs[i]);
			}
			else if (p.p == segs[i].data.one)//U
			{

			}
			else//C
			{
				C.push_back(segs[i]);
			}
		}
	}
	if ((L.size() + U.size() + C.size() > 1) && called == false)
	{
		Point tmp(p.p.x, p.p.y, 4);
		points.push_back(tmp);
		eventsround tmpz(tmp.x, tmp.y);
		AllCollisions.push_back(tmpz);
	}
	cout << "B4 L";

	for (int i = 0; i < L.size(); i++)
	{
		L[i].p = p.p; L[i].r = p.r;
		cout << L[i];
		Tround.removedata(L[i]);
	}
	cout << "C";
	//Tround.displayInOrder();
	for (int i = 0; i < C.size(); i++)
	{
		C[i].p = p.p; C[i].r = p.r;
		cout << C[i];
		Tround.removedata(C[i]);
		//Tround.displayInOrder();
	}

	for (int i = 0; i < C.size(); i++)
	{
		U.push_back(C[i]);
	}
	sort(U.begin(), U.end(), SortCSegsround);
	for (int i = 0; i < U.size(); i++)
	{
		U[i].p = p.p; U[i].r = p.r;
		U[i].data.DealWithUpper();
		Tround.insertNode(U[i]);
	}
	cout << "AFTER ";
	//Tround.displayInOrder();
	if (U.empty() && C.empty())
	{
		if (!L.empty())
		{
			segnodesround tmp(L[0].data.one.x, L[0].data.one.y, L[0].data.two.x, L[0].data.two.y, p.p.x, p.p.y);
			tmp.data.DealWithUpper();
			Tround.insertNode(tmp);
			segnodesround left = Tround.findleftneighbor(tmp);
			segnodesround right = Tround.findrightneighbor(tmp);
			Tround.removedata(tmp);
			if (right.data.one.x != -1 && left.data.one.x != -1)
				FindNewEventround(left.data, right.data, p.p, p, points, called);
		}
	}
	else
	{
		segnodesround left = Tround.findleftneighbor(U[0]);
		if (left.data.one.x != -1)
			FindNewEventround(left.data, U[0].data, p.p, p, points, called);

		segnodesround right = Tround.findrightneighbor(U[U.size() - 1]);
		if (right.data.one.x != -1)
			FindNewEventround(right.data, U[U.size() - 1].data, p.p, p, points, called);
	}
}

void FindNewEventround(Segment sl, Segment sr, glm::vec2 p, eventsround P, vector<Point>& points, bool called)
{
	bool intersecting = SegmentsIntersect(sl.one, sl.two, sr.one, sr.two);
	if (intersecting)
	{
		Point p1(sl.one.x, sl.one.y);
		Point p2(sl.two.x, sl.two.y);
		Point p3(sr.one.x, sr.one.y);
		Point p4(sr.two.x, sr.two.y);
		Point intersection = LineIntersectionPoint(p1, p2, p3, p4);
		if (intersection.x != 10000)
		{
			if (intersection.y > p.y || (intersection.y == p.y && intersection.x > p.x))
			{
				eventsround e1(intersection.x, intersection.y);
				Queueround.insertNode(e1);
				/*int index = -1;
				for (int i = 0; i < AllCollisions.size(); i++)
				{
					if (e1 == AllCollisions[i])
					{
						index = i;
						break;
					}
				}
				if (index == -1)
				{
					Queueround.insertNode(e1);
				}
				else
				{
					if (called == false && P.uppersegments.size() > 0)
					{
						HandleEventPointround(AllCollisions[index], points, true);
					}
				}*/
			}
		}
	}
}




/*End of roundline*/
bool IsPointInHull(Polygon& convexhull, Point p)
{
	//get direction
	Point d(100, 400);
	Point pp;
	pp.x = p.x + d.x;
	pp.y = p.y + d.y;
	int intersections = 0;
	for (int i = 0; i < convexhull.points.size(); i++)
	{
		Point intersection = LineIntersectionPoint(convexhull.points[i], convexhull.points[(i + 1) % convexhull.points.size()], p, pp);
		if (intersection.x == 0 && intersection.y == 0)
		{
			cout << "parralel" << endl;
		}
		if (IsPointOnSegment(convexhull.points[i], convexhull.points[(i + 1) % convexhull.points.size()], intersection)
			&& IsPointOnSegment(p, pp, intersection))
		{
			intersections++;
		}
	}
	//cout << "intersections: " << intersections << endl;
	if (intersections % 2 == 0)
	{
		return false;
	}
	return true;
}

bool IsPointOnSegment(Point p1, Point p2, Point p)
{
	float epsilon = 0.2f;
	float value = pointmagnitude(pointsubract(p1, p)) + pointmagnitude(pointsubract(p2, p)) - pointmagnitude(pointsubract(p1, p2));
	if ((-epsilon < value) && (value < epsilon))
	{
		return true;
	}
	return false;
}

Point LineIntersectionPoint(Point p1, Point p2, Point p3, Point p4)
{
	//p1 + t1d1 = p3 + t2d2
	//t1d1 - t2d2 = p3 - p1
	//Ax = b
	//x = A^-1*b
	glm::dvec2 d1 = glm::dvec2(p2.x, p2.y) - glm::dvec2(p1.x, p1.y);
	glm::dvec2 d2 = glm::dvec2(p4.x, p4.y) - glm::dvec2(p3.x, p3.y);
	if(p2.x != p1.x || p2.y != p1.y)
	{
		//d1 = glm::normalize(d1);
	}
	//glm::vec2 d1 = glm::normalize(glm::vec2(p2.x, p2.y) - glm::vec2(p1.x, p1.y));
	//glm::vec2 d2 = glm::normalize(glm::vec2(p4.x, p4.y) - glm::vec2(p3.x, p3.y));
	glm::dmat2x2 A;
	A[0][0] = d1.x; A[1][0] = d2.x;
	A[0][1] = d1.y; A[1][1] = d2.y;
	glm::dvec2 b = glm::dvec2(p3.x - p1.x, p3.y - p1.y);
	if (abs(glm::determinant(A)) <= 0.01f)
	{
		cout << "parralel lines" << endl;
		Point tmp(-10000, -10000);
		return tmp;
	}
	glm::dvec2 x = glm::inverse(A) * b;
	//cout << "inverse: " << x.x << "det: "<<glm::determinant(A)<< endl;
	//cout << p2.x<<" "<<p1.x <<" "<<p2.y<<" "<<p1.y<< endl;
	glm::dvec2 intersection = glm::dvec2(p1.x,p1.y) + x.x*d1;
	//cout << intersection.x << " " << intersection.y << endl;
	Point tmp(intersection.x, intersection.y);
	return tmp;
}
//*for convex, in voronoimesh maintain a convex which just needs a function that calculates a new convex when one point is added
//then if a point is outside convex just find 1 or 2 intersections within that region, add new edges and references and your done
//if you have one just create that line and move on, but if you have one again you know it has to be end 
//so create that line and leave
//if they are same point make sure not to copy
//to determine what direction to go just have to see if the line intersects with any other lines with a little error in that direction
//the one that doesn't intersect wins
//**if you hav one intersetion that site is convex get the consicutive convex points, now test conescutive 2 with farpoint it should be different
//*for convex, in voronoimesh maintain a convex which just needs a function that calculates a new convex when one point is added
//then if a point is outside convex just find 1 or 2 intersections within that region, add new edges and references and your done
//*for convex, in voronoimesh maintain a convex which just needs a function that calculates a new convex when one point is added
//then if a point is outside convex just find 1 or 2 intersections within that region, add new edges and references and your done
VoronoiMesh VoronoiIncremental(Polygon& pointset)
{
	VoronoiMesh vmesh;
	if (pointset.points.size() < 3)
		return vmesh;
	Point p0, p1, p2;
	p0 = pointset.points[0];
	p1 = pointset.points[1];
	p2 = pointset.points[2];
	if (Leftof(p0, p1, p2) > 0)
	{
		//cout << "switched!" << endl;
		p0 = pointset.points[0];
		p1 = pointset.points[2];
		p2 = pointset.points[1];
	}
	/*create vmesh with first 3 points*/
	Point b01, f01, b02, f02, b12, f12;
	/*first bisector*/
	b01.x = (p0.x + p1.x) / 2.0f;
	b01.y = (p0.y + p1.y) / 2.0f;
	float ydiff = p0.y - p1.y;
	float xdiff = p0.x - p1.x;
	glm::vec2 r = glm::normalize(glm::vec2(1, (-xdiff / ydiff)));//direction doestn matter just finding intersection
	f01.x = b01.x + r.x;
	f01.y = b01.y + r.y;
	/*2nd bisector*/
	b02.x = (p0.x + p2.x) / 2.0f;
	b02.y = (p0.y + p2.y) / 2.0f;
	ydiff = p0.y - p2.y;
	xdiff = p0.x - p2.x;
	r = glm::normalize(glm::vec2(1, (-xdiff / ydiff)));
	f02.x = b02.x + r.x;
	f02.y = b02.y + r.y;
	/*bisector 3*/
	b12.x = (p1.x + p2.x) / 2.0f;
	b12.y = (p1.y + p2.y) / 2.0f;
	ydiff = p1.y - p2.y;
	xdiff = p1.x - p2.x;
	r = glm::normalize(glm::vec2(1, (-xdiff / ydiff)));
	f12.x = b12.x + r.x;
	f12.y = b12.y + r.y;
	/*intersection point*/
	Point p = LineIntersectionPoint(b01, f01, b02, f02);
	/*edges*/
	float value = 500000;
	Point dir;
	glm::vec2 tmp(b01.x - p.x, b01.y - p.y); tmp = glm::normalize(tmp); dir.x = tmp.x; dir.y = tmp.y;
	Point p01 = pointadd(p, pointmultiplyconstant(dir, value));
	if (Leftofvalue(p0, p1, p01) < 0)
	{
		tmp.x = p.x - b01.x; tmp.y = p.y - b01.y; tmp = glm::normalize(tmp); dir.x = tmp.x; dir.y = tmp.y;
		p01 = pointadd(p, pointmultiplyconstant(dir, value));
	}
	glm::vec2 tmp2(b02.x - p.x, b02.y - p.y); tmp2 = glm::normalize(tmp2); dir.x = tmp2.x; dir.y = tmp2.y;
	Point p02 = pointadd(p, pointmultiplyconstant(dir, value));
	if (Leftofvalue(p2, p0, p02) < 0)
	{
		tmp2.x = p.x - b02.x; tmp2.y = p.y - b02.y; tmp2 = glm::normalize(tmp2); dir.x = tmp2.x; dir.y = tmp2.y;
		p02 = pointadd(p, pointmultiplyconstant(dir, value));
	}
	glm::vec2 tmp3(b12.x - p.x, b12.y - p.y); tmp3 = glm::normalize(tmp3); dir.x = tmp3.x; dir.y = tmp3.y;
	Point p12 = pointadd(p, pointmultiplyconstant(dir, value));
	if (Leftofvalue(p1, p2, p12) < 0)
	{
		tmp3.x = p.x - b12.x; tmp3.y = p.y - b12.y; tmp3 = glm::normalize(tmp3); dir.x = tmp3.x; dir.y = tmp3.y;
		p12 = pointadd(p, pointmultiplyconstant(dir, value));
	}
	vmesh.Addvpoints(p.x, p.y);
	vmesh.Addvpoints(p01.x, p01.y);
	vmesh.Addvpoints(p02.x, p02.y);
	vmesh.Addvpoints(p12.x, p12.y);
	vmesh.Addvedge(0, 1, 0, 1);
	vmesh.Addvedge(0, 2, 0, 2);
	vmesh.Addvedge(0, 3, 1, 2);
	vmesh.Addsite(p0); vmesh.SiteAddEdge(0, 0); vmesh.SiteAddEdge(0, 1);
	vmesh.Addsite(p1); vmesh.SiteAddEdge(1, 0); vmesh.SiteAddEdge(1, 2);
	vmesh.Addsite(p2); vmesh.SiteAddEdge(2, 1); vmesh.SiteAddEdge(2, 2);
	vmesh.PrintInfo();
	/*initialize convex hull*/
	vmesh.convexhull.points.push_back(p0);
	vmesh.convexhull.points.push_back(p1);
	vmesh.convexhull.points.push_back(p2);
	bool outsideconvex = false;
	/*incremental algorithm now*/
	for (int i = 3; i < pointset.points.size(); i++)
	{
		outsideconvex = false;
		if (!IsPointInHull(vmesh.convexhull, pointset.points[i]))
		{
			vmesh.convexhull.points.push_back(pointset.points[i]);
			vmesh.convexhull = ConvexHullGrahamScan(vmesh.convexhull);
			outsideconvex = true;
		}
		//cout << "new increment" << endl;
		//figure out what site point is in
		int siteindex = vmesh.ClosestSite(pointset.points[i]);
		int originalsiteindex = siteindex;
		vector<int> oppositeneighbors;
		vector<int> bisectorneighbors;
		vector<int> pointindexdelete;
		vector<int> edgesaves;
		set<int> pointindexdeleteset;
		int vpointstartingindex = vmesh.vpoints.size();
		bool leave = false;
		while (!leave)
		{
			//cout << "site index: " << siteindex << endl;
			//create the perpendicular bisector between point and site
			Point bisector;
			Point perpendicular;
			bisector.x = (pointset.points[i].x + vmesh.sites[siteindex].pos.x) / 2.0f;
			bisector.y = (pointset.points[i].y + vmesh.sites[siteindex].pos.y) / 2.0f;
			float ydiff = pointset.points[i].y - vmesh.sites[siteindex].pos.y;
			float xdiff = pointset.points[i].x - vmesh.sites[siteindex].pos.x;
			glm::vec2 r;
			if (ydiff == 0)
			{
				r = glm::normalize(glm::vec2(0, 4));//direction doestn matter just finding intersection
			}
			else
			{
				r = glm::normalize(glm::vec2(4, 4 * (-xdiff / ydiff)));//direction doestn matter just finding intersection
			}
			perpendicular.x = bisector.x + r.x;
			perpendicular.y = bisector.y + r.y;
			Point intersection;
			int intersectioncount = 0;
			vector<Point> collisions;
			vector<int> collisionedges;
			for (int j = 0; j < vmesh.sites[siteindex].edgeindices.size(); j++)
			{
				/*get intersection point*/
				intersection = LineIntersectionPoint(bisector, perpendicular, vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p1index],
					vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p2index]);
				//cout << intersection.x << " " << intersection.y << endl;
				/*is intersection on segment*/
				if (IsPointOnSegment(vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p1index], vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p2index],
					intersection))
				{
					collisions.push_back(intersection);
					collisionedges.push_back(j);
					intersectioncount++;
				}
			}
			if (intersectioncount != 2 && outsideconvex == false)
			{
				//cout << "ERROR " << intersectioncount << " intersections!" << endl;
				return vmesh;
			}
			else if (outsideconvex == true)
			{
				leave = true;
				int counter = 0;
				vmesh.Addsite(pointset.points[i]);
				for (int x = 0; x < collisions.size(); x++)
				{
					/*find neighbor to edge*/
					int othersiteindex;
					if (vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].s1index == siteindex)
						othersiteindex = vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].s2index;
					else if (vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].s2index == siteindex)
						othersiteindex = vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].s1index;
					else
						int a;
						//cout << "ERROR sites doesn't contain correct one outside convex" << endl;
					/*add point*/
					vmesh.Addvpoints(collisions[x].x, collisions[x].y);
					/*add edges*/
					if (pointmagnitude(pointsubract(pointset.points[i], vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p1index]))
						< pointmagnitude(pointsubract(vmesh.sites[siteindex].pos, vmesh.vpoints[vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p1index])))
					{
						vmesh.SiteAddEdge(othersiteindex, vmesh.vedges.size());
						vmesh.SiteAddEdge(othersiteindex, vmesh.vedges.size() + 1);
						vmesh.SiteAddEdge(siteindex, vmesh.vedges.size() + 1);
						vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
						vmesh.Addvedge(vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p1index, vmesh.vpoints.size() - 1, vmesh.sites.size() - 1, othersiteindex);
						vmesh.Addvedge(vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p2index, vmesh.vpoints.size() - 1, siteindex, othersiteindex);
					}
					else
					{
						vmesh.SiteAddEdge(othersiteindex, vmesh.vedges.size());
						vmesh.SiteAddEdge(othersiteindex, vmesh.vedges.size() + 1);
						vmesh.SiteAddEdge(siteindex, vmesh.vedges.size() + 1);
						vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
						vmesh.Addvedge(vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p2index, vmesh.vpoints.size() - 1, vmesh.sites.size() - 1, othersiteindex);
						vmesh.Addvedge(vmesh.vedges[vmesh.sites[siteindex].edgeindices[collisionedges[counter]]].p1index, vmesh.vpoints.size() - 1, siteindex, othersiteindex);
					}
					counter++;
				}
				if (intersectioncount == 2)
				{
					/*remove edge*/
					vmesh.Removevedge(vmesh.sites[siteindex].edgeindices[collisionedges[0]]);//**look at function
					vmesh.Removevedge(vmesh.sites[siteindex].edgeindices[collisionedges[1] - 1]);
					/*connect edge and add reference*/
					vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
					vmesh.SiteAddEdge(siteindex, vmesh.vedges.size());
					vmesh.Addvedge(vmesh.vpoints.size() - 2, vmesh.vpoints.size() - 1, siteindex, vmesh.sites.size() - 1);
				}
				else if (intersectioncount == 1)
				{
					/*remove edge*/
					vmesh.Removevedge(vmesh.sites[siteindex].edgeindices[collisionedges[0]]);
					/*get direction of bisector with correct orientation and add far away point*/
					value = 500000;
					Point farpoint1(collisions[0].x + value * r.x, collisions[0].y + value * r.y);
					Point farpoint2(collisions[0].x - value * r.x, collisions[0].y - value * r.y);
					float v1 = Leftofvalue(vmesh.sites[siteindex].pos, vmesh.sites[vmesh.sites.size() - 1].pos, farpoint1);
					float v2 = Leftofvalue(vmesh.sites[siteindex].pos, vmesh.sites[vmesh.sites.size() - 1].pos, collisions[0]);
					if ((v1 > 0 && v2 > 0) || (v1 < 0 && v2 < 0))//both same switch directions
					{
						//use farpoint 2
						vmesh.Addvpoints(farpoint2.x, farpoint2.y);
						vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
						vmesh.SiteAddEdge(siteindex, vmesh.vedges.size());
						vmesh.Addvedge(vmesh.vpoints.size() - 1, vmesh.vpoints.size() - 2, siteindex, vmesh.sites.size() - 1);
					}
					else
					{
						vmesh.Addvpoints(farpoint1.x, farpoint1.y);
						vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
						vmesh.SiteAddEdge(siteindex, vmesh.vedges.size());
						vmesh.Addvedge(vmesh.vpoints.size() - 1, vmesh.vpoints.size() - 2, siteindex, vmesh.sites.size() - 1);
					}
				}
				else
				{
					//cout << "ERROR intersection count outside convex" << endl;
				}
			}
			else
			{
				//see which one is ccw
				int edgecollider;
				if (Leftofvalue(pointset.points[i], collisions[0], collisions[1]) < 0)
				{
					intersection = collisions[0];
					edgecollider = collisionedges[0];
				}
				else
				{
					intersection = collisions[1];
					edgecollider = collisionedges[1];
				}
				//save edge
				//get next neighbor
				/*add point and the 2 edges and get its neighbors based on what point is closer to the new point*/
				int othersiteindex;
				if (vmesh.vedges[vmesh.sites[siteindex].edgeindices[edgecollider]].s1index == siteindex)
					othersiteindex = vmesh.vedges[vmesh.sites[siteindex].edgeindices[edgecollider]].s2index;
				else if (vmesh.vedges[vmesh.sites[siteindex].edgeindices[edgecollider]].s2index == siteindex)
					othersiteindex = vmesh.vedges[vmesh.sites[siteindex].edgeindices[edgecollider]].s1index;
				else
					int a;
					//cout << "ERROR sites doesn't contain correct one" << endl;

				vmesh.Addvpoints(intersection.x, intersection.y);
				edgesaves.push_back(vmesh.sites[siteindex].edgeindices[edgecollider]);
				bisectorneighbors.push_back(siteindex);
				oppositeneighbors.push_back(othersiteindex);
				siteindex = othersiteindex;
			}
			if (siteindex == originalsiteindex)
			{
				leave = true;
			}
		}
		if (outsideconvex == true)
		{

		}
		else
		{
			int counter = 0;
			/*connect all the new points we added by adding edges*/
			vmesh.Addsite(pointset.points[i]);
			//Polygon convexhull;
			for (int z = vpointstartingindex; z < vmesh.vpoints.size(); z++)
			{
				if (z + 1 == vmesh.vpoints.size())
				{
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size());
					vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
					vmesh.Addvedge(z, vpointstartingindex, vmesh.sites.size() - 1, oppositeneighbors[counter]);
					counter++;
				}
				else
				{
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size());
					vmesh.SiteAddEdge(vmesh.sites.size() - 1, vmesh.vedges.size());
					vmesh.Addvedge(z, z + 1, vmesh.sites.size() - 1, oppositeneighbors[counter]);
					counter++;
				}
				//convexhull.points.push_back(vmesh.vpoints[z]);
			}

			/*add all the new double edges from new points*/
			counter = 0;
			for (int z = vpointstartingindex; z < vmesh.vpoints.size(); z++)
			{
				//if its inside the convexhull
				//IsPointInHull(convexhull, vmesh.vpoints[vmesh.vedges[edgesaves[counter]].p1index])
				//IsPointInHull(convexhull, vmesh.vpoints[vmesh.vedges[edgesaves[counter]].p2index])
				if (pointmagnitude(pointsubract(pointset.points[i], vmesh.vpoints[vmesh.vedges[edgesaves[counter]].p1index]))
					< pointmagnitude(pointsubract(vmesh.sites[bisectorneighbors[counter]].pos, vmesh.vpoints[vmesh.vedges[edgesaves[counter]].p1index])))
				{
					//p1index is closest to new site
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size());
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.SiteAddEdge(bisectorneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p1index, z, vmesh.sites.size() - 1, oppositeneighbors[counter]);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p2index, z, oppositeneighbors[counter], bisectorneighbors[counter]);
					//pointindexdelete.push_back(vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p1index);
					pointindexdeleteset.insert(vmesh.vedges[edgesaves[counter]].p1index);
				}
				else
				{
					//p1index is farthest to new site
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size());
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.SiteAddEdge(bisectorneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p2index, z, vmesh.sites.size() - 1, oppositeneighbors[counter]);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p1index, z, oppositeneighbors[counter], bisectorneighbors[counter]);
					//pointindexdelete.push_back(vmesh.vedges[vmesh.sites[siteindex].edgeindices[j]].p2index);
					pointindexdeleteset.insert(vmesh.vedges[edgesaves[counter]].p2index);
				}
				//else
				//{
					/*vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size());
					vmesh.SiteAddEdge(oppositeneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.SiteAddEdge(bisectorneighbors[counter], vmesh.vedges.size() + 1);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p2index, z, oppositeneighbors[counter], bisectorneighbors[counter]);
					vmesh.Addvedge(vmesh.vedges[edgesaves[counter]].p1index, z, oppositeneighbors[counter], bisectorneighbors[counter]);*/
					//cout << "ERROR no points are inside region" << endl;
				//}
				counter++;
			}
			/*ERRORS when colliding with 3, it hits 2 edges that are joining and counts them as two
			When it goes to infinity seems to not count it
			*/

			/*we made a set for unique points now copy in vector because we need mutable structure*/
			pointindexdelete.clear();
			std::set<int>::iterator it;
			for (int z = 0; z < pointindexdeleteset.size(); z++)
			{
				it = pointindexdeleteset.begin(); std::advance(it, z);
				pointindexdelete.push_back(*it);
			}
			/*delete all points that were closer to our new point*/
			for (int z = 0; z < pointindexdelete.size(); z++)
			{
				vmesh.Removevpoints(pointindexdelete[z]);
				for (int c = z + 1; c < pointindexdelete.size(); c++)
				{
					if (pointindexdelete[c] > pointindexdelete[z])
					{
						pointindexdelete[c]--;
					}
				}
			}
			//vmesh.PrintInfo();
			//cout << endl;
		}
	}
	vmesh.CheckStatus();
	return vmesh;
}

bool collinear(Point& p1, Point& p2, Point& p3)
{
	//get angle of vectors p1-p2, p3-p2
	Point one = pointsubract(p1, p2);
	Point two = pointsubract(p3, p2);
	float angle = acos(dotproduct(one, two) / (pointmagnitude(one)*pointmagnitude(two)));
	//cout << angle << endl;
	if (angle > 3.05f)//3.05
	{
		return true;
	}
	return false;
}

int IsPointInTriangle(Point t1, Point t2, Point t3, Point a)
{
	//get barycentric coordinates and see if b and y are positive and less than 1
	float B, Y;

	float ynum = (t1.y - t2.y)*a.x + (t2.x - t1.x)*a.y + t1.x*t2.y - t2.x*t1.y;
	float ydenom = (t1.y - t2.y)*t3.x + (t2.x - t1.x)*t3.y + t1.x*t2.y - t2.x*t1.y;
	if (ydenom != 0)
	{
		Y = ynum / ydenom;
	}
	else
	{
		//cout << "ydenom is zero area very small" << endl;
		return -1;
	}

	float bnum = (t1.y - t3.y)*a.x + (t3.x - t1.x)*a.y + t1.x*t3.y - t3.x*t1.y;
	float bdenom = (t1.y - t3.y)*t2.x + (t3.x - t1.x)*t2.y + t1.x*t3.y - t3.x*t1.y;
	if (bdenom != 0)
	{
		B = bnum / bdenom;
	}
	else
	{
		//cout << "bdenom is zero area very small" << endl;
		return -1;
	}

	if (B > 0 && Y > 0 && B + Y <= 1.03f)//1.03
	{
		return 1;
	}
	return 0;
}
bool IsPointInTriangleError(Point t1, Point t2, Point t3, Point a)
{
	if (IsPointInTriangle(t1,t2,t3,a) == 1)
		return true;
	if (IsPointInTriangle(t2, t3, t1, a) == 1)
		return true;
	if (IsPointInTriangle(t3, t1, t2, a) == 1)
		return true;
	return false;
}
/*
algorithm finds convexhull, triangulates it. Then goes through the pointset finds all the points not already in the triangulated mesh.
Creates 3 triangles around the triangle its contained in and repeats.
To deal with colinear point I test if the angle formed by the line is greater than a little less than pi, then if its colinear it doesnt form
a triangle which would be very small. Also if its colinear it checks the other triangle its in by creating a little error in the in triangle function
which tests for that triangle as well. There are two minor errors to help it not be exact and look nicer
this seems to work very well, it can look wierd but thats only when the angles are super small and the lines intersect
complexity is a little longer its around nlogn + n^2 + n^2 + nlogn + 2n, mainly because it has to use two other functions for hull and triangulation
*/
TriangleMesh TriangulationSplitting(Polygon& pointset)
{
	TriangleMesh tri;
	Polygon convexhull = ConvexHullGiftWrap(pointset);
	tri = TriangulationIncremental(convexhull);
	//shuffle points for random triangulations
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(pointset.points.begin(), pointset.points.end(), std::default_random_engine(seed));

	for (int i = 0; i < pointset.points.size(); i++)
	{
		if (tri.FindPointIndex(pointset.points[i].x, pointset.points[i].y) == -1)//if point isnt already in trianglemesh
		{
			vector<glm::vec3> futuretriangles;
			vector<int> triangledelete;
			for (int t = 0; t < tri.tindex.size(); t++)
			{
				if (IsPointInTriangleError(tri.vertices[tri.tindex[t][0]], tri.vertices[tri.tindex[t][1]], tri.vertices[tri.tindex[t][2]], pointset.points[i]) == 1)
				{
					tri.AddVertices(pointset.points[i].x, pointset.points[i].y);
					int lastindex = tri.vertices.size() - 1;
					bool shouldwebreak = true;
					if (!collinear(tri.vertices[tri.tindex[t][0]], pointset.points[i], tri.vertices[tri.tindex[t][1]]))
					{
						futuretriangles.emplace_back(tri.tindex[t][0], tri.tindex[t][1], lastindex);
					}
					else
						shouldwebreak = false;
					if (!collinear(tri.vertices[tri.tindex[t][1]], pointset.points[i], tri.vertices[tri.tindex[t][2]]))
					{
						futuretriangles.emplace_back(tri.tindex[t][1], tri.tindex[t][2], lastindex);
					}
					else
						shouldwebreak = false;
					if (!collinear(tri.vertices[tri.tindex[t][2]], pointset.points[i], tri.vertices[tri.tindex[t][0]]))
					{
						futuretriangles.emplace_back(tri.tindex[t][2], tri.tindex[t][0], lastindex);
					}
					else
						shouldwebreak = false;					
					triangledelete.push_back(t);	
					/*if(shouldwebreak)
						break;*/
				}
			}
			for (int z = 0; z < triangledelete.size(); z++)
			{
				tri.tindex.erase(tri.tindex.begin() + triangledelete[z] - z);
			}
			for (int z = 0; z < futuretriangles.size(); z++)
			{
				tri.Addtriangle(futuretriangles[z][0], futuretriangles[z][1], futuretriangles[z][2]);
			}
		}
	}

	return tri;
}
/*
algorithm is pretty much incremental algorithm for convex hull because I need the convex hull in order to find visible points for the new triangles.
Fixed an issue with collinear points. 
It seems to work pretty well except if there are more than 3 collinear points right off the beggining then those points won't show, its an easy fix
just annoying.
The triangulations look a little ugly because they are so tall.
kinda slow complexity is worst case n^2 + n^2 + nlgn
*/
TriangleMesh TriangulationIncremental(Polygon& pointset)
{
	TriangleMesh tri;
	Polygon convexhull;
	list<Point> convexlist;
	list<Point>::iterator it;
	list<Point>::iterator it2;
	list<Point>::iterator it3;
	/*take care of special cases where n<=3*/
	if (pointset.points.size() <= 3)
	{
		for (int i = 0; i < pointset.points.size(); i++)
		{
			tri.AddVertices(pointset.points[i].x, pointset.points[i].y);
		}
		if (pointset.points.size() == 3)
			tri.Addtriangle(0, 1, 2);
		return tri;
	}

	sort(pointset.points.begin(), pointset.points.end(), comparepoints);
	for (int i = 0; i < 3; i++)
	{
		Point tmp(pointset.points[i].x, pointset.points[i].y);
		convexhull.points.push_back(tmp);
		tri.AddVertices(pointset.points[i].x, pointset.points[i].y);
	}
	tri.Addtriangle(0, 1, 2);

	int index = 3;
	/*goes until the first 3 points are not collinear*/
	while(convexhull.points[0].x == convexhull.points[1].x && convexhull.points[0].x == convexhull.points[2].x)
	{
		//cout << "dealt with first 3 being colinear" << endl;
		if (index > pointset.points.size())
		{
			cout << "error all colinear"; return tri;
		}
		int top;
		int bottom;
		if ((convexhull.points[0].y > convexhull.points[1].y && convexhull.points[0].y > convexhull.points[2].y))
			top = 0;
		else if ((convexhull.points[1].y > convexhull.points[0].y && convexhull.points[1].y > convexhull.points[2].y))
			top = 1;
		else
			top = 2;
		if (convexhull.points[0].y <= convexhull.points[1].y && convexhull.points[0].y <= convexhull.points[2].y)
			bottom = 0;
		else if (convexhull.points[1].y < convexhull.points[0].y && convexhull.points[1].y < convexhull.points[2].y)
			bottom = 1;
		else
			bottom = 2;
		Point save = convexhull.points[top];
		convexhull.points[0] = convexhull.points[bottom];
		convexhull.points[1] = save;
		convexhull.points[2] = pointset.points[index];
		index++;
	}
	for (int i = 0; i < 3; i++)
	{
		convexlist.emplace_back(convexhull.points[i].x, convexhull.points[i].y);
		//cout << convexhull.points[i].x << " " << convexhull.points[i].y << endl;
	}
	convexhull.points.clear();

	for (int i = index; i < pointset.points.size(); i++)
	{
		bool visible = false;
		bool switched = false;
		int index1 = -1;
		int index2 = -1;
		//cout << convexlist.size() << endl;
		for (int j = 0; j < convexlist.size(); j++)
		{
			it = convexlist.begin(); std::advance(it, (j + 1) % convexlist.size());
			it2 = convexlist.begin(); std::advance(it2, j);
			it3 = convexlist.begin(); std::advance(it3, (j + convexlist.size() - 1) % convexlist.size());

			int c = Leftofvalue(*it,*it2,*it3);
			int p = Leftofvalue(*it,*it2, pointset.points[i]);

			if ((c < 0 && p < 0) || (c > 0 && p > 0))//invisible
			{
				if (switched == true)
				{
					index2 = j;
					/*create trianglemesh*/
					tri.AddVertices(pointset.points[i].x, pointset.points[i].y);
					it = convexlist.begin(); std::advance(it, index1);
					tri.AddVertices(it->x, it->y);
					for (int x = index1; x <= index2 - 1; x++)
					{
						it = convexlist.begin(); std::advance(it, x);
						it2 = convexlist.begin(); std::advance(it2, x + 1);
						tri.AddVertices(it2->x, it2->y);
						tri.Addtriangle(*it, *it2, pointset.points[i]);
					}
					/*create the hull*/
					it = convexlist.begin(); std::advance(it, index1 + 1);
					it2 = convexlist.begin(); std::advance(it2, index2);
					convexlist.erase(it, it2);
					it = convexlist.begin(); std::advance(it, index1 + 1);
					convexlist.insert(it, pointset.points[i]);

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

			/*shouldn't reach here end catch error means its edge 0 and 1*/
			if ((j == convexlist.size() - 1))
			{
				index2 = j;
				/*create trianglemesh*/
				tri.AddVertices(pointset.points[i].x, pointset.points[i].y);
				it = convexlist.begin(); std::advance(it, index1);
				tri.AddVertices(it->x, it->y);
				for (int x = index1; x <= index2; x++)
				{
					it = convexlist.begin(); std::advance(it, x);
					it2 = convexlist.begin(); std::advance(it2, (x + 1) % convexlist.size());
					tri.AddVertices(it2->x, it2->y);
					tri.Addtriangle(*it, *it2, pointset.points[i]);
				}
				/*create the hull*/
				it = convexlist.begin(); std::advance(it, index1 + 1);
				it2 = convexlist.end();
				convexlist.erase(it, it2);
				it = convexlist.begin(); std::advance(it, index1 + 1);
				convexlist.insert(it, pointset.points[i]);
			}
		}
	}

	return tri;
}

Polygon ConvexHullDivideandConqureStart(Polygon& pointset)
{
	Polygon convexhull;
	convexhull.points.reserve(pointset.points.size() - 1);

	sort(pointset.points.begin(), pointset.points.end(), comparepoints);
	ConvexHullDivideandConquer(convexhull, pointset, 0, pointset.points.size() - 1);

	return convexhull;
}

void ConvexHullDivideandConquer(Polygon& convexhull, Polygon& pointset, int b, int e)
{
	//base case
	if ((e - b) + 1 <= 5)
	{
		Polygon tmp;
		for (int i = b; i <= e; i++)
		{
			tmp.points.push_back(pointset.points[i]);
			//convexhull.points.emplace_back(pointset.points[i].x, pointset.points[i].y);
		}
		Polygon tmpconvex = ConvexHullGrahamScan(tmp);
		for (int i = 0; i < tmpconvex.points.size(); i++)
		{
			convexhull.points.push_back(tmpconvex.points[i]);
		}
		
		return;
	}

	//split up
	if (b == e)
		cout << "b == e error...";
	int m = (b + e) / 2;
	ConvexHullDivideandConquer(convexhull, pointset, b, m);
	ConvexHullDivideandConquer(convexhull, pointset, m + 1, e);
	
	//merge
	ConvexHullDivideandConqureMerg(convexhull, pointset, b, m, e);
}

void ConvexHullDivideandConqureMerg(Polygon& convexhull, Polygon& pointset, int b, int m, int e)
{
	cout << "sup";
	int bindex = m + 1;
	int aindex = b;
	for (int i = b; i <= m; i++)
	{
		if (pointset.points[i].x > pointset.points[aindex].x)
		{
			aindex = i;
		}
	}
	int ia = aindex;
	int ib = bindex;
	int sizea = m - b + 1;
	int sizeb = e - m;
	//lower hull
	bool leave = false;
	cout << b << " " << m << " " << e << endl;
	while (!leave)
	{
		cout << "one";
		leave = 1;
		while (Leftof(pointset.points[aindex], pointset.points[bindex], pointset.points[((bindex - (m+1) + 1) % (sizeb)) + m + 1]) >= 0)
		{
			cout << bindex << endl;
			bindex = ((bindex - (m + 1) + 1) % (sizeb)) + m + 1;
		}
		while (Leftof(pointset.points[bindex], pointset.points[aindex], pointset.points[(aindex - b - 1 + sizea % sizea) + b]) <= 0)
		{
			cout << "2";
			aindex = (aindex - b - 1 + sizea % sizea) + b;
			leave = 0;
		}
	}
	int lowerb, lowera;
	lowerb = bindex;
	lowera = aindex;

	aindex = ia;
	bindex = ib;
	//upperhull
	while (!leave)
	{
		leave = 1;
		while (Leftof(pointset.points[bindex], pointset.points[aindex], pointset.points[(aindex - b + 1 % (sizea)) + b]) >= 0)
		{
			aindex = (aindex - b + 1 % (sizea)) + b;
		}
		while (Leftof(pointset.points[aindex], pointset.points[bindex], pointset.points[(bindex - m + 1 - 1 + sizeb % sizeb) + m + 1]) <= 0)
		{
			bindex = (bindex - m + 1 - 1 + sizeb % sizeb) + m + 1;
			leave = 0;
		}
	}
	int upperb, uppera;
	upperb = bindex;
	uppera = aindex;
	

}


Point p0;
/*some errors I fixed were I had to switch lefton because SDL switches y direction. I dealt with colinear points by saying they were left when that caused a bug and all
I had to do was make it right. I had logical errors where I checked the point set instead of the current convexhull when backtracking.
It starts by sorting to find the bottommostright point, then I find all the angles by using cross product if angles are same sort by distance.
Then go through and get rid of all points with same angle except largest distance.
once its all sorted its really easy just step and see if it turns left, if it turns left its good switch the two new points. If its right then just go back 
in the convexhull array and check and delete until its a right turn. Thats all.
This algorith runs in O(nlgn) time which is very quick. This time is based off sorting, the steps only take 2N max to do it.

This seems to work very well and is optimized.
*/
Polygon ConvexHullGrahamScan(Polygon& pointset)
{
	Polygon convexhull;
	convexhull.points.reserve(pointset.points.size());
	/*take care of special cases where n<=3*/
	if (pointset.points.size() <= 3)
	{
		for (int i = 0; i < pointset.points.size(); i++)
		{
			convexhull.AddPointvalue(pointset.points[i].x, pointset.points[i].y);
		}
		return convexhull;
	}
	int bottomrightindex = 0;
	int highesty = pointset.points[0].y;
	for (int i = 1; i < pointset.points.size(); i++)
	{
		if ((pointset.points[i].y > highesty) || (pointset.points[i].y == highesty && pointset.points[i].x > pointset.points[bottomrightindex].x))
		{
			bottomrightindex = i;
			highesty = pointset.points[i].y;
		}
	}
	swap(pointset.points[0], pointset.points[bottomrightindex]);
	Point startingpoint = pointset.points[0];
	p0 = startingpoint;
	sort(pointset.points.begin() + 1, pointset.points.end(), PointStorageSort);
	int m = 2;
	for (int i = 2; i < pointset.points.size(); i++)
	{
		int val = (pointset.points[i].y - p0.y) * (pointset.points[m - 1].x - pointset.points[i].x) -
			(pointset.points[i].x - p0.x) * (pointset.points[m - 1].y - pointset.points[i].y);
		if (val != 0)
		{
			pointset.points[m] = pointset.points[i];
			m++;
		}
	}

	Point one = startingpoint;
	Point two = pointset.points[1];
	convexhull.points.emplace_back(one.x, one.y);
	convexhull.points.emplace_back(two.x, two.y);
	for (int i = 2; i < m; i++)//n times
	{
		if (pointset.points[i].x == two.x && pointset.points[i].y == two.y)
			continue;
		
		if (Leftof(one, two, pointset.points[i]))
		{
			convexhull.points.emplace_back(pointset.points[i].x, pointset.points[i].y);
			one = two;
			two = pointset.points[i];
		}
		else
		{
			convexhull.points.pop_back();
			for (int j = convexhull.points.size() - 2; j >= 0; j--)
			{
				if (Leftof(convexhull.points[j], convexhull.points[j + 1], pointset.points[i]))
				{
					break;
				}
				else
				{
					convexhull.points.pop_back();
				}
			}		
			two = pointset.points[i];
			one = convexhull.points[convexhull.points.size() - 1];
			convexhull.points.emplace_back(pointset.points[i].x, pointset.points[i].y);
		}
	}
	//convexhull.points.shrink_to_fit();
	return convexhull;
}

/*
Algorithm first checks if n <= 3 which is a trivial case. Then it sorts the pointset to the highest rightmost point.
Then it creates a horizontal line and caculates the angle from each point to that using dot product equation.
The two most recent convexhull points form the new line and each point is subtracted from the newest point to compare angles.
The one with the highest angle is the next convex point.
Degeneracies only mattered if they happened on convex points. If convex points were collinear or the same point. If they are the same point as the
two current convex points then it skips it. If the new convex points are the same it will treat them as collinear and deal with that next. If points are
collinear you get an array of all the convexpoints who have the same angle, now compare the distance the one with the largest distance from the other
convex point is the new convex point.
This algorithms seems to work with every case now. Its runtime is O(n*h) where h is hull points.
*this runs slower because bottleneck is the angle calculation it has to do that big calculation n*h times. could implemenet
using turn lefts to make way quicker
*/
Polygon ConvexHullGiftWrap(Polygon& pointset)
{
	Polygon convexhull;
	/*take care of special cases where n<=3*/
	if (pointset.points.size() <= 3)
	{
		for (int i = 0; i < pointset.points.size(); i++)
		{
			convexhull.AddPointvalue(pointset.points[i].x, pointset.points[i].y);
		}
		return convexhull;
	}

	sort(pointset.points.begin(), pointset.points.end(), pointcompareright);
	bool leave = false;
	Point two = pointset.points[0];//bottomright point
	Point one(two.x - 10, two.y);//just make a horizontal line pointing to its left
	while (!leave)//h times
	{
		float angle;
		float topangle = -1.0f;
		int nexthullindex = -1;
		Point convexline = pointsubract(one, two);
		vector<int> collinearpoints;
		for (int i = 0; i < pointset.points.size(); i++)//n times
		{
			if ((pointset.points[i].x == two.x && pointset.points[i].y == two.y) || (pointset.points[i].x == one.x && pointset.points[i].y == one.y))
			{
				continue;
			}
			Point iline = pointsubract(pointset.points[i], two);
			angle = acos(dotproduct(convexline, iline) / (pointmagnitude(convexline)*pointmagnitude(iline)));
			if (angle > topangle)
			{
				topangle = angle;
				nexthullindex = i;
				collinearpoints.clear();
			}
			else if (angle == topangle)
			{
				collinearpoints.push_back(i);
			}
		}
		
		if (collinearpoints.size() > 0)//this saves to much time skipping all the collinear points
		{
			//cout << "colinear";
			for (int c = 0; c < collinearpoints.size(); c++)
			{
				Point currentp = pointsubract(pointset.points[nexthullindex], one);
				Point newp = pointsubract(pointset.points[collinearpoints[c]], one);
				if (pointmagnitude(newp) > pointmagnitude(currentp))
				{
					nexthullindex = collinearpoints[c];
				}
			}
		}
		convexhull.AddPointvalue(pointset.points[nexthullindex].x, pointset.points[nexthullindex].y);
		one = two;
		two = pointset.points[nexthullindex];
		if (nexthullindex == 0)
			leave = true;
	}

	return convexhull;
}

/*
This uses linked lists because it needs to remove and insert elements in the middle. The method of finding the two points is using the equation 
f(p) = r * (p - q) which is based off the line to plane intersection algorithm. Pretty much you find the normal of the line (reciprical) then plugging in any point
will give you 0 if its on the line or a positive or negative depending on what side its on. * is dot product. 
this algorithm runs in O(n^2) time complexity
**still has problems with points on linear point, really just at beggining if first points are all on same line or x
**fixed issues where first 3 are collinear, only breaks if first 3 points are identical which is an annoying fix but easy
*/
Polygon ConvexHullIncremental(Polygon& pointset)
{
	Polygon convexhull;
	list<Point> convexlist;
	list<Point>::iterator it;
	list<Point>::iterator it2;

	/*take care of special cases where n<=3*/
	if (pointset.points.size() <= 3)
	{
		for (int i = 0; i < pointset.points.size(); i++)
		{
			convexhull.AddPointvalue(pointset.points[i].x, pointset.points[i].y);
		}
		return convexhull;
	}

	sort(pointset.points.begin(), pointset.points.end(), comparepoints);
	/*for (int i = 0; i < 3; i++)
	{
		convexlist.emplace_back(pointset.points[i].x, pointset.points[i].y);
	}*/
	for (int i = 0; i < 3; i++)
	{
		Point tmp(pointset.points[i].x, pointset.points[i].y);
		convexhull.points.push_back(tmp);
	}
	int index = 3;
	while (convexhull.points[0].x == convexhull.points[1].x && convexhull.points[0].x == convexhull.points[2].x)
	{
		//cout << "dealt with first 3 being colinear" << endl;
		if (index > pointset.points.size())
		{
			cout << "error all colinear"; return convexhull;
		}
		int top;
		int bottom;
		if ((convexhull.points[0].y > convexhull.points[1].y && convexhull.points[0].y > convexhull.points[2].y))
			top = 0;
		else if ((convexhull.points[1].y > convexhull.points[0].y && convexhull.points[1].y > convexhull.points[2].y))
			top = 1;
		else
			top = 2;
		if (convexhull.points[0].y <= convexhull.points[1].y && convexhull.points[0].y <= convexhull.points[2].y)
			bottom = 0;
		else if (convexhull.points[1].y < convexhull.points[0].y && convexhull.points[1].y < convexhull.points[2].y)
			bottom = 1;
		else
			bottom = 2;
		Point save = convexhull.points[top];
		convexhull.points[0] = convexhull.points[bottom];
		convexhull.points[1] = save;
		convexhull.points[2] = pointset.points[index];
		index++;
	}
	for (int i = 0; i < 3; i++)
	{
		convexlist.emplace_back(convexhull.points[i].x, convexhull.points[i].y);
	}
	convexhull.points.clear();

	for (int i = index; i < pointset.points.size(); i++)
	{
		bool visible = false;
		bool switched = false;
		float ydiff, xdiff;
		int index1 = -1;
		int index2 = -1;
		for (int j = 0; j < convexlist.size(); j++)
		{
			it = convexlist.begin(); std::advance(it, (j + 1) % convexlist.size());
			it2 = convexlist.begin(); std::advance(it2, j);
			ydiff = it->y - it2->y;
			xdiff = it->x - it2->x;
			Point r(1, (-xdiff / ydiff));

			it = convexlist.begin(); std::advance(it, j);
			Point v1 = pointsubract(pointset.points[i], *it);
			if (j == 0) {
				it = convexlist.begin(); std::advance(it, convexlist.size() - 1);
			}
			else {
				it = convexlist.begin(); std::advance(it, j - 1);
			}
			it2 = convexlist.begin(); std::advance(it2, j);
			Point v2 = pointsubract(*it, *it2);
			float value1 = dotproduct(r, v1);
			float value2 = dotproduct(r, v2);

			if ((value1 < 0 && value2 < 0) || (value1 > 0 && value2 > 0))//invisible
			{
				if (switched == true)
				{
					index2 = j;
					it = convexlist.begin(); std::advance(it, index1+1);
					it2 = convexlist.begin(); std::advance(it2, index2);
					convexlist.erase(it, it2);
					it = convexlist.begin(); std::advance(it, index1 + 1);
					convexlist.insert(it, pointset.points[i]);
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

			/*shouldn't reach here end catch error means its edge 0 and 1*/
			if ((j == convexlist.size() - 1))
			{
				index2 = 0;
				it = convexlist.begin(); std::advance(it, index1 + 1);
				it2 = convexlist.end();
				convexlist.erase(it, it2);
				it = convexlist.begin(); std::advance(it, index1 + 1);
				convexlist.insert(it, pointset.points[i]);
			}
		}
	}
	
	/*copy list into polygon*/
	for (int i = 0; i < convexlist.size(); i++)
	{
		it = convexlist.begin(); std::advance(it, i);
		convexhull.AddPointvalue(it->x, it->y);
	}

	return convexhull;
}

float dotproduct(Point p1, Point p2)
{
	return p1.x*p2.x + p1.y*p2.y;
}

Point pointsubract(const Point& p1, const Point& p2)
{
	Point tmp(p1.x - p2.x, p1.y - p2.y);
	return tmp;
}

bool comparepoints(const Point& p1, const Point& p2)
{
	return p1.x < p2.x;
}

bool PointStorageSort(const Point& p1, const Point& p2)
{
	int val = (p1.y - p0.y) * (p2.x - p1.x) -
		(p1.x - p0.x) * (p2.y - p1.y);
	if (val == 0)
	{
		return pointsmagnitudequick(p0, p1) > pointsmagnitudequick(p0, p2);
	}
	if (val < 0)
		return false;

	return true;
	/*if (p1.storage == p2.storage)
	{
		return pointsmagnitudequick(p0, p1) > pointsmagnitudequick(p0, p2);
	}
	return p1.storage > p2.storage;*/
}

bool pointcompareright(const Point& p1, const Point& p2)
{
	if (p1.y > p2.y)
	{
		return true;
	}
	if (p1.y == p2.y)
	{
		if (p1.x > p2.x)
		{
			return true;
		}
	}
	return false;
}

bool pointcomparerightback(const Point& p1, const Point& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return false;

	if (p1.y > p2.y)
	{
		return false;
	}
	if (p1.y == p2.y && p1.x > p2.x)
	{
		return false;
	}

	return true;
}

float pointmagnitude(const Point& p)
{
	return sqrt(pow(p.x, 2) + pow(p.y, 2));
}

float pointsmagnitudequick(const Point& p1, const Point& p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool Leftof(const Point& a, const Point& b, const Point& c)//** in sdl y is reflected so the answer will be reversed
{
	Point v = pointsubract(c, a);
	Point w = pointsubract(b, a);

	float cross = v.x*w.y - v.y*w.x;
	if (cross > 0)
		return true;
	if (cross < 0)
		return false;
	
	return false;
}

int Leftofvalue(const Point& a, const Point& b, const Point& c)
{
	Point v = pointsubract(c, a);
	Point w = pointsubract(b, a);

	float cross = v.x*w.y - v.y*w.x;
	if (cross > 0)
		return 1;
	if (cross < 0)
		return -1;

	return 0;
}

float Leftoffloat(const Point& a, const Point& b, const Point& c)
{
	Point v = pointsubract(c, a);
	Point w = pointsubract(b, a);

	float cross = v.x*w.y - v.y*w.x;
	//cout << cross << endl;
	return cross;
}

float Leftoffloat(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	glm::vec2 v = c - a;
	glm::vec2 w = b - a;

	float cross = v.x*w.y - v.y*w.x;
	//cout << cross << endl;
	return cross;
}

Point pointadd(const Point& p1, const Point& p2)
{
	Point tmp(p1.x + p2.x, p1.y + p2.y);
	return tmp;
}
Point pointmultiplyconstant(const Point& p1, float value)
{
	Point tmp(p1.x*value, p1.y*value);
	return tmp;
}