#pragma once
#include "FortuneEventTree.h"
#include <iostream>
#include <vector>
#include <queue>
#include <glm\glm.hpp>
float Leftoffloat(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
bool SegmentsIntersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
bool OnSegment(glm::vec2 pi, glm::vec2 pj, glm::vec2 pk);
bool collinear(Point& p1, Point& p2, Point& p3);
glm::dvec2 LineIntersectionPoint(glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3, glm::dvec2 p4);
double scanliney;
/*
The math works by finding the parabola having a focus point p and on the line L. So you have two pararbola equations and set them equal to eachother.
Then to avoid using messy math just use ax^2 + bx + c = 1 quadratic formula. Find a,b,and c component for both parabolas, subtract right hand to left hand and do
quadratic. There are two answers of course the beach line is the lowest(highest in my case) y value of all parabola intersections.
*/
double getbreakpoint(glm::dvec2 p, glm::dvec2 r, double y)
{
	double dp = 2.0 * (p.y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p.x / dp;
	double c1 = y + dp / 4 + p.x * p.x / dp;

	dp = 2.0 * (r.y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r.x / dp;
	double c2 = y + dp / 4 + r.x * r.x / dp;

	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;

	double disc = b * b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2 * a);
	double x2 = (-b - std::sqrt(disc)) / (2 * a);

	//double r1 = (x1 * x1 - 2 * p.x*x1 + p.x*p.x + p.y*p.y - y * y) / (2.0 * (p.y - y));
	//double r2 = (x2 * x2 - 2 * p.x*x2 + p.x*p.x + p.y*p.y - y * y) / dp;
	//if a,c its leftmost
	//its the one closest to a,c a
	double ry;
	if (p.y > r.y) ry = std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;
}
struct beachnode;
struct Node;
class FortunesBeachLine;
struct fortuneevent
{
	glm::dvec2 site;
	double radius;
	bool siteevent = false;
	Node* leafz;

	friend ostream& operator<<(ostream& os, const fortuneevent& e)
	{
		os << "(" << e.site.x << "," << e.site.y << ") leaf: ";
		return os;
	}
	/*
	if (leafz == NULL || leafz != other.leafz)
			{
				return true;
			}
			return false;
	*/
	bool operator<(const fortuneevent& other)//*same y?
	{
		if (site.y < other.site.y)
		{
			return true;
		}
		else if (site.y == other.site.y)
		{
			if (site.x < other.site.x)
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
	bool operator>(const fortuneevent& other)
	{
		if (site.y > other.site.y)
		{
			return true;
		}
		else if (site.y == other.site.y)
		{
			if (site.x > other.site.x)
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
};
struct sitetuple
{
	glm::dvec2 leftsite;
	glm::dvec2 rightsite;

	sitetuple()
	{
		leftsite.x = -1; leftsite.y = -1;
		rightsite.x = -1; rightsite.y = -1;
	}
	friend std::ostream& operator<<(std::ostream& os, const sitetuple& e)
	{
		os << "site1:(" << e.leftsite.x << "," << e.leftsite.y << ")site2:(" << e.rightsite.x << "," << e.rightsite.y << ")";
		return os;
	}
};
struct beachnode
{
	bool leaf;
	glm::dvec2 leafsite;
	//FortuneEventTree<fortuneevent>::EventNode* leafevent;
	fortuneevent* leafevent;
	sitetuple internalsites;
	int internaledgeindex;

	beachnode()
	{
		leafsite.x = -1;
		leafsite.y = -1;
		internaledgeindex = -1;
		leafevent = NULL;
	}

	friend std::ostream& operator<<(std::ostream& os, const beachnode& e)
	{
		if (e.leaf)
		{
			os << "leaf:(" << e.leafsite.x << "," << e.leafsite.y << ")";
		}
		else
		{
			os << "internal:(" << e.internalsites.leftsite.x << "," << e.internalsites.leftsite.y << ")(" << e.internalsites.rightsite.x << "," << e.internalsites.rightsite.y << ")(index:"<< e.internaledgeindex<<")";
		}
		return os;
	}
	bool operator==(const beachnode& other)//if values arent exact use memory instead of this
	{
		if (leaf == other.leaf)
		{
			if (leaf == true)
			{
				if (leafsite == other.leafsite && leafevent == other.leafevent)
				{
					return true;
				}
				return false;
			}
			else
			{
				if ((internalsites.leftsite == other.internalsites.leftsite) && (internalsites.rightsite == other.internalsites.rightsite) && (internaledgeindex == other.internaledgeindex))
				{
					return true;
				}
				return false;
			}
		}
		return false;
	}
	bool operator<(const beachnode& other)//other is tuple, leftside is leaf*same y?
	{
		double breakpointx = getbreakpoint(other.internalsites.leftsite, other.internalsites.rightsite, scanliney);
		if (leafsite.x < breakpointx)
		{
			return true;
		}
		return false;
	}
	bool operator>(const beachnode& other)
	{
		double breakpointx = getbreakpoint(other.internalsites.leftsite, other.internalsites.rightsite, scanliney);
		if (leafsite.x > breakpointx)
		{
			return true;
		}
		return false;
	}
};

/*This is a balanced binary tree, specifically, an AVL Tree
The only functions to overload are <, >, and <<
*/
struct Node {
	beachnode value;
	Node* parent;
	Node* left;
	Node* right;
	int height;
};
class FortunesBeachLineTree
{
public:
	FortunesBeachLineTree()
	{
		root = NULL;
		Q = NULL;
		voronoi = NULL;
		insertedleft = NULL;
		insertedright = NULL;
	}
	~FortunesBeachLineTree()
	{
		//cleartree();
	}
	int getheight() { return root == nullptr ? 0 : root->height; }
	void insert(beachnode, int);
	void remove(beachnode);
	void printinorder();
	void printnodeinfo();
	void printlevelorder();
	bool IsTreeBalanced();
	void BoundVoronoi();
	sitetuple getneighbors(beachnode);
	beachnode poptop();
	bool empty()
	{
		if (root == NULL)
		{
			return true;
		}
		return false;
	}
	void cleartree();
	FortuneEventTree<fortuneevent>* Q;
	Voronoi* voronoi;
	Polygon* Sites;

	Node* root;
	Node* insertedleft;
	Node* insertedright;
	sitetuple neighborfunction(Node*& searchnode);
	void deletetripleevent(Node*& searchnode, int eindex, int vindex);
	void makedeletion(Node*& searchnode, bool left, int eindex, int vindex);
private:

	sitetuple getneighbors(Node*& searchnode, beachnode newnode);
	void BoundVoronoi(Node*& searchnode);
	void insert(Node*& searchnode, Node*& newnode, int);
	void insertoperation(Node*& searchnode, Node*& newnode, int);
	void remove(Node*& searchnode, beachnode num);
	void printinorder(Node*& node);
	void printnodeinfo(Node*& node);
	void printlevelorder(Node*& node);
	void cleartree(Node*& node);
	bool IsTreeBalanced(Node*& node);
	Node* findmin(Node* node)
	{
		assert(node != NULL);
		Node* t = node;
		while (t->left != NULL)
		{
			t = t->left;
		}
		return t;
	}
	Node* findmax(Node* node)
	{
		assert(node != NULL);
		Node* t = node;
		while (t->right != NULL)
		{
			t = t->right;
		}
		return t;
	}
	void RightRotate(Node*& node)
	{
		Node* leftchild = node->left;
		node->left = leftchild->right;
		leftchild->right->parent = node;
		leftchild->right = node;
		leftchild->parent = node->parent;
		node->parent = leftchild;
		leftchild->height = max(height(leftchild->left), height(leftchild->right)) + 1;
		node->height = max(height(node->left), height(node->right)) + 1;
		node = leftchild;
	}
	void LeftRotate(Node*& node)
	{
		Node* rightchild = node->right;
		node->right = rightchild->left;
		rightchild->left->parent = node;
		rightchild->left = node;
		rightchild->parent = node->parent;
		node->parent = rightchild;
		rightchild->height = max(height(rightchild->left), height(rightchild->right)) + 1;
		node->height = max(height(node->left), height(node->right)) + 1;
		node = rightchild;
	}
	int height(Node *N)
	{
		if (N == NULL)
			return 0;
		return N->height;
	}
	int getBalance(Node *N)
	{
		if (N == NULL)
			return 0;
		return height(N->left) - height(N->right);
	}
	int max(int a, int b)
	{
		return (a > b) ? a : b;
	}
	void balance(Node*& node)
	{
		//std::cout << node->value;
		if (node == NULL)
		{
			return;
		}

		int balance = getBalance(node);

		if (balance > 1)
		{
			if (height(node->left->left) > height(node->left->right))
			{
				RightRotate(node);
			}
			else
			{
				LeftRotate(node->left);
				RightRotate(node);
			}
		}
		else if (balance < -1)
		{
			if (height(node->right->right) > height(node->right->left))
			{
				LeftRotate(node);
			}
			else
			{
				RightRotate(node->right);
				LeftRotate(node);
			}
		}

		node->height = max(height(node->left), height(node->right)) + 1;
	}
};
/*Bound voronoi*/
void FortunesBeachLineTree::BoundVoronoi()
{
	BoundVoronoi(root);
}
void FortunesBeachLineTree::BoundVoronoi(Node*& searchnode)
{
	if (searchnode == NULL)
	{
		return;
	}
	BoundVoronoi(searchnode->left);
	BoundVoronoi(searchnode->right);
	//do operation
	if (searchnode->value.leaf == false)
	{
		if (searchnode->value.internaledgeindex == -1)
		{
			return;
		}
		if (voronoi->vedges[searchnode->value.internaledgeindex].x != -1 && voronoi->vedges[searchnode->value.internaledgeindex].y != -1)
		{
			return;
		}
		glm::dvec2 leftsite = searchnode->value.internalsites.leftsite;
		glm::dvec2 rightsite = searchnode->value.internalsites.rightsite;
		glm::dvec2 middle = leftsite + rightsite; middle.x = middle.x / 2.0; middle.y = middle.y / 2.0;
		glm::dvec2 dir = rightsite - leftsite;
		double savey = dir.y; dir.y = -dir.x; dir.x = savey; dir = glm::normalize(dir);
		glm::dvec2 point1 = middle + 10000.0 * dir;
		glm::dvec2 point2 = middle - 10000.0 * dir;
		glm::dvec2 intersectionone;
		glm::dvec2 intersectiontwo;
		int count = 0;
		double boxsize = 5000;
		std::vector<glm::dvec2> boxvertex;
		glm::dvec2 topleft(-boxsize, -boxsize);
		glm::dvec2 topright(boxsize, -boxsize);
		glm::dvec2 bottomright(boxsize, boxsize);
		glm::dvec2 bottomleft(-boxsize, boxsize);
		boxvertex.push_back(topleft); boxvertex.push_back(topright);
		boxvertex.push_back(bottomright); boxvertex.push_back(bottomleft);
		for (int i = 0; i < 4; i++)
		{
			if (SegmentsIntersect(boxvertex[i], boxvertex[(i + 1) % 4], point1, point2))
			{
				if (count == 0)
				{
					intersectionone = LineIntersectionPoint(boxvertex[i], boxvertex[(i + 1) % 4], point1, point2);
					count++;
				}
				else if (count == 1)
				{
					intersectiontwo = LineIntersectionPoint(boxvertex[i], boxvertex[(i + 1) % 4], point1, point2);
					count++;
				}
			}
		}
		if (count != 2)
		{
			std::cout << "ERROR: bounding box didnt get 2 intersections" << std::endl;
		}
		if (voronoi->vedges[searchnode->value.internaledgeindex].x == -1 && voronoi->vedges[searchnode->value.internaledgeindex].y == -1)
		{
			voronoi->AddVertex(intersectionone.x, intersectionone.y);
			voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
			voronoi->AddVertex(intersectiontwo.x, intersectiontwo.y);
			voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
			return;
		}

		//find site other than these two
		int leftcount = 0;
		int rightcount = 0;
		for (int i = 0; i < Sites->points.size(); i++)
		{
			float valz = Leftoffloat(leftsite, rightsite, glm::dvec2(Sites->points[i].x, Sites->points[i].y));
			if (valz < 0)
			{
				leftcount++;
			}
			if (valz > 0)
			{
				rightcount++;
			}
		}
		if (rightcount > leftcount)
		{
			float valz = 1;
			float orientation1 = Leftoffloat(leftsite, rightsite, intersectionone);
			if (orientation1 <= 0)
			{
				voronoi->AddVertex(intersectionone.x, intersectionone.y);
				voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
				return;
			}
			voronoi->AddVertex(intersectiontwo.x, intersectiontwo.y);
			voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
			return;
		}
		else
		{
			float valz = -1;
			float orientation1 = Leftoffloat(leftsite, rightsite, intersectionone);
			if (orientation1 > 0)
			{
				voronoi->AddVertex(intersectionone.x, intersectionone.y);
				voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
				return;
			}
			voronoi->AddVertex(intersectiontwo.x, intersectiontwo.y);
			voronoi->AddVertextoEdge(searchnode->value.internaledgeindex, voronoi->Getlastvertexindex());
			return;
		}
		/*glm::dvec2 othervertex;
		if (voronoi->vedges[searchnode->value.internaledgeindex].x == -1)
		{
			othervertex = voronoi->vvertex[voronoi->vedges[searchnode->value.internaledgeindex].y];
		}
		else
		{
			othervertex = voronoi->vvertex[voronoi->vedges[searchnode->value.internaledgeindex].x];
		}
		float orientationsite = Leftoffloat(leftsite, rightsite, othervertex);*/
	}
}
/*Get Neighbors*/
sitetuple FortunesBeachLineTree::getneighbors(beachnode newnode)
{
	if (root == NULL)
	{
		sitetuple tmp;
		return tmp;
	}
	else
	{
		return getneighbors(root, newnode);
	}
}
sitetuple FortunesBeachLineTree::getneighbors(Node*& searchnode, beachnode newnode)
{
	if (searchnode == NULL)
	{
		std::cout << "value is not in tree";
		sitetuple tmp;
		return tmp;
	}
	else if (searchnode->value.leaf == true)
	{
		return neighborfunction(searchnode);
	}
	else if (newnode < searchnode->value)
	{
		return getneighbors(searchnode->left, newnode);
	}
	else if (newnode > searchnode->value)
	{
		return getneighbors(searchnode->right, newnode);
	}
}
sitetuple FortunesBeachLineTree::neighborfunction(Node*& searchnode)
{
	//assert(searchnode->value.leaf == true, "ERROR: neighborfunction node not a leaf");

	glm::dvec2 left = glm::dvec2(-1, -1);
	glm::dvec2 right = glm::dvec2(-1, -1);
	sitetuple tmp;
	tmp.leftsite = left;
	tmp.rightsite = right;
	bool findleft = false;
	if (searchnode->parent != NULL)//corrupted parent pointer
	{
		//assert(searchnode->parent->left != NULL, "neighborfunction nodes left child doesnt exist");
		if (searchnode->parent->left->value == searchnode->value)
		{
			/*Node* min = findmin(searchnode->parent->right);
			if (min->value.leaf == false)
			{
				std::cout << "leaf error";
			}
			tmp.rightsite = min->value.leafsite;*/
			tmp.rightsite = searchnode->parent->value.internalsites.rightsite;
			findleft = true;
		}
		else
		{
			/*Node* max = findmax(searchnode->parent->left);
			if (max->value.leaf == false)
			{
				std::cout << "leaf error";
			}
			tmp.leftsite = max->value.leafsite;*/
			tmp.leftsite = searchnode->parent->value.internalsites.leftsite;
		}
	}
	else
	{
		return tmp;
	}
	if (findleft)
	{
		Node* currenthead = searchnode->parent;
		bool leave = false;
		while (!leave)
		{
			if (currenthead->parent != NULL)
			{
				if (currenthead->parent->right != NULL)
				{
					if (currenthead->parent->right->value == currenthead->value)
					{
						//assert(currenthead->parent->right->value == currenthead->value, "leftneighbor functio error");
						leave = true;
					}
				}
				currenthead = currenthead->parent;
			}
			else
			{
				return tmp;
			}
		}
		/*Node* max = findmax(currenthead->left);
		if (max->value.leaf == false)
		{
			std::cout << "leaf error";
		}
		tmp.leftsite = max->value.leafsite;*/
		tmp.leftsite = currenthead->value.internalsites.leftsite;
	}
	else
	{
		Node* currenthead = searchnode->parent;
		bool leave = false;
		while (!leave)
		{
			if (currenthead->parent != NULL)
			{
				if (currenthead->parent->left != NULL)
				{
					if (currenthead->parent->left->value == currenthead->value)
					{
						//assert(currenthead->parent->left->value == currenthead->value, "rightneighbor functio error");
						leave = true;
					}
				}
				currenthead = currenthead->parent;
			}
			else
			{
				return tmp;
			}
		}
		/*Node* min = findmin(currenthead->right);
		if (min->value.leaf == false)
		{
			std::cout << "leaf error";
		}
		tmp.rightsite = min->value.leafsite;*/
		tmp.rightsite = currenthead->value.internalsites.rightsite;
	}
	return tmp;
}
/*Get Neighbors info*/
void FortunesBeachLineTree::deletetripleevent(Node*& searchnode, int eindex, int vindex)
{
	/*if (searchnode->value.leafevent != NULL)
	{
		Q->remove(*searchnode->value.leafevent);
		searchnode->value.leafevent = NULL;
	}*/
	bool findleft = false;
	if (searchnode->parent != NULL)
	{
		if (searchnode->parent->left == NULL)
		{
			//std::cout << searchnode->parent->value << std::endl;
		}
		//assert(searchnode->parent->left != NULL, "deletetripleevent nodes parents left child doesnt exist");

		if (searchnode->parent->left->value == searchnode->value)
		{
			Node* min = findmin(searchnode->parent->right);
			//assert(min->value.leaf == true, "deletetripleevent right neighbor found not leaf");
			if (min->value.leafevent != NULL)
			{
				Q->remove(*min->value.leafevent);
				min->value.leafevent = NULL;
			}
			insertedright = min;
			findleft = true;
		}
		else
		{
			Node* max = findmax(searchnode->parent->left);
			//assert(max->value.leaf == true, "deletetripleevent left neighbor found not leaf");
			if (max->value.leafevent != NULL)
			{
				Q->remove(*max->value.leafevent);
				max->value.leafevent = NULL;
				//Q->makedeletion(max->value.leafevent);
			}
			insertedleft = max;
		}
		//add edge reference
		voronoi->AddVertextoEdge(searchnode->parent->value.internaledgeindex, vindex);
	}
	else
	{
		std::cout << "deletetripleevent, deleting last leaf in T" << std::endl;
		return;
	}
	if (findleft)
	{
		Node* currenthead = searchnode->parent;
		bool leave = false;
		while (!leave)
		{
			if (currenthead->parent != NULL)
			{
				if (currenthead->parent->right != NULL)
				{
					if (currenthead->parent->right->value == currenthead->value)
					{
						leave = true;
					}
				}
				currenthead = currenthead->parent;
			}
			else
			{
				return;
			}
		}
		//add edge reference
		voronoi->AddVertextoEdge(currenthead->value.internaledgeindex, vindex);
		currenthead->value.internalsites.rightsite = insertedright->value.leafsite;
		currenthead->value.internaledgeindex = eindex;
		voronoi->AddVertextoEdge(eindex, vindex);

		Node* max = findmax(currenthead->left);
		//assert(max->value.leaf == true, "deletetripleevent left neighbor found not leaf2");
		if (max->value.leafevent != NULL)
		{
			Q->remove(*max->value.leafevent);
			max->value.leafevent = NULL;
		}
		insertedleft = max;
	}
	else
	{
		Node* currenthead = searchnode->parent;
		bool leave = false;
		while (!leave)
		{
			if (currenthead->parent != NULL)
			{
				if (currenthead->parent->left != NULL)
				{
					if (currenthead->parent->left->value == currenthead->value)
					{
						leave = true;
					}
				}
				currenthead = currenthead->parent;
			}
			else
			{
				return;
			}
		}
		//add edge reference
		voronoi->AddVertextoEdge(currenthead->value.internaledgeindex, vindex);
		currenthead->value.internalsites.leftsite = insertedleft->value.leafsite;
		currenthead->value.internaledgeindex = eindex;
		voronoi->AddVertextoEdge(eindex, vindex);

		Node* min = findmin(currenthead->right);
		//assert(min->value.leaf == true, "deletetripleevent right neighbor found not leaf2");
		if (min->value.leafevent != NULL)
		{
			Q->remove(*min->value.leafevent);
			min->value.leafevent = NULL;
		}
		insertedright = min;
	}
}
/*Insert*/
void FortunesBeachLineTree::insert(beachnode newnode, int vindex)
{
	Node* tmp = new Node();
	tmp->value = newnode;
	tmp->height = 1;
	tmp->left = NULL;
	tmp->parent = NULL;
	tmp->right = NULL;
	if (root == NULL)
	{
		tmp->value.leaf = true;
		root = tmp;
	}
	else
	{
		insert(root, tmp, vindex);
	}
}
void FortunesBeachLineTree::insert(Node*& searchnode, Node*& newnode, int vindex)
{
	if (searchnode->value.leaf == true)
	{
		if (searchnode->value.leafevent != NULL)
		{
			Q->remove(*searchnode->value.leafevent);
			searchnode->value.leafevent = NULL;
			//Q->makedeletion(searchnode->value.leafevent);
		}
		insertoperation(searchnode, newnode, vindex);
		return;
	}
	else if (newnode->value < searchnode->value)
	{
		insert(searchnode->left, newnode, vindex);
	}
	else
	{
		insert(searchnode->right, newnode, vindex);
	}

	//balance(searchnode);
}
void FortunesBeachLineTree::insertoperation(Node*& searchnode, Node*& newnode, int vindex)
{
	/*
	newsite.leaf = true;
	newsite.leafsite = tmp.site;
	newsite.leafevent = NULL;	
	*/

	Node* leafaleft = new Node();
	leafaleft->value.leaf = true;
	leafaleft->value.leafevent = NULL;
	leafaleft->value.leafsite = searchnode->value.leafsite;
	leafaleft->left = NULL;
	leafaleft->right = NULL;

	Node* leafaright = new Node();
	leafaright->value.leaf = true;
	leafaright->value.leafevent = NULL;
	leafaright->value.leafsite = searchnode->value.leafsite;
	leafaright->left = NULL;
	leafaright->right = NULL;

	Node* internalleft = new Node();
	internalleft->value.leaf = false;
	internalleft->value.internalsites.leftsite = searchnode->value.leafsite;
	internalleft->value.internalsites.rightsite = newnode->value.leafsite;
	internalleft->value.internaledgeindex = vindex;
	internalleft->left = leafaleft;
	internalleft->right = newnode;

	searchnode->value.leaf = false;
	searchnode->value.internalsites.leftsite = newnode->value.leafsite;
	searchnode->value.internalsites.rightsite = searchnode->value.leafsite;
	searchnode->value.internaledgeindex = vindex;
	searchnode->left = internalleft;
	searchnode->right = leafaright;
	searchnode->height = 3;

	newnode->parent = internalleft;
	leafaleft->parent = internalleft;
	leafaright->parent = searchnode;
	internalleft->parent = searchnode;

	newnode->height = 1;
	leafaleft->height = 1;
	leafaright->height = 1;
	internalleft->height = 2;

	insertedleft = leafaleft;
	insertedright = leafaright;
}
/*Remove*/
void FortunesBeachLineTree::remove(beachnode num)
{
	if (root != NULL)
	{
		if (root->height == 1)
		{
			delete root;
			root = NULL;
		}
		else
		{
			remove(root, num);
		}
	}
}
void FortunesBeachLineTree::remove(Node*& searchnode, beachnode num)
{
	std::cout << "shouldnt be CALLED" << std::endl;
	return;
	if (searchnode == NULL)
	{
		std::cout << "value is not in tree";
		return;
	}
	else if (num < searchnode->value)
	{
		if (searchnode->left == NULL)
		{
			std::cout << "value is not in tree";
			return;
		}
		if (searchnode->left->value.leaf == true)
		{
			makedeletion(searchnode, true,0,0);
			return;
		}
		remove(searchnode->left, num);
	}
	else if (num > searchnode->value)
	{
		if (searchnode->right == NULL)
		{
			std::cout << "value is not in tree";
			return;
		}
		if (searchnode->right->value.leaf == true)
		{
			makedeletion(searchnode, false,0,0);
			return;
		}
		remove(searchnode->right, num);
	}
	balance(searchnode);
}
void FortunesBeachLineTree::makedeletion(Node*& searchnode, bool left, int eindex, int vindex)
{
	if (searchnode->parent == NULL)
	{
		std::cout << "delete root";
		delete searchnode;
		searchnode = NULL;
		return;
	}
	if (searchnode->parent->parent == NULL)
	{
		assert(1 == 0);
		if (left)
		{
			root = searchnode->parent->right;
			Node* tmp = searchnode->parent;
			delete searchnode;
			searchnode = NULL;
			delete tmp;
			tmp = NULL;
		}
		else
		{
			root = searchnode->parent->left;
			Node* tmp = searchnode->parent;
			delete searchnode;
			searchnode = NULL;
			delete tmp;
			tmp = NULL;
		}
		return;
	}

	bool leftgrand = false;
	if (searchnode->parent->parent->left->value == searchnode->parent->value)
	{
		leftgrand = true;
	}
	if (leftgrand)//if left internal node is same as deleting on then swap it with value
	{
		if (left)
		{
			//Node* max = findmax(searchnode->parent->right);
			//searchnode->parent->parent->value.internalsites.leftsite = max->value.leafsite;
		}
		else
		{
			//Node* max = findmax(searchnode->parent->left);
			//searchnode->parent->parent->value.internalsites.leftsite = max->value.leafsite;
			if (searchnode->parent->left->value.leaf == true)
			{
				searchnode->parent->parent->value.internalsites.leftsite = searchnode->parent->left->value.leafsite;
			}
			else
			{
				searchnode->parent->parent->value.internalsites.leftsite = searchnode->parent->left->value.internalsites.rightsite;
			}
		}
	}
	else
	{
		if (left)
		{
			//Node* min = findmin(searchnode->parent->right);
			//searchnode->parent->parent->value.internalsites.rightsite = min->value.leafsite;
			if (searchnode->parent->right->value.leaf == true)
			{
				searchnode->parent->parent->value.internalsites.rightsite = searchnode->parent->right->value.leafsite;
			}
			else
			{
				searchnode->parent->parent->value.internalsites.rightsite = searchnode->parent->right->value.internalsites.leftsite;
			}
		}
		else
		{
			//Node* min = findmin(searchnode->parent->left);
			//searchnode->parent->parent->value.internalsites.rightsite = min->value.leafsite;
		}
	}

	//check the same problem
	//assert(searchnode->parent->parent->value.internalsites.leftsite == searchnode->parent->parent->value.internalsites.rightsite);

	Node* par = searchnode->parent;
	Node** parbalance = &searchnode->parent->parent;
	if (left)
	{
		//cout << "LEFTDELETION";
		delete par->left;
		par->left = NULL;
		if (leftgrand)
		{
			par->parent->left = par->right;
		}
		else
		{
			par->parent->right = par->right;
		}
		par->right->parent = par->parent;
		delete par;
		par = NULL;
	}
	else
	{
		//cout << "RIGHTDELETION";
		delete par->right;
		par->right = NULL;
		if (leftgrand)
		{
			par->parent->left = par->left;
		}
		else
		{
			par->parent->right = par->left;
		}
		par->left->parent = par->parent;
		delete par;
		par = NULL;
	}
	//recursively balancing 
	/*std::cout << "balancing" << std::endl;
	assert(parbalance != NULL);
	bool leave2 = false;
	while (!leave2)//*parbalance != NULL
	{
		balance(*parbalance);
		if ((**parbalance).parent == NULL)
		{
			leave2 = true;
		}
		else
		{
			parbalance = &(**parbalance).parent;
		}
	}*/

	//only one new breakpoint arises so once you find it leave
	/*while (tmp->parent != NULL)
	{
		//searchnode->parent->value.internaledgeindex = eindex;
		//voronoi->AddVertextoEdge(eindex, vindex);
		bool leftgrand = false;
		if (tmp->parent->left != NULL)
		{
			if (tmp->parent->left == tmp)
			{
				leftgrand = true;
			}
		}
		if (leftgrand)
		{
				Node* max = findmax(tmp);
				if (max->value.leafsite != tmp->parent->value.internalsites.leftsite)
				{
					tmp->parent->value.internaledgeindex = eindex;
					voronoi->AddVertextoEdge(eindex, vindex);
					tmp->parent->value.internalsites.leftsite = max->value.leafsite;
					return;
				}
		}
		else
		{
				Node* min = findmin(tmp);
				if (tmp->parent->value.internalsites.rightsite != min->value.leafsite)
				{
					tmp->parent->value.internaledgeindex = eindex;
					voronoi->AddVertextoEdge(eindex, vindex);
					tmp->parent->value.internalsites.rightsite = min->value.leafsite;
					return;
				}
		}
		tmp = tmp->parent;
	}*/

	/*if (searchnode->parent != NULL)
	{
		searchnode->parent->value.internaledgeindex = eindex;
		voronoi->AddVertextoEdge(eindex, vindex);

		bool leftgrand = false;
		if (searchnode->parent->left != NULL)
		{
			if (searchnode->parent->left == searchnode)
			{
				leftgrand = true;
			}
		}
		if (leftgrand)//if left internal node is same as deleting on then swap it with value
		{
			if (left)
			{
				Node* max = findmax(searchnode->right);
				searchnode->parent->value.internalsites.leftsite = max->value.leafsite;
			}
			else
			{
				Node* max = findmax(searchnode->left);
				searchnode->parent->value.internalsites.leftsite = max->value.leafsite;
			}
		}
		else
		{
			if (left)
			{
				Node* min = findmin(searchnode->right);
				searchnode->parent->value.internalsites.leftsite = min->value.leafsite;
			}
			else
			{
				Node* min = findmin(searchnode->left);
				searchnode->parent->value.internalsites.leftsite = min->value.leafsite;
			}
		}
	}
	if (searchnode->parent->parent != NULL)
	{
		bool leftgrandgrand = false;
		if (searchnode->parent->parent->left != NULL)
		{
			if (searchnode->parent->parent->left == searchnode->parent)
			{
				leftgrandgrand = true;
			}
		}
		if (leftgrandgrand)//if left internal node is same as deleting on then swap it with value
		{
			if (left)
			{
				Node* max = findmax(searchnode->parent->right);
				searchnode->parent->parent->value.internalsites.leftsite = max->value.leafsite;
			}
			else
			{
				Node* max = findmax(searchnode->parent->left);
				searchnode->parent->parent->value.internalsites.leftsite = max->value.leafsite;
			}
		}
		else
		{
			if (left)
			{
				Node* min = findmin(searchnode->parent->right);
				searchnode->parent->parent->value.internalsites.leftsite = min->value.leafsite;
			}
			else
			{
				Node* min = findmin(searchnode->parent->left);
				searchnode->parent->parent->value.internalsites.leftsite = min->value.leafsite;
			}
		}
	}*/
}
/*print in order*/
void FortunesBeachLineTree::printinorder()
{
	if (root != NULL)
		printinorder(root);
	std::cout << std::endl;
}
void FortunesBeachLineTree::printinorder(Node*& node)
{
	if (node != NULL)
	{
		printinorder(node->left);
		std::cout << node->value << ", ";
		printinorder(node->right);
	}
}
/*print level order*/
void FortunesBeachLineTree::printnodeinfo()
{
	if (root != NULL)
		printnodeinfo(root);
	std::cout << std::endl;
}
void FortunesBeachLineTree::printnodeinfo(Node*& node)
{
	if (node == nullptr)
	{
		return;
	}

	std::queue<Node*> QUE;

	Node* temp = new Node();
	QUE.push(node);

	while (QUE.empty() == 0)
	{
		if (QUE.front()->left != nullptr)
		{
			temp = QUE.front();
			QUE.push(temp->left);
		}
		if (QUE.front()->right != nullptr)
		{
			temp = QUE.front();
			QUE.push(temp->right);
		}
		std::cout << QUE.front()->value << "left: ";
		if (QUE.front()->left != NULL)
		{
			std::cout << QUE.front()->left->value;
		}
		std::cout << "right: ";
		if (QUE.front()->right != NULL)
		{
			std::cout << QUE.front()->right->value;
		}
		std::cout << std::endl;
		/*std::cout << "parent: ";
		if (QUE.front()->parent != NULL)
		{
			std::cout << QUE.front()->parent->value;
		}
		std::cout << std::endl;*/
		QUE.pop();
	}
}