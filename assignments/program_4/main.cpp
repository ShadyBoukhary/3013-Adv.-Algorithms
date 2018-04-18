/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*										Shady Boukhary											*
*									3013 Advanced Algorithms									*
*									Thursday 18th, April 2018									*
*										Dr. Terry Griffin										*
*																								*
*																								*
*	This program constructs a graph of cities in the US according to user options. The user		*
*	can choose to limit the graph to a specific state or include all - as well as setting		*
*	a starting city. The program reads a file contatining 18.9K cities into a graph with		*
*	no edges. Then it connects, starting with the city, the vertices to the closest cities		*
*	around it.																					*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


#include <iostream>
#include "csv.h"
#include "distance.h"
#include "graph.h"
#include <algorithm>
#include <utility>
#include <queue>

using namespace std;
typedef pair<double, int> distIndexPair;

void loadGraph(string, Graph&);
int search(Graph&, string, string);
bool isValidEdge(Graph&, int, int, int);
void connectGraph(Graph&);

void getLimits(Graph&, int&, int&, string&, string&);
void getClosestVertices(Graph&, distIndexPair[], int, int);

// Test Driver
int main()
{
	Graph G;
	int max_vertices = 0;
	int max_edges = 0;
	string searchType;
	string key;
	string filename = "filtered_cities.csv";


	ofstream outfile;
	outfile.open("graphViz.dat");

	loadGraph(filename, G);
	//G.printGraph();
	connectGraph(G);
	//closestEdges(G);
	//G.printGraph();



	outfile << G.graphViz(false);

	int countZ = 0, count1 = 0, count2 = 0, count3 = 0;
	for (int i = 0; i < G.vList.size(); i++) {
		if (G.vList[i]->E.size() == 0)
			countZ++;
		if (G.vList[i]->E.size() == 1)
			count1++;
		if (G.vList[i]->E.size() == 2)
			count2++;
		if (G.vList[i]->E.size() == 3)
			count3++;
	}
	cout << endl << "Zero: " << countZ << endl
		<< "One: " << count1 << endl
		<< "Two: " << count2 << endl
		<< "Three: " << count3 << endl;
	system("pause");
	return 0;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											loadGraph()											*
*	parameters: string, Graph&																	*
*	return: void																				*
*	reads the input file of cities and adds them as vertices to a graph							*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void loadGraph(string filename, Graph &G) {
	ifstream file(filename);

	int zip;
	double lat;
	double lon;
	string strZip, strLat, strLon, city, state, county, key;
	int count = 0;

	cout << "What state do you wanna construct a graph for? (all for everything): ";
	getline(cin, key);
	// read file 
	while (!file.eof()) {
		getline(file, strZip, ',');
		getline(file, strLat, ',');
		getline(file, strLon, ',');
		getline(file, city, ',');
		getline(file, state, ',');
		getline(file, county);

		// convert from strings to ints and doubles
		zip = stoi(strZip);
		lat = stod(strLat);
		lon = stod(strLon);

		if (key == "all" || key == "ALL") {
			G.addVertex(city, state, lat, lon);
			count++;
		}
		else if (state == key) {
			G.addVertex(city, state, lat, lon);
			count++;

		}

	}
	cout << "Cities: " << count << "\nState(s): " << key << endl;;
	file.close();
}


/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											search()											*
*	parameters: Graph&, string, state															*
*	return: int																					*
*	searches for a city in the graph, returns index if found, -1 if not found					*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int search(Graph &G, string key, string state) {
	for (int i = 0; i < G.vList.size(); i++)
	{
		if (G.vList[i]->city == key && G.vList[i]->state == state)
		{
			cout << G.vList[i]->city << "," << G.vList[i]->state
				<< " found at index " << i << endl;
			return i;
		}
	}
	return -1;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											isValidEdge()										*
*	parameters: Graph&, int, int, int															*
*	return: bool																				*
*	determines whether 2 edges can be connected according to the limits set by user				*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
bool isValidEdge(Graph &G, int fromID, int toID, int edgePerV) {
	return ((G.vList[fromID]->E.size() < edgePerV)
		&& (G.vList[toID]->E.size() < edgePerV)
		&& (fromID != toID));
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											getClosestVertices()								*
*	parameters: Graph&, distIndexPair, int, int													*
*	return: void																				*
*	creates an array of distance-index pairs filled with the closest vertices to a vertex		*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void getClosestVertices(Graph& graph, distIndexPair closestVectors[], int size, int indexStarting) {
	latlon from, to;
	double distance;

	for (int y = 0; y < graph.vList.size(); y++) {
		// get latlon of starting location
		from = graph.vList[indexStarting]->location;

		// get latlon of close vertex
		to = graph.vList[y]->location;

		// compute the distance between 2 cities
		distance = distanceEarth(from.lat, from.lon, to.lat, to.lon);

		// make a distance, index pair for the vertex and store in array
		closestVectors[y] = make_pair(distance, y);
	}
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											connectGraph()										*
*	parameters: Graph&																			*
*	return: void																				*
*	Connects the vertices in the graph according to limitations set by the user					*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void connectGraph(Graph& graph) {
	int edgeLimit, indexStarting;
	string startingCity;
	string startingState;
	latlon from, to;

	getLimits(graph, edgeLimit, indexStarting, startingCity, startingState);


	queue<distIndexPair> q;
	double distance, totalDistance = 0;
	const int VSIZE = graph.vList.size();

	// array to hold closest vertices 
	distIndexPair * closestVectors = new distIndexPair[VSIZE];
	for (int x = 0; x < graph.vList.size(); x++) {
		getClosestVertices(graph, closestVectors, VSIZE, indexStarting);

		// sort all vertices from closest to farthest from starting city
		sort(closestVectors, closestVectors + VSIZE);
		// push starting vector on the queue
		q.emplace(closestVectors[0]);
		bool first = true;
		int h = 0;
		// connect the vertices
		// empty queue means we're done
		while (!q.empty()) {
			// get index of current vertex dealt with
			int fromIndex = q.front().second;
			int qCount = q.size() - 1;

			//if (!first) {
				getClosestVertices(graph, closestVectors, VSIZE, fromIndex);
				sort(closestVectors, closestVectors + VSIZE);
			//}
			first = false;
			// keep looking for a vertex to connect until all have been checked or limit per vertex is reached
			for (int currentClosest = 1; currentClosest < VSIZE && graph.vList[fromIndex]->E.size() < edgeLimit; currentClosest++) {
				// get the ID of the current closest vertex
				int toID = graph.vList[closestVectors[currentClosest].second]->ID;
				//if second closest has a much lower degree, use that instead
				if (currentClosest + 50 < VSIZE && graph.vList[closestVectors[currentClosest].second]->E.size() > graph.vList[closestVectors[currentClosest + 50].second]->E.size()
					&& isValidEdge(graph, fromIndex, toID, edgeLimit))
					toID = graph.vList[closestVectors[currentClosest + 50].second]->ID;
				double currentDistance = closestVectors[currentClosest].first;

				// check if edges can be connected
				if (isValidEdge(graph, fromIndex, toID, edgeLimit)) {
					if (!graph.vList[fromIndex]->visited)
						q.emplace(closestVectors[currentClosest]);
					graph.addEdge(fromIndex, toID, currentDistance, false);
					totalDistance += currentDistance;
					qCount++;
				}
			}
			/*cout << h++ << endl;
			cout.flush();*/
			graph.vList[fromIndex]->visited = true;
			// pop the finished vertex to continue on to the next one
			q.pop();
		}
	}
	cout << graph.getNumEdges() << " edges.\n"
		<< totalDistance << " miles." << endl;

}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*											getLimits()											*
*	parameters: Graph&, int&, int&, string&, string&											*
*	return: void																				*
*	Gets the starting city to connect graphs and the edge limit per vertex from the user		*
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void getLimits(Graph& graph, int& edgeLimit, int& index, string& starting, string& state) {
	// get starting city
	cout << "Enter the starting city: (eg. Dallas,TX) ";
	getline(cin, starting, ',');
	getline(cin, state);
	index = search(graph, starting, state);
	while (index == -1) {
		cout << "City not found, try again.";
		getline(cin, starting);
		index = search(graph, starting, state);
	}

	// get edge limit
	cout << "Edge limit per vertex: ";
	cin >> edgeLimit;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(128, '\n');
		cout << "Try Again.";
	}
}

