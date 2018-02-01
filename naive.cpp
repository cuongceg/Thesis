#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
//#include <tuple> //tuple


//So we don't need to write std:: everywhere
using namespace std;

//point type for holding a coordinate 
struct point {
	double x;
	double y;
	point(double x,double y){
		this->x = x;
		this->y = y;
	}
	point(){

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
	}
	lineSegment(){

	}
};

//code from internet
double CCW(point a, point b, point c)
{ return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x); }

int middle(int a, int b, int c) {
  int t;    
  if ( a > b ) {
    t = a;
    a = b;
    b = t;
  }
  if ( a < c && c < b ) return 1;
  return 0;
}

int intersect(lineSegment a, lineSegment b) {
  if ( ( CCW(a.q, a.p, b.q) * CCW(a.q, a.p, b.p) < 0 ) &&
     ( CCW(b.q, b.p, a.q) * CCW(b.q, b.p, a.p) < 0 ) ) return 1;

  if ( CCW(a.q, a.p, b.q) == 0 && middle(a.q.x, a.p.x, b.q.x) && middle(a.q.y, a.p.y, b.q.y) ) return 1;
  if ( CCW(a.q, a.p, b.p) == 0 && middle(a.q.x, a.p.x, b.p.x) && middle(a.q.y, a.p.y, b.p.y) ) return 1;
  if ( CCW(b.q, b.p, a.q) == 0 && middle(b.q.x, b.p.x, a.q.x) && middle(b.q.y, b.p.y, a.q.y) ) return 1;
  if ( CCW(b.q, b.p, a.p) == 0 && middle(b.q.x, b.p.x, a.p.x) && middle(b.q.y, b.p.y, a.p.y) ) return 1;

    return 0;
}
/// end web


//Function for reading the next point in stdin
point readPoint(){
	point p;
	scanf("%lf,%lf\n",&p.x,&p.y);
	return p;
}

//Function that given a point returns a string representation of it
string toString(point p){
	ostringstream os;
	os << "(" << p.x << "," << p.y << ")";
	string s = os.str();
	return s;
}

//Function that given a linesegment returns a string representation of it
string toString(lineSegment l){
	ostringstream os;
	os << toString(l.p) << " - " << toString(l.q);
	string s = os.str();
	return s;
}


void readInput(point &start, point &end, string testTitle, vector<lineSegment> &lineSegments, vector<point> &points){

	//Get the test title
	getline(cin,testTitle);

	//Read start and end points
	start = readPoint();
	end  = readPoint();
	points.push_back(start);

	//Get the number of Polygons
	int numberOfPolygons;
	cin >> numberOfPolygons;

	//Iterate through the polygons
	for(int i=0;i<numberOfPolygons;i++){

		//Get the number of sides
		int numberOfSides;
		cin >> numberOfSides;

		//Get the first point and remember it 
		//so we can make the last linesegment after the loop
		point firstPoint = readPoint();

		//Create a variable for the last point we saw
		point lastPoint  = firstPoint;

		//Add the first point
		points.push_back(firstPoint);

		for(int j=1;j<numberOfSides;j++){

			//Get the next point
			point currentPoint = readPoint();

			//Add point to list of points
			points.push_back(currentPoint);

			//create linesegment
			lineSegment l;

			//Set the linesegment
			l.p = lastPoint;
			l.q = currentPoint;

			//push it to the list of linesegments
			lineSegments.push_back(l);

			//and update the lastPoint
			lastPoint = currentPoint;
		}

		//Construct the missing linesegment
		lineSegment l;	
		l.p = lastPoint;
		l.q = firstPoint;

		//and push it to the vector
		lineSegments.push_back(l);
	}

	points.push_back(end);

}

//Function for calculating the distance between two points
double dist(point p, point q){
	//calculate euclidean distance sqrt( (p.x-q.x)^2+(p.y-q.y)^2 )
	return (double) sqrt(pow(p.x-q.x,2.0)+pow(p.y-q.y,2.0));
}

double calculateA(lineSegment l){
	if(l.p.x==l.q.x) return 0;
	return (l.p.y-l.q.y)/(l.p.x-l.q.x);
}
double calculateB(lineSegment l, double a){
	//Since in this case we get ax+b=y => b=y
	if(a==0 || l.p.x == 0) return l.p.y;
	return l.p.y/(a*l.p.x);
}

bool crosses(lineSegment l1, lineSegment l2){
	return intersect(l1,l2);
	double l1_a = calculateA(l1);
	double l1_b = calculateB(l1,l1_a);

	cout << l1_a << "x+" << l1_b << "=y" << endl;

	double l2_a = calculateA(l2);
	double l2_b = calculateB(l2,l2_a);

	cout << l2_a << "x+" << l2_b << "=y" << endl;

	//If the slope is equal the lines a parallel
	//TODO: we might need a tolerance here
	if(l1_a==l2_a) return false;

	double crossing_x = (l2_b-l1_b)/(l1_a-l2_a);

	bool above_both_l1 = crossing_x >= l1.p.x && crossing_x >= l1.q.x;
	bool below_both_l1 = crossing_x <= l1.p.x && crossing_x <= l1.q.x;

	bool between_l1 = !(above_both_l1 || below_both_l1);

	bool above_both_l2 = crossing_x >= l2.p.x && crossing_x >= l2.q.x;
	bool below_both_l2 = crossing_x <= l2.p.x && crossing_x <= l2.q.x;

	bool between_l2 = !(above_both_l2 || below_both_l2);

	return between_l1 && between_l2;
}

