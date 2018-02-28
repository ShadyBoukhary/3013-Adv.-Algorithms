#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

using namespace std;

struct nodeAVL {
	string value;
	string type;
	nodeAVL *left;
	nodeAVL *right;
	nodeAVL *parent;
	int avlValue;
	nodeAVL(string word, string t) {
		value = word;
		type = t;
		left = right = parent = NULL;
		avlValue = 0;
	}

};

class avlTree {
private:
	nodeAVL *root;
	bool rightHeavy(nodeAVL *);
	bool leftHeavy(nodeAVL *);
	void insert(nodeAVL *&, nodeAVL *&);
	void inorder(nodeAVL *);
	void preorder(nodeAVL *);
	void postorder(nodeAVL *);

	nodeAVL* remove(nodeAVL*&, string);
	nodeAVL* predSuccessor(nodeAVL*);

	void printNode(nodeAVL *, string);
	int  height(nodeAVL *);
	void computeAvlValues(nodeAVL *&);
	void rotateLeft(nodeAVL *&);
	void rotateRight(nodeAVL *&);
	int  avlValue(nodeAVL *);

public:
	avlTree();
	~avlTree();
	void doDumpTree(nodeAVL *);
	void dumpTree() {
		cout << "---------------------------------" << endl;
		cout << "Root:   " << root << "\n";
		doDumpTree(root);
	};

	void insert(string, string);
	void showInorder() { inorder(root); };
	void showPreorder() { preorder(root); };
	void showPostorder() { postorder(root); };
	string search(string);
	void remove(string word) { root = remove(root, word); };
	int  treeHeight();
	void graphVizGetIds(nodeAVL *, ofstream &);
	void graphVizMakeConnections(nodeAVL *, ofstream &);
	void graphVizOut(string);
	int comparisons = 0;

};
