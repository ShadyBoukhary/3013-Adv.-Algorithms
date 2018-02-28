#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <map>

#define ADJ 15572
#define ADV 3238
#define NOUN 53738
#define VERBS 12019
#define ANM 235

using namespace std;

// Opens an input file.
ifstream openInputFile(string file_name);

// Opens an output file in truncate, then delete current content.
void initializeOutputFile();

// Opens an output file in append.
ofstream openOutputFile(string file_name);

// reads input files into arrays
void readFile(string, string *, int);

// Displays string and exports string into output file.
void exportString(string s, ofstream&);

// Prints the array to console
void printA(string *, int);

// Generates all words
void generateAllWords(string *, string *, string *, string *, string *);

// Generates a random sequence of word types to be used to generate a word
int getRandom(int, int);

// Generates a random word using the lists of words
string generateWord(string *, string *, string *, string *, string *, int);

int main()
{
	string * adjs = new string[ADJ];
	string * advs = new string[ADV];
	string * nouns = new string[NOUN];
	string * verbs = new string[VERBS];
	string * animals = new string[ANM];


	readFile("adjectives.txt", adjs, ADJ);
	readFile("adverbs.txt", advs, ADV);
	readFile("nouns.txt", nouns, NOUN);
	readFile("verbs.txt", verbs, VERBS);
	readFile("animals.txt", animals, ANM);

	initializeOutputFile();
	generateAllWords(adjs, advs, nouns, verbs, animals);

	delete[] adjs; 
	delete[] advs;
	delete[] nouns;
	delete[] verbs;
	delete[] animals;

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
* Displays string and exports string into output file.
* @usage: exportString("Hello World!");
* @param: string
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void exportString(string s, ofstream& myfile) {
	cout << s;
	myfile << s;
	//myfile.close();

}



/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* Opens an output file in truncate, then delete current content.
* @usage: initializeOutputFile();
* @param: string
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void initializeOutputFile() {
	string file_name = "tenthousandwords.txt";
	ofstream myfile;
	try
	{
		myfile.open(file_name, ios_base::trunc);
		if (!myfile)
			throw string("Unable to open output file.Terminating\n");
		else
			myfile.close();
	}
	catch (string error)
	{
		cout << error;
		system("pause");
		exit(1);
	}
}


/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* Opens an output file in append.
* @usage: myfile = openOutputFile("output.txt");
* @param: string
* @return: ofstream
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
ofstream openOutputFile(string file_name) {
	ofstream myfile;
	try
	{
		myfile.open(file_name, ios_base::app);
		if (!myfile)
			throw string("Unable to open output file. Terminating\n");
	}
	catch (string error)
	{
		cout << error;
		system("pause");
		exit(1);
	}
	return myfile;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* reads input file into an array
* @usage: readFile
* @param: string, string[], int
* @return: void
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void readFile(string fileName, string * words, int size)
{
	int x = 1;
	ifstream infile = openInputFile(fileName);
	infile >> words[0];
	while (!infile.eof() && x < size) {
		infile >> words[x];
		x++;
	}
	infile.close();
}

void printA(string * words, int size)
{
	for (int x = 0; x < size; x++) {
		cout << words[x] + "\n";
	}
}


void generateAllWords(string * adjs, string * advs, string * nouns, string * verbs,
	string * animals)
{
	srand((unsigned int)time(0));
	ofstream outfile = openOutputFile("tenthousandwords.txt");

	map<string, int> wordMap;
	map<string, int>::iterator it;

	for (int x = 0; x < 10000; x++) {
		int seq = getRandom(1, 9);
		string word = generateWord(adjs, advs, nouns, verbs, animals, seq);
		// check if the word has already been used before
		if (wordMap.find(word) == wordMap.end()) {
			wordMap[word] = 1;
			outfile << word << endl;
		}
		else { x--; }
	}
	outfile.close();
}


int getRandom(int start, int end)
{
	/*	adjective adjective noun
		noun verb animal
		adjective adjective animal
		adjective adverb adjective animal
		adverb adjective adjective animal
		adjective noun verb animal
		animal verb noun
		adverb verb adjective animal
		adjective adjective adverb verb animal
		*/
	return rand() % end + start;
}

string generateWord(string * adjs, string * advs, string * nouns, string * verbs, string * animals,
	int sequence)
{
	// adjective adjective noun
	if (sequence == 1) {
		return adjs[getRandom(0, ADJ - 1)] + " " + adjs[getRandom(0, ADJ - 1)]
			+ " " + nouns[getRandom(0, NOUN - 1)];
	}
	// noun verb animal
	if (sequence == 2) {
		return nouns[getRandom(0, NOUN - 1)] + " " + verbs[getRandom(0, VERBS - 1)] + "ing "
			+ animals[getRandom(0, ANM - 1)];
	}
	// adjective adjective animal
	if (sequence == 3) {
		return adjs[getRandom(0, ADJ - 1)] + " " + adjs[getRandom(0, ADJ - 1)]
			+ " " + animals[getRandom(0, ANM - 1)];
	}
	// adjective adverb adjective animal
	if (sequence == 4) {
		return adjs[getRandom(0, ADJ - 1)] + " " + advs[getRandom(0, ADV - 1)]
			+ " " + adjs[getRandom(0, ADJ - 1)] + " " + animals[getRandom(0, ANM - 1)];
	}
	// adverb adjective adjective animal
	if (sequence == 5) {
		return advs[getRandom(0, ADV - 1)] + " " + adjs[getRandom(0, ADJ - 1)]
			+ " " + adjs[getRandom(0, ADJ - 1)] + " " + animals[getRandom(0, ANM - 1)];
	}
	// adjective noun verb animal
	if (sequence == 6) {
		return adjs[getRandom(0, ADJ - 1)] + " " + nouns[getRandom(0, NOUN - 1)]
			+ " " + verbs[getRandom(0, VERBS - 1)] + "ing " + animals[getRandom(0, ANM - 1)];
	}
	// animal verb noun
	if (sequence == 7) {
		return animals[getRandom(0, ANM - 1)] + " " + verbs[getRandom(0, VERBS - 1)] + "ing "
			+ nouns[getRandom(0, NOUN - 1)];
	}
	// adverb verb adjective animal
	if (sequence == 8) {
		return advs[getRandom(0, ADV - 1)] + " " + verbs[getRandom(0, VERBS - 1)] + "ing "
			+ adjs[getRandom(0, ADJ - 1)] + " " + animals[getRandom(0, ANM - 1)];
	}
	// adjective adjective adverb verb animal
	if (sequence == 9) {
		return adjs[getRandom(0, ADJ - 1)] + " " + adjs[getRandom(0, ADJ - 1)] + " "
			+ advs[getRandom(0, ADV - 1)] + " " + verbs[getRandom(0, VERBS - 1)] + "ing "
			+ animals[getRandom(0, ANM - 1)];
	}
	return "";
}

