#include <iostream>
#include <fstream>
#include <time.h>
#include "avlTree.h"

//https://visualgo.net/en/bst

using namespace std;

avlTree::avlTree() {
	root = NULL;
}

avlTree::~avlTree() {

}

//************************************************************************
// Method Name: insert
//
// Private 
//
// Purpose: Inserts a node into a binary tree
//
// Arguments: reference to the root, and a reference to the new node
//
// Returns: Nothing.
//*************************************************************************
void avlTree::insert(nodeAVL *&nodePtr, nodeAVL *&newNode) {

	if (nodePtr == NULL) {
		nodePtr = newNode;
	}
	else if (newNode->value <= nodePtr->value) {
		newNode->parent = nodePtr;
		insert(nodePtr->left, newNode);

	}
	else {
		newNode->parent = nodePtr;
		insert(nodePtr->right, newNode);
	}

}

//************************************************************************
// Method Name: insertNode
//
// Public 
//
// Purpose: Creates a new node and assigns the passed in value to it. Subsequently
//          calls insert to actually travers the binary tree and link the node in
//          proper location. 
//
// Arguments: integer to be placed in binary tree.
//
// Returns: Nothing.
//*************************************************************************
void avlTree::insert(string word, string t) {
	nodeAVL *newNode;

	newNode = new nodeAVL(word, t);
	insert(root, newNode);
	computeAvlValues(root);
}

//************************************************************************
// Method Name: inorder,postorder,preorder (all the same)
//
// Public 
//
// Purpose: Traverses a binary tree 
//
// Arguments: integer to be placed in binary tree.
//
// Returns: Nothing.
//*************************************************************************
void avlTree::inorder(nodeAVL *nodePtr) {
	if (nodePtr) {
		inorder(nodePtr->left);
		cout << nodePtr->value << " " << "(" << nodePtr->avlValue << ") ";
		inorder(nodePtr->right);
	}
}

void avlTree::postorder(nodeAVL *nodePtr) {
	if (nodePtr) {
		inorder(nodePtr->left);
		inorder(nodePtr->right);
		cout << nodePtr->value << " " << "(" << nodePtr->avlValue << ") ";
	}
}

void avlTree::preorder(nodeAVL *nodePtr) {
	if (nodePtr) {
		cout << nodePtr->value << " " << "(" << nodePtr->avlValue << ") ";
		inorder(nodePtr->left);
		inorder(nodePtr->right);
	}
}

//************************************************************************
// Method Name: searchNode
//
// Public 
//
// Purpose: Traverses a binary tree looking for a key value
//
// Arguments: integer to look for
//
// Returns: type of word if found, empty string if not found 
//*************************************************************************
string avlTree::search(string word) {
	nodeAVL *nodePtr = root;

	while (nodePtr) {
		if (nodePtr->value == word)
			return nodePtr->type;
		else if (word < nodePtr->value)
		{
			comparisons++;
			nodePtr = nodePtr->left;
		}
		else
		{
			comparisons++;
			nodePtr = nodePtr->right;
		}
	}
	return "";
}


//************************************************************************
// Method Name: remove
//
// Private 
//
// Purpose: Actually removes a node from a tree by pointer manipulation and
//          frees the memory
//
// Arguments: address of nodeAVL to be deleted
//
// Returns: void 
//*************************************************************************

nodeAVL* avlTree::remove(nodeAVL*& root, string key)
{
	if (!root)
	{
		return NULL;
	}
	if (key < root->value)
	{
		cout << "going left" << endl;
		root->left = remove(root->left, key);
	}
	else if (key > root->value)
	{
		cout << "going right" << endl;
		root->right = remove(root->right, key);
	}
	else
	{
		if (root->left == NULL)
		{
			nodeAVL *temp = root->right;
			delete root;
			return temp;
		}
		else if (root->right == NULL)
		{
			nodeAVL *temp = root->left;
			delete root;
			return temp;
		}

		// nodeAVL with two children: Get the inorder successor (smallest
		// in the right subtree)
		nodeAVL *temp = predSuccessor(root);

		printNode(temp, "predSuccessor");

		// Copy the inorder successor's content to this nodeAVL
		root->value = temp->value;

		// Delete the inorder successor
		root->right = remove(root->right, temp->value);
	}
	return root;
}

