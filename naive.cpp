#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream

//So we don't need to write std:: everywhere
using namespace std;

//point type for holding a coordinate 
struct point {
	double x;
	double y;
};

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

int main(){
	//Get the test title
	string testTitle;
	getline(cin,testTitle);

	//Read start and end points
	point start = readPoint();
	point end  = readPoint();

	//Get the number of Polygons
	int numberOfPolygons;
	cin >> numberOfPolygons;


	//Create vector for containing the linesegments
	vector<lineSegment> lineSegments;

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

		for(int j=1;j<numberOfSides;j++){

			//Get the next point
			point currentPoint = readPoint();

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
