//***class taken from https://www.cppbuzz.com/programs/c++/implementation-of-binary-tree-using-template-in-c++
//BinaryTree.h
// Binary Tree Template
#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class BinaryTree
{
public:
	struct TreeNode
	{
		T value;
		TreeNode *left;
		TreeNode *right;
	};
	TreeNode *root;
	void insert(TreeNode *&, TreeNode *&);
	void destroySubTree(TreeNode *);
	void deleteNode(T, TreeNode *&);
	void deleteNodedata(T, TreeNode *&);
	void swapnodewithvalue(T, TreeNode *&);
	int findrightneighbor(T, TreeNode *&, TreeNode *&, TreeNode *&);
    int findleftneighbor(T num, TreeNode *& nodePtr, TreeNode *& parent, TreeNode *& grandparent);
	void makeDeletion(TreeNode *&);
	void getlistinorder(TreeNode *, vector<T>& list);
	void displayInOrder(TreeNode *);
	void displayPreOrder(TreeNode *);
	void displayPostOrder(TreeNode *);
	T grableastvalue(TreeNode *);
	int countNodes(TreeNode *&);
public:
	BinaryTree() // Constructor
	{
		root = NULL;
	}
	~BinaryTree() // Destructor
	{
		if (root)
		{
			destroySubTree(root);
		}
	}
	void insertNode(T);
	bool searchNode(T);
	void remove(T);
	void removedata(T);
	void swapnodewithvalue(T, T);
	T getnode(T);
	T findrightneighbor(T);
	T findleftneighbor(T);
	vector<T> getlistinorder()
	{
		vector<T> list;
		getlistinorder(root, list);
		return list;
	}
	void displayInOrder()
	{
		if (root)
		{
			cout <<"root: "<< root->value << endl;
		}
		displayInOrder(root);
	}
	void displayPreOrder()
	{
		displayPreOrder(root);
	}
	void displayPostOrder()
	{
		displayPostOrder(root);
	}
	T grableastvalue()
	{
		return grableastvalue(root);
	}
	int numNodes();
};


//*************************************************************
// insert accepts a TreeNode pointer and a pointer to a node. The function inserts the *
// node into the tree pointed to by the TreeNode pointer. This function is called recursively. *
//*************************************************************
template <class T>
void BinaryTree<T>::insert(TreeNode *&nodePtr, TreeNode *&newNode)
{
	if (nodePtr == NULL)
	{
		// Insert the node.
		nodePtr = newNode;
	}
	else if (newNode->value == nodePtr->value)
	{
		delete newNode;
		return;
	}
	else if (newNode->value < nodePtr->value)
	{
		// Search the left branch
		insert(nodePtr->left, newNode);
	}
	else
	{
		// Search the right branch
		insert(nodePtr->right, newNode);
	}
}
//**********************************************************
// insertNode creates a new node to hold num as its value, *
// and passes it to the insert function. *
//**********************************************************
template <class T>
void BinaryTree<T>::insertNode(T num)
{
	TreeNode *newNode = NULL; // Pointer to a new node.
	// Create a new node and store num in it.
	newNode = new TreeNode;
	newNode->value = num;
	newNode->left = newNode->right = NULL;
	// Insert the node.
	insert(root, newNode);
}
//***************************************************


// destroySubTree is called by the destructor. It *
// deletes all nodes in the tree. *
//***************************************************
template <class T>
void BinaryTree<T>::destroySubTree(TreeNode *nodePtr)
{
	if (nodePtr != NULL)
	{
		if (nodePtr->left)
		{
			destroySubTree(nodePtr->left);
		}
		if (nodePtr->right)
		{
			destroySubTree(nodePtr->right);
		}
		delete nodePtr;
	}
}