//Takes a line segment and returns the number of polygon edges it crosses
int numberOfCrossings(vector<lineSegment> &lineSegments, lineSegment l){
	int n=0;
	for(int i = 0; i < lineSegments.size();i++){
		if(crosses(l,lineSegments[i])){
			n++;
		}
	}
	return n;
}

//Implementation of dijkstra
//Takes a graph and a start and end point in the graph
//returns the distance
double dijkstra(vector< vector< double > > &graph, int start, int end,vector<int> &route){

	route.resize(graph.size());

	//Create a vector to see if we already visited the point
	vector<bool> visited(graph.size());

	//Create a priority queue where pq_pair consists of 
	//the distance to the point and the point index in points vector
	priority_queue<pq_pair> pq;

	//Put the start point in the queue
	pq.push(pq_pair(make_pair(0,make_pair(start,-1))));

	//While there a still points we haven't visited we run
	while(!pq.empty()){

		//Get the top point
		pq_pair p = pq.top();	

		//Remove it
		pq.pop();

		//How far have we travelled until now
		double distanceSoFar = -1*p.first;

		//What point are we at
		int point = p.second.first;

		int whereFrom = p.second.second;

		//If we already visited the point continue
		if(visited[point]) continue;

		route[point] = whereFrom;

		//We we have reached the distination return the distance
		if(point == end) return distanceSoFar;

		//Set the point to true in the visited vector
		visited[point] = true;

		//Go through every point we have an edge to and haven't visited
		for(int i = 0; i < graph[point].size() ; i++){
			if(graph[point][i]==-1 || visited[i]) continue;

			//calculate the complete distance to that point
			double newdistance = distanceSoFar + graph[point][i];

			//And push it to the queue
			pq.push(make_pair(-1*newdistance,make_pair(i,point)));
		}
	}
	return -1;
}

int constructGraph(vector<lineSegment> &lineSegments, vector<point> &points, vector< vector < double > > &graph){

	//Get how many points we have
	int numberOfPoints = points.size();

	//Create a two dimenstional vector for the graph
	//where graph[i][j] is the distance from point i to j
	//if graph[i][j] = -1 then there is no connection
	graph.resize(numberOfPoints,vector<double>(numberOfPoints));


	//Go through all pairs of points and calculate the distance
	for(int i=0;i<numberOfPoints;i++){
		for(int j=0;j<numberOfPoints;j++){

			//If it is the same point don't make an edge
			if(i==j) graph[i][j]=-1;
			else{

				//Call dist function to calculate the distance
				graph[i][j]=dist(points[i],points[j]);
			}
		}
	}

	//Go through all pairs of points again
	//This time we want to calculate if we cross any polygon
	for(int i=0;i<numberOfPoints;i++){
		for(int j=0;j<numberOfPoints;j++){
			if(graph[i][j]!=-1){
				//Make a line segment from i to j
				lineSegment l;
				l.p = points[i];
				l.q = points[j];

				//Call numberOfCrossings, which 
				//suprise suprise counts the number of crossings
				if(numberOfCrossings(lineSegments,l)>0){

					//And remove the edge if it crosses any polygon
					graph[i][j] = -1;
				}
			}
		}
	}

	for(int i=0;i<graph.size();i++){
		for(int j=0;j<graph[i].size();j++){
			double value = graph[i][j];
			if(value==-1) continue;
		}
	}


	return 0;
}

void printLineSegments(vector<lineSegment> &lineSegments){
	cout << "line segments" << endl;
	for(int i=0;i<lineSegments.size();i++){
		cout << toString(lineSegments[i]) << endl;
	}
}

void printPoints(vector<point> &points){
	cout << "points" << endl;
	for(int i=0;i<points.size();i++){
		cout << toString(points[i]) << endl;
	}
}

void printGraph(vector < vector < double > > &graph){
	for(int i=0;i<graph.size();i++){
		for(int j=0;j<graph[i].size();j++){
			cout << graph[i][j] << " ";
		}
		cout << endl;
	}

}

#include "crossTesting.cpp"

int main(){
	//test();
	//return 0;

	//Create variables for holding start and endpoint plus the test title
	point start, end;
	string testTitle;

	//Create vector for containing the linesegments of the polygons
	vector<lineSegment> lineSegments;

	//Create vector for containing the points of the polygons
	vector<point> points;

	//Call function that parses the file input
	readInput(start, end, testTitle, lineSegments, points);

	vector< vector < double > > graph;

	//Call function that calculate the distance
	int status = constructGraph(lineSegments, points, graph);

	//Vector so we can backtrack the route
	vector<int> route;

	//The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graph,0,graph.size()-1,route);

	//Output the distance
	cout << distance << endl;
	int i = graph.size()-1;
	while(i!=-1){
		cout << toString(points[i]) << " ";
		i = route[i];
	}
	cout << endl;
}