void avlTree::printNode(nodeAVL *n, string label = "")
{
	if (label != "")
	{
		cout << "[" << label << "]";
	}
	cout << "[[" << n << "][" << n->value << "]]\n";
	if (n->left)
	{
		cout << "\t|-->[L][[" << n->left << "][" << n->left->value << "]]\n";
	}
	else
	{
		cout << "\t\\-->[L][null]\n";
	}
	if (n->right)
	{
		cout << "\t\\-->[R][[" << n->right << "][" << n->right->value << "]]\n";
	}
	else
	{
		cout << "\t\\-->[R][null]\n";
	}
}

nodeAVL* avlTree::predSuccessor(nodeAVL *root)
{
	nodeAVL *current = root;

	if (root->right)
	{
		current = root->right;
		while (current->left != NULL)
		{
			current = current->left;
		}
	}
	else if (root->left)
	{
		current = root->left;
		while (current->right != NULL)
		{
			current = current->right;
		}
	}

	return current;
}
//************************************************************************
// Method Name: height
//
// Private 
//
// Purpose: Actually removes a nodeAVL from a tree by pointer manipulation and
//          frees the memory
//
// Arguments: address of the root of the tree (or subtree)
//
// Returns: void 
//*************************************************************************
int avlTree::height(nodeAVL *nodeAVLPtr) {
	int left_height = 0;
	int right_height = 0;
	if (nodeAVLPtr == NULL)
		return 0;
	else {
		left_height = height(nodeAVLPtr->left);
		right_height = height(nodeAVLPtr->right);
	}
	if (left_height>right_height)
		return 1 + left_height;
	else
		return 1 + right_height;
}

//************************************************************************
// Method Name: treeHeight
//
// Public 
//
// Purpose: Public method to call the private height method
//
// Arguments: none
//
// Returns: void 
//*************************************************************************
int avlTree::treeHeight() {
	return height(root);
}

//************************************************************************
// Method Name: avlValue
//
// Private 
//
// Purpose: Private method to calculate the avl value of a nodeAVL.
//
// Arguments: address of a nodeAVL
//
// Returns: void 
//*************************************************************************
int avlTree::avlValue(nodeAVL *nodeAVLPtr)
{
	return height(nodeAVLPtr->right) - height(nodeAVLPtr->left);
}

//************************************************************************
// Method Name: computeAvlValues
//
// Private 
//
// Purpose: Private method to calculate the avl values of the entire tree.
//
// Arguments: address of a nodeAVL
//
// Returns: void 
//*************************************************************************
void avlTree::computeAvlValues(nodeAVL *&nodeAVLPtr)
{
	if (nodeAVLPtr) {
		computeAvlValues(nodeAVLPtr->left);
		computeAvlValues(nodeAVLPtr->right);
		nodeAVLPtr->avlValue = avlValue(nodeAVLPtr);
		if (nodeAVLPtr->avlValue > 1) {
			rotateLeft(nodeAVLPtr);
		}
		else if (nodeAVLPtr->avlValue < -1) {
			rotateRight(nodeAVLPtr);
		}
	}
}

//************************************************************************
// Method Name: rotateLeft 
//
// Private 
//
// Purpose: Private method to perform a left rotation from a given position in a tree
//
// Arguments: address of a nodeAVL
//
// Returns: void 
//*************************************************************************
void avlTree::rotateLeft(nodeAVL *&SubRoot)
{


	if (leftHeavy(SubRoot->right)) {
		rotateRight(SubRoot->right);
	}

	nodeAVL *Temp;

	Temp = SubRoot->right;
	SubRoot->right = Temp->left;
	Temp->left = SubRoot;
	SubRoot = Temp;

	computeAvlValues(SubRoot);
}

//************************************************************************
// Method Name: rotateLeft 
//
// Private 
//
// Purpose: Private method to perform a right rotation from a given position in a tree
//
// Arguments: address of a nodeAVL
//
// Returns: void 
//*************************************************************************
void avlTree::rotateRight(nodeAVL *&SubRoot)
{
	if (rightHeavy(SubRoot->left)) {
		rotateLeft(SubRoot->left);
	}

	nodeAVL *Temp;

	Temp = SubRoot->left;
	SubRoot->left = Temp->right;
	Temp->right = SubRoot;
	SubRoot = Temp;

	computeAvlValues(SubRoot);
}