template <class T>
void BinaryTree<T>::swapnodewithvalue(T, TreeNode *&)
{

}
template <class T>
void BinaryTree<T>::swapnodewithvalue(T num, T newnum)
{
	TreeNode *nodePtr = root;
	while (nodePtr)
	{
		if (nodePtr->value == num)
		{
			cout << "swapped successfuly" << endl;
			nodePtr->value = newnum;
			return;
		}
		else if (num < nodePtr->value)
		{
			nodePtr = nodePtr->left;
		}
		else
		{
			nodePtr = nodePtr->right;
		}
	}
}
template <class T>
T BinaryTree<T>::getnode(T num)
{
	TreeNode *nodePtr = root;
	while (nodePtr)
	{
		if (nodePtr->value == num)
		{
			return nodePtr->value;
		}
		else if (num < nodePtr->value)
		{
			nodePtr = nodePtr->left;
		}
		else
		{
			nodePtr = nodePtr->right;
		}
	}
}
//***************************************************
// searchNode determines if a value is present in the tree. If so, *
// the function returns true. Otherwise, it returns false. *
//***************************************************
template <class T>
bool BinaryTree<T>::searchNode(T num)
{
	bool status = false;
	TreeNode *nodePtr = root;
	while (nodePtr)
	{
		if (nodePtr->value == num)
		{
			status = true;
		}
		else if (num < nodePtr->value)
		{
			nodePtr = nodePtr->left;
		}
		else
		{
			nodePtr = nodePtr->right;
		}
	}
	return status;
}
//**********************************************
// remove calls deleteNode to delete the *
// node whose value member is the same as num. *
//**********************************************
template <class T>
void BinaryTree<T>::remove(T num)
{
	deleteNode(num, root);
}
template <class T>
void BinaryTree<T>::removedata(T num)
{
	deleteNodedata(num, root);
}

template <class T>
T BinaryTree<T>::findrightneighbor(T num)
{
	TreeNode* parent = NULL;
	TreeNode* grandparent = NULL;
	TreeNode* start = root;
	if (start)
	{
		T lol;
		int d = findrightneighbor(num, start, parent, grandparent);
		switch (d)
		{
		case 0: return start->right->value;
			break;
		case 1: return parent->value;
			break;
		case 2: return grandparent->value;
			break;
		case 3: return lol;
			break;
		}
	}
	T tmp;
	return tmp;
}

template <class T>
int BinaryTree<T>::findrightneighbor(T num, TreeNode *&nodePtr, TreeNode *& parent, TreeNode *& grandparent)
{
	if (num < nodePtr->value)
	{
		cout << "left";
		grandparent = parent;
		parent = nodePtr;
		nodePtr = nodePtr->left;
		findrightneighbor(num, nodePtr, parent, grandparent);
	}
	else if (num > nodePtr->value)
	{
		cout << "right";
		grandparent = parent;
		parent = nodePtr;
		nodePtr = nodePtr->right;
		findrightneighbor(num, nodePtr, parent, grandparent);
	}
	else
	{
		cout << "same";
		if (nodePtr->right)
		{
			cout << "rightchild";
			return 0;
		}
		if (parent)
		{
			if (num < parent->value)
			{
				cout << "parent";
				return 1;
			}

		}
		if (grandparent)
		{
			if (num < grandparent->value)
			{
				cout << "grandparent";
				return 2;
			}

		}
		
		T tmp;
		return 3;
	}
}

template <class T>
T BinaryTree<T>::findleftneighbor(T num)
{
	TreeNode* parent = NULL;
	TreeNode* grandparent = NULL;
	TreeNode* start = root;
	if (start)
	{
		T lol;
		int d = findleftneighbor(num, start, parent, grandparent);
		switch(d)
		{
		case 0: return start->left->value;
			break;
		case 1: return parent->value;
			break;
		case 2: return grandparent->value;
			break;
		case 3: return lol; 
			break;
		}
	}
	T tmp;
	return tmp;
}

template <class T>
int BinaryTree<T>::findleftneighbor(T num, TreeNode *&nodePtr, TreeNode*& parent, TreeNode*& grandparent)
{//root doesnt have right neighbor
	if (num < nodePtr->value)
	{
		cout << "left";
		grandparent = parent;
		parent = nodePtr;
		nodePtr = nodePtr->left;
		findleftneighbor(num, nodePtr, parent, grandparent);
	}
	else if (num > nodePtr->value)
	{
		cout << "right";
		grandparent = parent;
		parent = nodePtr;
		nodePtr = nodePtr->right;
		//cout << parent->value << endl;
		findleftneighbor(num, nodePtr, parent, grandparent);
	}
	else
	{
		cout << "same";
		if (nodePtr->left)
		{
			return 0;
		}
		if (parent)
		{
			if (num > parent->value)
			{
				return 1;
			}
		}
		if (grandparent)
		{
			if (num > grandparent->value)
			{
				return 2;
			}

		}
		cout<<"none";
		T tmp;
		return 3;
	}
}
//********************************************
// deleteNode deletes the node whose value *
// member is the same as num. *
//********************************************
template <class T>
void BinaryTree<T>::deleteNode(T num, TreeNode *&nodePtr)
{
	if (num < nodePtr->value)
	{
		deleteNode(num, nodePtr->left);
	}
	else if (num > nodePtr->value)
	{
		deleteNode(num, nodePtr->right);
	}
	else
	{
		makeDeletion(nodePtr);
	}
}

