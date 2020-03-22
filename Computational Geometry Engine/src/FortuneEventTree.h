#pragma once
#ifndef FortuneEventTree_H
#define FortuneEventTree_H
#include <iostream>
#include <vector>
#include <queue>

/*This is a balanced binary tree, specifically, an AVL Tree
The only functions to overload are <, >, and <<
*/
template <class T>
class FortuneEventTree
{
public:
	FortuneEventTree()
	{
		root = NULL;
	}
	~FortuneEventTree()
	{
		cleartree();
	}
	int getheight() { return root == nullptr ? 0 : root->height; }
	void insert(T);
	void remove(T);
	void printinorder();
	void printnodeinfo();
	void printlevelorder();
	bool IsTreeBalanced();
	T poptop();
	void cleartree();
	bool empty()
	{
		if (root == NULL)
		{
			return true;
		}
		return false;
	}
	struct EventNode;
	void makedeletion(EventNode*& searchnode);
	struct EventNode {
		T value;
		EventNode* left;
		EventNode* right;
		int height;
	};
	EventNode* root;
	EventNode* recentlyinserted;
	T* recentlyinsertedevent;
private:
	void insert(EventNode*& searchnode, EventNode*& newnode);
	void remove(EventNode*& searchnode, T num);
	void printinorder(EventNode*& node);
	void printnodeinfo(EventNode*& node);
	void printlevelorder(EventNode*& node);
	void cleartree(EventNode*& node);
	bool IsTreeBalanced(EventNode*& node);
	EventNode* findmin(EventNode* node)
	{
		if (node == NULL)
		{
			std::cout << "find min error";
			return node;
		}
		EventNode* t = node;
		while (t->left != NULL)
		{
			t = t->left;
		}
		return t;
	}
	void RightRotate(EventNode*& node)
	{
		EventNode* leftchild = node->left;
		node->left = leftchild->right;
		leftchild->right = node;
		leftchild->height = max(height(leftchild->left), height(leftchild->right)) + 1;
		node->height = max(height(node->left), height(node->right)) + 1;
		node = leftchild;
	}
	void LeftRotate(EventNode*& node)
	{
		EventNode* rightchild = node->right;
		node->right = rightchild->left;
		rightchild->left = node;
		rightchild->height = max(height(rightchild->left), height(rightchild->right)) + 1;
		node->height = max(height(node->left), height(node->right)) + 1;
		node = rightchild;
	}
	int height(EventNode *N)
	{
		if (N == NULL)
			return 0;
		return N->height;
	}
	int getBalance(EventNode *N)
	{
		if (N == NULL)
			return 0;
		return height(N->left) - height(N->right);
	}
	int max(int a, int b)
	{
		return (a > b) ? a : b;
	}
	void balance(EventNode*& node)
	{
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
/*Insert*/
template <class T>
void FortuneEventTree<T>::insert(T newnode)
{
	EventNode* tmp = new EventNode();
	tmp->value = newnode;
	tmp->height = 1;
	tmp->left = NULL;
	tmp->right = NULL;
	insert(root, tmp);
}
template <class T>
void FortuneEventTree<T>::insert(EventNode*& searchnode, EventNode*& newnode)
{
	if (searchnode == NULL)
	{
		searchnode = newnode;
		recentlyinserted = searchnode;
		recentlyinsertedevent = &searchnode->value;
		return;
	}
	else if (newnode->value < searchnode->value)
	{
		insert(searchnode->left, newnode);
	}
	else if (newnode->value > searchnode->value)
	{
		insert(searchnode->right, newnode);
	}
	else
	{
		std::cout << "same inserted value in Q" << std::endl;
		assert(3 == 0);
	}
	balance(searchnode);
}
/*Remove*/
template <class T>
void FortuneEventTree<T>::remove(T num)
{
	if (root != NULL)
		remove(root, num);
}
template <class T>
void FortuneEventTree<T>::remove(EventNode*& searchnode, T num)
{
	if (searchnode == NULL)
	{
		std::cout << "value is not in Q tree";
		//assert(2 == 0);
		return;
	}
	else if (num < searchnode->value)
	{
		remove(searchnode->left, num);
	}
	else if (num > searchnode->value)
	{
		remove(searchnode->right, num);
	}
	else
	{
		makedeletion(searchnode);
	}

	balance(searchnode);
}
template <class T>
void FortuneEventTree<T>::makedeletion(EventNode*& searchnode)
{
	if (searchnode->left != NULL && searchnode->right != NULL)
	{
		searchnode->value = findmin(searchnode->right)->value;
		remove(searchnode->right, searchnode->value);
	}
	else
	{
		if (searchnode->left != NULL)
		{
			EventNode* tmp = searchnode->left;
			delete searchnode;
			searchnode = tmp;
		}
		else
		{
			EventNode* tmp = searchnode->right;
			delete searchnode;
			searchnode = tmp;
		}
	}
}
/*print in order*/
template <class T>
void FortuneEventTree<T>::printinorder()
{
	if (root != NULL)
		printinorder(root);
	std::cout << std::endl;
}
template <class T>
void FortuneEventTree<T>::printinorder(EventNode*& node)
{
	if (node != NULL)
	{
		printinorder(node->left);
		std::cout << node->value << ", ";
		printinorder(node->right);
	}
}
/*print level order*/
template <class T>
void FortuneEventTree<T>::printlevelorder()
{
	if (root != NULL)
		printlevelorder(root);
	std::cout << std::endl;
}
template <class T>
void FortuneEventTree<T>::printlevelorder(EventNode*& node)
{
	if (node == nullptr)
	{
		return;
	}

	std::queue<EventNode*> QUE;

	EventNode* temp = new EventNode();
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
		std::cout << QUE.front()->value << " ";
		QUE.pop();
	}
}
/*print level order*/
template <class T>
void FortuneEventTree<T>::printnodeinfo()
{
	if (root != NULL)
		printnodeinfo(root);
	std::cout << std::endl;
}
template <class T>
void FortuneEventTree<T>::printnodeinfo(EventNode*& node)
{
	if (node == nullptr)
	{
		return;
	}

	std::queue<EventNode*> QUE;

	EventNode* temp = new EventNode();
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
		QUE.pop();
	}
}
/*poptop*/
template <class T>
T FortuneEventTree<T>::poptop()
{
	EventNode* tmp = root;
	while (tmp->left != NULL)
	{
		tmp = tmp->left;
	}
	T top = tmp->value;
	remove(root, top);
	return top;
}
/*clear tree*/
template <class T>
void FortuneEventTree<T>::cleartree()
{
	if (root != NULL)
		cleartree(root);
}
template <class T>
void FortuneEventTree<T>::cleartree(EventNode*& node)
{
	if (node != NULL)
	{
		cleartree(node->left);
		cleartree(node->right);
		delete node;
		node = NULL;
	}
}
/*balance check*/
template <class T>
bool FortuneEventTree<T>::IsTreeBalanced()
{
	if (root != NULL)
		return IsTreeBalanced(root);
	return true;
}
template <class T>
bool FortuneEventTree<T>::IsTreeBalanced(EventNode*& node)
{
	if (node != NULL)
	{
		if (!IsTreeBalanced(node->left))
		{
			return false;
		}
		if (!IsTreeBalanced(node->right))
		{
			return false;
		}
		if (abs(getBalance(node)) > 1)
		{
			return false;
		}
	}
	return true;
}
#endif