//************************************************************************
// Method to help create GraphViz code so the expression tree can 
// be visualized. This method prints out all the unique nodeAVL id's
// by traversing the tree.
// Recivies a nodeAVL pointer to root and performs a simple recursive 
// tree traversal.
//************************************************************************
void avlTree::graphVizGetIds(nodeAVL *nodePtr, ofstream &VizOut) {
	static int NullCount = 0;
	if (nodePtr) {
		graphVizGetIds(nodePtr->left, VizOut);
		VizOut << "node" << nodePtr->value
			<< "[label=\"" << nodePtr->value << "\\n"
			<< "Avl:" << nodePtr->avlValue << "\\n"
			//<<"Add:"<<nodePtr<<"\\n"
			//<<"Par:"<<nodePtr->parent<<"\\n"
			//<<"Rt:"<<nodePtr->right<<"\\n"
			//<<"Lt:"<<nodePtr->left<<"\\n"
			<< "\"]" << endl;
		if (!nodePtr->left) {
			NullCount++;
			VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
		}
		graphVizGetIds(nodePtr->right, VizOut);
		if (!nodePtr->right) {
			NullCount++;
			VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;

		}
	}
}

//************************************************************************
// This method is partnered with the above method, but on this pass it 
// writes out the actual data from each node.
// Don't worry about what this method and the above method do, just
// use the output as your told:)
//************************************************************************
void avlTree::graphVizMakeConnections(nodeAVL *nodePtr, ofstream &VizOut) {
	static int NullCount = 0;
	if (nodePtr) {
		graphVizMakeConnections(nodePtr->left, VizOut);
		if (nodePtr->left)
			VizOut << "node" << nodePtr->value << "->" << "node" << nodePtr->left->value << endl;
		else {
			NullCount++;
			VizOut << "node" << nodePtr->value << "->" << "nnode" << NullCount << endl;
		}

		if (nodePtr->right)
			VizOut << "node" << nodePtr->value << "->" << "node" << nodePtr->right->value << endl;
		else {
			NullCount++;
			VizOut << "node" << nodePtr->value << "->" << "nnode" << NullCount << endl;
		}

		graphVizMakeConnections(nodePtr->right, VizOut);

	}
}

//************************************************************************
// Recieves a filename to place the GraphViz data into.
// It then calls the above two graphviz methods to create a data file
// that can be used to visualize your expression tree.
//************************************************************************
void avlTree::graphVizOut(string filename)
{
	ofstream VizOut;
	VizOut.open(filename);
	graphVizGetIds(root, VizOut);
	graphVizMakeConnections(root, VizOut);
	VizOut.close();

}

//************************************************************************
// Method Name: doDumpTree 
//
// Private 
//
// Purpose: Private method to show the basic pointer structure of the tree. 
//          Initially written to help with debugging.
//
// Arguments: address of a node
//
// Returns: void 
// Outputs: tree information
//*************************************************************************
void avlTree::doDumpTree(nodeAVL *nodePtr)
{
	if (nodePtr) {
		cout << "Add:    " << nodePtr << "\n"
			<< "Parent->" << nodePtr->parent << "\n"
			<< "Val:    " << nodePtr->value << "\n"
			<< "Avl:    " << nodePtr->avlValue << "\n"
			<< "Left->  " << nodePtr->left << "\n"
			<< "Right-> " << nodePtr->right << "\n\n";

		doDumpTree(nodePtr->left);
		doDumpTree(nodePtr->right);
	}
}

//************************************************************************
// Method Name: leftHeavy,rightHeavy 
//
// Private 
//
// Purpose: Compares the subtrees of a node to see which is taller
//
// Arguments: address of a node
//
// Returns: true if (left/right) heavy 
//
//*************************************************************************
bool avlTree::leftHeavy(nodeAVL *nodePtr)
{
	return height(nodePtr->left)>height(nodePtr->right);
}

bool avlTree::rightHeavy(nodeAVL *nodePtr)
{
	return height(nodePtr->right)>height(nodePtr->left);

}