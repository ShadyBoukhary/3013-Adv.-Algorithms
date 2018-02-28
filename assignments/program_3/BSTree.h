#pragma once
#include <string>
#include <fstream>
using namespace std;
struct node
{
	string data;
	string type;
	node *left;
	node *right;
	node()
	{
		data = -1;
		left = NULL;
		right = NULL;
	}
	node(string x, string t)
	{
		data = x;
		type = t;
		left = NULL;
		right = NULL;
	}
};

class BSTree
{
private:
	node *root;
	int count(node *);
	void insert(node *&, node *&);
	void print_node(node *, string);
	/**

	* type = ['predecessor','successor']

	*/
	node *minValueNode(node *);
	node *deleteNode(node *&, string);
	// node with two children: Get the inorder successor (smallest

	// in the right subtree)

	node *temp = minValueNode(root);
	int height(node *root);
	/* Print nodes at a given level */

	void printGivenLevel(node *, int);



	void GraphVizGetIds(node *nodePtr, ofstream &VizOut);

	void GraphVizMakeConnections(node *nodePtr, ofstream &VizOut);
public:
	int comparisons = 0;
	BSTree();
	~BSTree();
	int count();
	void insert(string x, string);
	void deleteNode(string key);
	void minValue();
	int height(string key);
	string top();
	/* Function to line by line print level order traversal a tree*/
	void printLevelOrder();
	void GraphVizOut(string filename);
	string search(string);
};