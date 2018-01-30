#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue

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
typedef std::pair<double,int> pq_pair;

//linesegment type holding two points
struct lineSegment {
	point p1;
	point p2;
};

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
	os << toString(l.p1) << " - " << toString(l.p2);
	string s = os.str();
	return s;
}

//Create variables for holding start and endpoint plus the test title
point start, end;
string testTitle;

//Create vector for containing the linesegments of the polygons
vector<lineSegment> lineSegments;

//Create vector for containing the points of the polygons
vector<point> points;

void readInput(){

	//Get the test title
	getline(cin,testTitle);

	//Read start and end points
	start = readPoint();
	end  = readPoint();

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
			l.p1 = lastPoint;
			l.p2 = currentPoint;

			//push it to the list of linesegments
			lineSegments.push_back(l);

			//and update the lastPoint
			lastPoint = currentPoint;
		}

		//Construct the missing linesegment
		lineSegment l;	
		l.p1 = lastPoint;
		l.p2 = firstPoint;

		//and push it to the vector
		lineSegments.push_back(l);
	}

}

//Function for calculating the distance between two points
double dist(point p, point q){
	//calculate euclidean distance sqrt( (p.x-q.x)^2+(p.y-q.y)^2 )
	return (double) sqrt(pow(p.x-q.x,2.0)+pow(p.y-q.y,2.0));
}

double calculateA(lineSegment l){
	return (l.p1.y-l.p2.y)/(l.p1.x-l.p2.x);
}
double calculateB(lineSegment l, double a){
	return l.p1.y/(a*l.p1.x);
}


bool crosses(lineSegment l1, lineSegment l2){
	double l1_a = calculateA(l1);
	double l1_b = calculateB(l1,l1_a);

	double l2_a = calculateA(l2);
	double l2_b = calculateB(l2,l2_a);

	//If the slope is equal the lines a parallel
	//TODO: we might night a tolerance here
	if(l1_a==l2_a) return false;

	double crossing_x = (l2_b-l1_b)/(l1_a-l2_a);

	bool above_both = crossing_x > l1.p1.x && crossing_x > l1.p2.x;
	bool below_both = crossing_x < l1.p1.x && crossing_x < l1.p2.x;

	bool do_they_cross = !(above_both || below_both);

	return do_they_cross;
}

//Takes a line segment and returns the number of polygon edges it crosses
int numberOfCrossings(lineSegment l){
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
double dijkstra(vector< vector< double > > &graph,int from, int to){

	//Create a vector to see if we already visited the point
	vector<bool> visited(graph.size());

	//Create a priority queue where pq_pair consists of 
	//the distance to the point and the point index in points vector
	priority_queue<pq_pair> pq;

	//Put the start point in the queue
	pq.push(pq_pair(0,from));

	//Set the visit status to true
	visited[0] = true;

	//While there a still points we haven't visited we run
	while(!pq.empty()){

		//Get the top point
		pq_pair p = pq.top();	

		//Remove it
		pq.pop();

		//How far have we travelled until now
		double distanceSoFar = p.first;

		//What point are we at
		int point = p.second;

		//We we have reached the distination return the distance
		if(point == to) return distanceSoFar;

		//Set the point to true in the visited vector
		visited[point] = true;

		//Go through every point we have an edge to and haven't visited
		for(int i = 0; i < graph[point].size() ; i++){
			if(graph[point][i]==-1 || visited[i]) continue;

			//calculate the complete distance to that point
			double newdistance = distanceSoFar + graph[point][i];

			//And push it to the queue
			pq.push(pq_pair(newdistance,i));
		}
	}
	return -1;
}

double calculateDistance(){

	//Get how many points we have
	int numberOfPoints = points.size();

	//Create a two dimenstional vector for the graph
	//where graph[i][j] is the distance from point i to j
	//if graph[i][j] = -1 then there is no connection
	vector< vector<double> > graph(numberOfPoints,vector<double>(numberOfPoints));


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
				l.p1 = points[i];
				l.p2 = points[j];

				//Call numberOfCrossings, which 
				//suprise suprise counts the number of crossings
				if(numberOfCrossings(l)>0){

					//And remove the edge if it crosses any polygon
					graph[i][j] = -1;
				}
			}
		}
	}

	//The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graph,0,graph.size()-1);

	//And return it
	return distance;
}

int main(){
	//Call function that parses the file input
	readInput();

	//Call function that calculate the distance
	double distance = calculateDistance();

	//Output the distance
	cout << distance << endl;
}
