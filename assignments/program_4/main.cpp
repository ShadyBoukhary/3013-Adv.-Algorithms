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
int num_of_lines(string);
int search(Graph&, string, string);
bool isValidEdge(Graph&, int, int, int);
void connectGraph(Graph&);
void closestEdges(Graph&);

void getLimits(Graph&, int&, int&, string&, string&);
void getClosestVertices(Graph&, distIndexPair[], int, int);
//void randomEdges(Graph&, int);

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
	G.printGraph();
	connectGraph(G);
	//closestEdges(G);
	system("pause");
	//G.printGraph();
	//closestEdges(G);
	//randomEdges(G, 100);


	outfile << G.graphViz(false);
	//G.printVids();

	//int *size = G.graphSize();

	//cout << "V= " << size[0] << " E= " << size[1] << endl;

	// for(int i=0;i<G.vertexList.size();i++){
	//     cout<<(*G.vertexList[i])<<endl;
	// }
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

void loadGraph(string filename, Graph &G) {
	ifstream file(filename);

	int zip;
	double lat;
	double lon;
	string strZip, strLat, strLon, city, state, county, key;
	int count = 0;

	cout << "What state do you wanna construct a graph for? (all for everything): ";
	getline(cin, key);
	//cin.ignore();
	while (!file.eof()) {
		getline(file, strZip, ',');
		getline(file, strLat, ',');
		getline(file, strLon, ',');
		getline(file, city, ',');
		getline(file, state, ',');
		getline(file, county);

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

int num_of_lines(string filename) {
	int numLines = 0;
	string line;
	ifstream file(filename);

	while (getline(file, line))
		numLines++;

	return numLines;
}

int search(Graph &G, string key, string state) {
	//cout << key << " " << state << endl;
	for (int i = 0; i < G.vList.size(); i++)
	{
		if (G.vList[i]->city == key && G.vList[i]->state == state)
		{
			cout << G.vList[i]->city << "::" << G.vList[i]->state
				<< "::" << i << " Found!" << endl;
			return i;
		}
	}
	return -1;
}

bool isValidEdge(Graph &G, int fromID, int toID, int edgePerV) {
	return ((G.vList[fromID]->E.size() < edgePerV)
		&& (G.vList[toID]->E.size() < edgePerV)
		&& (fromID != toID));
}

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
	cout << graph.getNumEdges() << " edges were created traveling in total "
		<< totalDistance << " miles." << endl;

}

void getLimits(Graph& graph, int& edgeLimit, int& index, string& starting, string& state) {
	// get starting city
	cout << "Enter the starting city: (eg. Dallas,TX)";
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

void closestEdges(Graph &G) {
	//int edgePerV;
	//int index;
	//std::string keyCity;
	//std::string keyState;
	//double distance;
	//double totDistance = 0;
	//latlon from;
	//latlon to;

	//while (1) {
	//	std::cout << "Enter Starting Location:\n(Ex. Wichita Falls,TX) ";
	//	std::getline(std::cin, keyCity, ',');
	//	std::getline(std::cin, keyState);

	//	index = search(G, keyCity);

	//	if (index == -1)
	//		std::cout << keyCity << ',' << keyState << " not found!" << std::endl;
	//	else if (index != -1)
	//		break;
	//}

	//while (1) {
	//	std::cout << "Edges per Vertex: ";
	//	std::cin >> edgePerV;

	//	if (std::cin.fail()) {
	//		std::cout << "Enter a proper integer!" << std::endl;
	//		std::cin.clear();
	//		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//	}
	//	else
	//		break;
	//}

	//distIndexPair * closestV = new distIndexPair[G.vList.size()];
	////std::vector<distIndexPair> closestV;
	//std::queue<distIndexPair> q;

	//for (int j = 0; j < G.vList.size(); j++) {
	//	// Get closest surrounding vertices
	//	for (int i = 0; i < G.vList.size(); i++) {
	//		from = G.vList[index]->location;
	//		to = G.vList[i]->location;
	//		distance = distanceEarth(from.lat, from.lon, to.lat, to.lon);
	//		//closestV.push_back(std::make_pair(distance, i));
	//		closestV[i] = make_pair(distance, i);
	//	}
	//	std::sort(closestV, closestV + G.vList.size());
	//	q.emplace(closestV[0]);
	//	bool first = true;
	//	while (!q.empty()) {
	//		int qCount = q.size() - 1;
	//		index = q.front().second;
	//		/*if (!first) {
	//		getClosestVertices(G, closestV, G.vList.size(), index);
	//		sort(closestV, closestV + G.vList.size());
	//		}
	//		first = false;*/

	//		for (int i = 1; i < G.vList.size() && i; i++) {
	//			// std::cout << "LOOP" << std::endl;
	//			int fromID = index;
	//			int toID = G.vList[closestV[i].second]->ID;
	//			double weight = closestV[i].first;

	//			if (isValidEdge(G, fromID, toID, edgePerV) && qCount < edgePerV) {
	//				q.emplace(closestV[i]);
	//				G.addEdge(fromID, toID, weight, false);
	//				totDistance += weight;
	//				qCount++;
	//			}
	//		}
	//		q.pop();
	//	}
	//	//closestV.clear();
	//}
	//cout << G.getNumEdges() << " edges were created traveling in total "
	//	<< totDistance << " miles." << endl;
}

//void randomEdges(Graph &G,int numEdges){
//    int r1,r2;
//    latlon from;
//    latlon to;
//    double d;
//    for(int i=0;i<numEdges;i++){
//        r1 = rand() % G.vList.size();
//        r2 = rand() % G.vList.size();
//        from = G.vList[r1]->location;
//        to = G.vList[r2]->location;
//        d = distanceEarth(from.lat,from.lon,to.lat,to.lon);
//        G.addEdge(r1,r2,(int)d,true);
//    }
//}