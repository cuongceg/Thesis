#include "naive.cpp"

int main(int argc, const char* argv[]){
	int k=0;
	bool naive = true;

	if(argc>1){
		k = atoi(argv[1]);
	}
	bool printGraph = true;

	//Create variables for holding start and endpoint plus the test title
	point start, end;
	string testTitle;

	//Create vector for containing the linesegments of the polygons
	vector< vector < lineSegment> > polygons;

	//Create vector for containing the points of the polygons
	vector<point> points;

	//Call function that parses the file input
	readInput(start, end, testTitle, polygons, points);

	vector< vector < int > > graph;
	vector< vector < double > > graphDistance;

	//Vector so we can backtrack the route
	vector<int> route;

	vector< vector < int > > crossesNumber;

	calculateNumberOfCrossings(crossesNumber, polygons, points);

	//Call function that calculate the distance
	makeVisabilityGraph(graph, graphDistance, crossesNumber, points);


	//The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graphDistance,graph,route);

	//Output the distance
	
	if(printGraph){
		cout << testTitle << endl;
		cout << start.x << "," << start.y << endl;
		cout << end.x << "," << end.y << endl;
		cout << polygons.size() << endl;
		for(int i=0;i<polygons.size();i++){
			cout << polygons[i].size() << " ";
			for(int j=0;j<polygons[i].size();j++){
				cout << polygons[i][j].p.x << "," << polygons[i][j].p.y << " ";	
			}
			cout << endl;
		}
		int current = graph.size()-1;
		while(current!=-1){
			int x = (int) points[current].x;
			int y = (int) points[current].y;
			cout << x << "," << y << " ";
			current = route[current];
		}
		cout << endl;
	}
	cout << distance << endl;
}
