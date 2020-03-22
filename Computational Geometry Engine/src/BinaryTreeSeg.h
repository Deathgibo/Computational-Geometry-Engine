//***class taken from https://www.cppbuzz.com/programs/c++/implementation-of-binary-tree-using-template-in-c++
//BinaryTreeSeg.h
// Binary Tree Template

#ifndef BinaryTreeSeg_H
#define BinaryTreeSeg_H
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class BinaryTreeSeg
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
	void insertequal(TreeNode *&, TreeNode *&);
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
	BinaryTreeSeg() // Constructor
	{
		root = NULL;
	}
	~BinaryTreeSeg() // Destructor
	{
		if (root)
		{
			destroySubTree(root);
		}
	}
	void insertNode(T);
	void insertNodeequal(T);
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
			cout << "root: " << root->value << endl;
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
void BinaryTreeSeg<T>::insert(TreeNode *&nodePtr, TreeNode *&newNode)
{
	if (nodePtr == NULL)
	{
		// Insert the node.
		nodePtr = newNode;
	}
	else if (newNode->value < nodePtr->value)
	{
		// Search the left branch
		insert(nodePtr->left, newNode);
	}
	else if (newNode->value > nodePtr->value)
	{
		// Search the right branch
		insert(nodePtr->right, newNode);
	}
	else
	{
		//same so to right
		insert(nodePtr->right, newNode);
	}
}
template <class T>
void BinaryTreeSeg<T>::insertequal(TreeNode *&nodePtr, TreeNode *&newNode)
{
	if (nodePtr == NULL)
	{
		// Insert the node.
		nodePtr = newNode;
	}
	else if (newNode->value < nodePtr->value)
	{
		// Search the left branch
		insert(nodePtr->left, newNode);
	}
	else if (newNode->value > nodePtr->value)
	{
		// Search the right branch
		insert(nodePtr->right, newNode);
	}
	else
	{
		cout << "point same when no reinserting" << endl;
		exit(0);
	}
}
//**********************************************************
// insertNode creates a new node to hold num as its value, *
// and passes it to the insert function. *
//**********************************************************
template <class T>
void BinaryTreeSeg<T>::insertNode(T num)
{
	TreeNode *newNode = NULL; // Pointer to a new node.
	// Create a new node and store num in it.
	newNode = new TreeNode;
	newNode->value = num;
	newNode->left = newNode->right = NULL;
	// Insert the node.
	insert(root, newNode);
}
template <class T>
void BinaryTreeSeg<T>::insertNodeequal(T num)
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
void BinaryTreeSeg<T>::destroySubTree(TreeNode *nodePtr)
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
void BinaryTreeSeg<T>::swapnodewithvalue(T, TreeNode *&)
{

}
template <class T>
void BinaryTreeSeg<T>::swapnodewithvalue(T num, T newnum)
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
T BinaryTreeSeg<T>::getnode(T num)
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
bool BinaryTreeSeg<T>::searchNode(T num)
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
void BinaryTreeSeg<T>::remove(T num)
{
	deleteNode(num, root);
}
template <class T>
void BinaryTreeSeg<T>::removedata(T num)
{
	deleteNodedata(num, root);
}

template <class T>
T BinaryTreeSeg<T>::findleftneighbor(T num)
{
	vector<T> list = getlistinorder();
	int index = -1;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == num)
		{
			index = i;
		}
	}
	if (index == -1)
	{
		cout << "rightneighbor error";
	}
	if (index > 0)
	{
		return list[index - 1];
	}
	T dummy;
	return dummy;
}

template <class T>
int BinaryTreeSeg<T>::findrightneighbor(T num, TreeNode *&nodePtr, TreeNode *& parent, TreeNode *& grandparent)
{

}

template <class T>
T BinaryTreeSeg<T>::findrightneighbor(T num)
{
	vector<T> list = getlistinorder();
	int index = -1;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == num)
		{
			index = i;
		}
	}
	if (index == -1)
	{
		cout << "rightneighbor error";
	}
	if (index < list.size()-1)
	{
		return list[index + 1];
	}
	T dummy;
	return dummy;
}

template <class T>
int BinaryTreeSeg<T>::findleftneighbor(T num, TreeNode *&nodePtr, TreeNode*& parent, TreeNode*& grandparent)
{
	
}
//********************************************
// deleteNode deletes the node whose value *
// member is the same as num. *
//********************************************
template <class T>
void BinaryTreeSeg<T>::deleteNode(T num, TreeNode *&nodePtr)
{
	//assert(nodePtr, "Error in deleting, the T structure tried to delete and couldn't find node");
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
void BinaryTreeSeg<T>::deleteNodedata(T num, TreeNode *& nodePtr)
{
	//assert(nodePtr, "Error in deleting, the T structure tried to delete and couldn't find node");
	if (num < nodePtr->value)
	{
		//cout << "delete left";
		deleteNodedata(num, nodePtr->left);
	}
	else if (num > nodePtr->value)
	{
		//cout << "delete right";
		deleteNodedata(num, nodePtr->right);
	}
	else
	{
		//cout << "delete";
		makeDeletion(nodePtr);
	}
}
//***********************************************************
// makeDeletion takes a reference to a pointer to the node that is to be deleted. *
// The node is removed and the branches of the tree below the node are reattached. *
//***********************************************************
template <class T>
void BinaryTreeSeg<T>::makeDeletion(TreeNode *&nodePtr)
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
T BinaryTreeSeg<T>::grableastvalue(TreeNode *nodePtr)
{
	while (nodePtr->left)
	{
		nodePtr = nodePtr->left;
	}
	return nodePtr->value;
}

template <class T>
void BinaryTreeSeg<T>::getlistinorder(TreeNode *nodePtr, vector<T>& list)
{
	if (nodePtr)
	{
		getlistinorder(nodePtr->left, list);
		list.push_back(nodePtr->value);
		getlistinorder(nodePtr->right, list);
	}
}
//****************************************************************
// The displayInOrder member function displays the values *
// in the subtree pointed to by nodePtr, via inorder traversal. *
//****************************************************************
template <class T>
void BinaryTreeSeg<T>::displayInOrder(TreeNode *nodePtr)
{
	if (nodePtr)
	{
		displayInOrder(nodePtr->left);
		cout << "NODE: " << nodePtr->value << " ";
		/*cout << "LEFT: ";
		if (nodePtr->left)
			cout << nodePtr->left->value;
		else
			cout << endl;
		cout << "RIGHT: ";
		if (nodePtr->right)
			cout << nodePtr->right->value;
		else
			cout << endl;*/
		displayInOrder(nodePtr->right);
	}
}
//****************************************************************
// The displayPreOrder member function displays the values *
// in the subtree pointed to by nodePtr, via preorder traversal. *
//****************************************************************
template <class T>
void BinaryTreeSeg<T>::displayPreOrder(TreeNode *nodePtr)
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
void BinaryTreeSeg<T>::displayPostOrder(TreeNode *nodePtr)
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
int BinaryTreeSeg<T>::numNodes()
{
	return countNodes(root);
}
//****************************************************************
// The countNodes function uses recursion to count the nodes in the tree.
// This function is called by the public member function numNodes.
//****************************************************************
template <class T>
int BinaryTreeSeg<T>::countNodes(TreeNode *&nodePtr)
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
