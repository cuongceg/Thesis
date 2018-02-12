#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple


//So we don't need to write std:: everywhere
using namespace std;

//point type for holding a coordinate 
void setMinMax(double x, double y);
struct point {
	double x;
	double y;
	point(double x,double y){
		this->x = x;
		this->y = y;
		setMinMax( x, y);
	}

	point(){

	}
	bool operator==(const point &other) const {
		return (this->x==other.x && this->y==other.y);
	}
};
//Type used in the priority queue in the dijkstra function
typedef std::pair<double,std::pair<int,int> > pq_pair;

//linesegment type holding two points
struct lineSegment {
	point p;
	point q;
	lineSegment(point p,point q){
		this->p = p;
		this->q = q;
	};
	lineSegment(){

	}
	bool operator==(const lineSegment &other) const {
		return (this->p==other.p && this->q==other.q) ||
		(this->p==other.q && this->q==other.p);
  }
};

#include "draw.cpp"
#include "naive.cpp"

int main(int argc, const char* argv[]){
	//cout << crosses(lineSegment(point(0,0),point(2,2)),lineSegment(point(1,1),point(2,0))) << endl;
	//return 0;
	max_x=max_y=min_y=min_x=0;
	int k=0;

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
		draw(testTitle,start,end, polygons,distance,points,route,graph);	
	}
	else{
		cout << distance << endl;
	}
}