template <class T>
void BinaryTree<T>::deleteNodedata(T num, TreeNode *& nodePtr)
{
	if (num < nodePtr->value)
	{
		deleteNode(num, nodePtr->left);
	}
	else if (num > nodePtr->value)
	{
		deleteNode(num, nodePtr->right);
	}
	else
	{
		makeDeletion(nodePtr);
	}
}
//***********************************************************
// makeDeletion takes a reference to a pointer to the node that is to be deleted. *
// The node is removed and the branches of the tree below the node are reattached. *
//***********************************************************
template <class T>
void BinaryTree<T>::makeDeletion(TreeNode *&nodePtr)
{
	//cout << "\n\nNow deleting " << nodePtr->value << " from the tree...." << endl;
	// Temporary pointer, used in reattaching the left subtree.
	TreeNode *tempNodePtr = NULL;
	if (nodePtr == NULL)
	{
		cout << "Cannot delete empty node.\n";
	}
	else if (nodePtr->right == NULL)
	{
		tempNodePtr = nodePtr;
		nodePtr = nodePtr->left; // Reattach the left child
		delete tempNodePtr;
	}
	else if (nodePtr->left == NULL)
	{
		tempNodePtr = nodePtr;
		nodePtr = nodePtr->right; // Reattach the right child
		delete tempNodePtr;
	}
	// If the node has two children.
	else
	{
		// Move one node the right.
		tempNodePtr = nodePtr->right;
		// Go to the end left node.
		while (tempNodePtr->left)
		{
			tempNodePtr = tempNodePtr->left;
		}
		// Reattach the left subtree.
		tempNodePtr->left = nodePtr->left;
		tempNodePtr = nodePtr;
		// Reattach the right subtree.
		nodePtr = nodePtr->right;
		delete tempNodePtr;
	}
}
//****************************************************************
template <class T>
T BinaryTree<T>::grableastvalue(TreeNode *nodePtr)
{
	while (nodePtr->left)
	{
		nodePtr = nodePtr->left;
	}
	return nodePtr->value;
}

template <class T>
void BinaryTree<T>::getlistinorder(TreeNode *nodePtr, vector<T>& list)
{
	if (nodePtr)
	{
		getlistinorder(nodePtr->left,list);
		list.push_back(nodePtr->value);
		getlistinorder(nodePtr->right,list);
	}
}
//****************************************************************
// The displayInOrder member function displays the values *
// in the subtree pointed to by nodePtr, via inorder traversal. *
//****************************************************************
template <class T>
void BinaryTree<T>::displayInOrder(TreeNode *nodePtr)
{
	if (nodePtr)
	{
		displayInOrder(nodePtr->left);
		cout << nodePtr->value << " ";
		displayInOrder(nodePtr->right);
	}
}
//****************************************************************
// The displayPreOrder member function displays the values *
// in the subtree pointed to by nodePtr, via preorder traversal. *
//****************************************************************
template <class T>
void BinaryTree<T>::displayPreOrder(TreeNode *nodePtr)
{
	if (nodePtr)
	{
		cout << nodePtr->value << " ";
		displayPreOrder(nodePtr->left);
		displayPreOrder(nodePtr->right);
	}
}
//****************************************************************
// The displayPostOrder member function displays the values *
// in the subtree pointed to by nodePtr, via postorder traversal.*
//****************************************************************
template <class T>
void BinaryTree<T>::displayPostOrder(TreeNode *nodePtr)
{
	if (nodePtr)
	{
		displayPostOrder(nodePtr->left);
		displayPostOrder(nodePtr->right);
		cout << nodePtr->value << " ";
	}

}
//****************************************************************
// The numNodes function returns the number of nodes in the tree.*
//****************************************************************
template <class T>
int BinaryTree<T>::numNodes()
{
	return countNodes(root);
}
//****************************************************************
// The countNodes function uses recursion to count the nodes in the tree.
// This function is called by the public member function numNodes.
//****************************************************************
template <class T>
int BinaryTree<T>::countNodes(TreeNode *&nodePtr)
{
	int count;
	if (nodePtr == NULL)
	{
		count = 0;
	}
	else
	{
		count = 1 + countNodes(nodePtr->left) + countNodes(nodePtr->right);
	}
	return count;
}
#endif