/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*	Name: Shady Boukhary
*	Date: March 1st, 2018
*	CMPS 3013 Advanced Algorithms and Analysis
*	Dr. Griffin
*	This program reads 5 files filled with words, inserts them into a binary
*	search tree and an AVL tree. Then, it reads 10,000 randomly generated
*	words from a file and looks the components
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#include <iostream>
#include <fstream>
#include "AVLTree.h"
#include "BSTree.h"
#include <map>
#include <sstream>
using namespace std;

// Opens an input file.
ifstream openInputFile(string file_name);

// reads input files into arrays
int readFile(string, BSTree&, avlTree&, string type);

// reads crazy words file and search
void readCrazy(char**, int& compBS, int& compAVL, BSTree& bstree, avlTree& avltree, int[]);



int main(int argc, char **argv)
{
	BSTree bsTree;
	avlTree avltree;
	ofstream outfile;
	int types[5] = { 0 };
	int compBS = 0;
	int compAVL = 0;
	if (argc < 2) {
		cout << "You need an input file!\n";
		cout << "Usage: analyze.exe filename.txt";
		exit(0);
	}
	string file = argv[1];
	// read all input files
	int adjCount = readFile("adjectives.txt", bsTree, avltree, "adjective");
	int advCount = readFile("adverbs.txt", bsTree, avltree, "adverb");
	int animalCount = readFile("animals.txt", bsTree, avltree, "animal");
	int nounCount = readFile("nouns.txt", bsTree, avltree, "noun");
	int verbCount = readFile("verbs.txt", bsTree, avltree, "verb");
	readCrazy(argv, compBS, compAVL, bsTree, avltree, types);

	outfile.open("analysis.out");
	
	// print analysis report to file
	outfile << "Number of each type based on Tree comparisons (frequency of each type in crazy words): \n\n";
	outfile << "Number of Adjectives: " << types[0]
		<< "\nNumber of adverbs: " << types[1]
		<< "\nNumber of Animals: " << types[2]
		<< "\nNumber of Nouns: " << types[3]
		<< "\nNumber of Verbs: " << types[4]
		<< "\n-------------------------------\n\n"

		<< "Actual number of each type present in the tree (words loaded from files)  \n\n"
		<< "Number of Adjectives: " << adjCount
		<< "\nNumber of adverbs: " << advCount
		<< "\nNumber of Animals: " << animalCount
		<< "\nNumber of Nouns: " << nounCount
		<< "\nNumber of Verbs: " << verbCount
		<< "\n-------------------------------\n";

	outfile << "\n\nComparisons BST: " << compBS << endl;
	outfile << "Comparisons AVL: " << compAVL << endl;
	outfile.close();

	return 0;
}


/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* Opens an input file.
* @usage: myfile = openInputFile("input.txt");
* @param: string
* @return: ifstream
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
ifstream openInputFile(string file_name) {

	ifstream myfile;
	try
	{
		myfile.open(file_name.c_str());
		if (!myfile.is_open())
			throw string("Unable to open file. Terminating\n");
	}
	catch (string message)
	{
		cout << message;
		system("pause");
		exit(1);
	}
	return myfile;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* reads input file into a BSTree and AVLTree
* @usage: numerous
* @param: string, BSTree&, avlTree&
* @return: int
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int readFile(string file, BSTree& bsTree, avlTree& avltree, string type)
{
	ifstream infile = openInputFile(file);
	map<string, int> wordMap;
	string word;

	int x = 0;
	while (!infile.eof())
	{
		infile >> word;

		if (wordMap.find(word) == wordMap.end()) {
			wordMap[word] = 1;
			bsTree.insert(word, type);
			avltree.insert(word, type);
			x++;
		}
	}

	infile.close();
	return x;

}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* reads input file of crazyWords and searches for components in trees
* @usage: readCrazy(argv, compBS, compAVL, bsTree, avltree);
* @param: char**, int&, int&, BSTree&, avlTree&
* @return: void
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void readCrazy(char ** file, int& compBS, int& compAVL, BSTree& bstree, avlTree& avltree, int types[])
{
	ifstream infile = openInputFile(file[1]);
	map<string, int> wordMap;
	string word;
	string typeBS, typeAVL;
	while (!infile.eof())
	{
		infile >> word;
		typeBS = bstree.search(word);
		typeAVL = avltree.search(word);
		// increment counter for word type
		if (typeBS == "adjective")
			types[0]++;
		else if (typeBS == "adverb")
			types[1]++;
		else if (typeBS == "animal")
			types[2]++;
		else if (typeBS == "noun")
			types[3]++;
		// verb
		else 
			types[4]++;
	}
	compBS = bstree.comparisons;
	compAVL = avltree.comparisons;
	infile.close();
}

