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
struct point {
	double x;
	double y;
	point(double x,double y){
		this->x = x;
		this->y = y;
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
	}
	lineSegment(){

	}
	bool operator==(const lineSegment &other) const {
		return (this->p==other.p && this->q==other.q) ||
		(this->p==other.q && this->q==other.p);
  }
};

//Function for reading the next point in stdin
point readPoint(){
	point p;
	scanf("%lf,%lf\n",&p.x,&p.y);
	return p;
}

void readInput(point &start, point &end, string &testTitle, vector< vector< lineSegment> > &polygons, vector<point> &points){

	//Get the test title
	getline(cin,testTitle);

	//Read start and end points
	start = readPoint();
	end  = readPoint();
	points.push_back(start);

	//Get the number of Polygons
	int numberOfPolygons;
	cin >> numberOfPolygons;

	polygons.resize(numberOfPolygons, vector<lineSegment>());

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
			polygons[i].push_back(l);

			//and update the lastPoint
			lastPoint = currentPoint;
		}

		//Construct the missing linesegment
		lineSegment l;	
		l.p = lastPoint;
		l.q = firstPoint;

		//and push it to the vector
		polygons[i].push_back(l);
	}

	points.push_back(end);

}

//Function for calculating the distance between two points
double dist(point p, point q){
	//calculate euclidean distance sqrt( (p.x-q.x)^2+(p.y-q.y)^2 )
	return (double) sqrt(pow(p.x-q.x,2.0)+pow(p.y-q.y,2.0));
}

double rightTurn(point p1, point p2, point p3){
	return (p1.y-p2.y)*(p2.x-p3.x)-(p2.y-p3.y)*(p1.x-p2.x);
}

int crosses(lineSegment l1, lineSegment l2){
	if(l1==l2) return -1;
	int returnValue = 0;
	if(l1.p == l2.p) returnValue++; 
	if(l1.p == l2.q) returnValue++;
	if(l1.q == l2.p) returnValue++;
	if(l1.q == l2.q) returnValue++;
	if((rightTurn(l1.p,l1.q,l2.p)*rightTurn(l1.p,l1.q,l2.q)<=0) 
			&&  
			(rightTurn(l2.p,l2.q,l1.p)*rightTurn(l2.p,l2.q,l1.q)<=0)) returnValue += 4;
	return returnValue;
}

//Takes a line segment and returns the number of polygon edges it crosses
int numberOfCrossings(vector<vector<lineSegment> > &polygons, lineSegment l){
	int n=0;
	for(int i = 0; i < polygons.size();i++){
		int numberOfvaolation=0;
		for(int j=0;j<polygons[i].size();j++){
			int result = crosses(l,polygons[i][j]);
			if(result==-1){
				numberOfvaolation = -1;
				break;
			}
			else{
				numberOfvaolation+=result;
			}
		}
		if(numberOfvaolation>2){
			n++;
		}
	}
	return n;
}

//Implementation of dijkstra
//Takes a graph and a start and end point in the graph
//returns the distance
double dijkstra(vector< vector< double > > &graphDistance, vector<vector< int> > &graph ,vector<int> &route){
	int start = 0;
	int end = graph.size()-1;

	route.resize(graph.size());

	//Create a vector to see if we already visited the point
	vector<bool> visited(graph.size());

	//Create a priority queue where pq_pair consists of 
	//the distance to the point and the point index in points vector
	priority_queue< tuple<double,int,int> > pq;

	//Put the start point in the queue
	//TODO: tuple instead of pair?
	pq.push(make_tuple(0,start,-1));

	//While there a still points we haven't visited we run
	while(!pq.empty()){

		//Get the top point
		tuple<double,int,int> t = pq.top();	

		//Remove it
		pq.pop();

		//How far have we travelled until now
		double distanceSoFar = -1*get<0>(t);

		//What point are we at
		int current = get<1>(t);

		int whereFrom = get<2>(t);

		//If we already visited the current continue
		if(visited[current]) continue;

		route[current] = whereFrom;

		//We we have reached the distination return the distance
		if(current == end) return distanceSoFar;

		//Set the current to true in the visited vector
		visited[current] = true;

		//Go through every current we have an edge to and haven't visited
		for(int i = 0; i < graph[current].size() ; i++){
			int next = graph[current][i];
			if(visited[next]) continue;

			//calculate the complete distance to that current
			double newdistance = distanceSoFar + graphDistance[current][i];

			//And push it to the queue
			tuple<double,int,int> newTuple = make_tuple(-1*newdistance,next,current);

			pq.push(newTuple);
		}
	}
	return -1;
}

int makeVisabilityGraph(vector< vector < int > > &graph, vector< vector < double > > &graphDistance,vector< vector<int> > &crossesNumber, vector<point> &points){

	//Get how many points we have
	int numberOfPoints = points.size();

	//Create a two dimenstional vector for the graph
	graph.resize(numberOfPoints,vector<int>());
	graphDistance.resize(numberOfPoints,vector<double>());

	//Go through all pairs of points and calculate the distance
	for(int i=0;i<graph.size();i++){
		for(int j=0;j<numberOfPoints;j++){

			int from = i;
			int to = j;//(i/numberOfPoints)*numberOfPoints+j+crossesNumber[i][j]*numberOfPoints;

			//If it is the same point don't make an edge
			//if(graph.size()>to){
			if(crossesNumber[i][j]==0){
				//Call dist function to calculate the distance
				double distance = dist(points[i],points[j]);
				graphDistance[i].push_back(distance);
				graph[i].push_back(j);
			}
		}
	}

	return 0;
}

int calculateNumberOfCrossings(vector < vector < int > > &crossesNumber,vector<vector<lineSegment> > &polygons, vector<point> &points){
	crossesNumber.resize(points.size(),vector<int>(points.size()));
	for(int i=0;i<points.size();i++){
		for(int j=0;j<points.size();j++){
				lineSegment l;
				l.p = points[i];
				l.q = points[j];

				//Call numberOfCrossings, which 
				//suprise suprise counts the number of crossings
				crossesNumber[i][j] = numberOfCrossings(polygons,l);
		}
	}
	return 0;	
}

