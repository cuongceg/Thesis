int numberOfSuccess = 0;
int numberOfFails = 0;
void testing(lineSegment l1, lineSegment l2, bool shouldCross){
	bool didCross = crosses(l1,l2);
	if(didCross==shouldCross){
		numberOfSuccess++;
		cout << "success" << endl;
	}
	else{
		numberOfFails++;
		cout << "Fail!" << endl;
	}
}

void test(){
	testing(lineSegment(point(0,0),point(1,1)),lineSegment(point(1,1),point(1,0)),false);
	testing(lineSegment(point(0,1),point(1,1)),lineSegment(point(0,0),point(1,1)),false);
	testing(lineSegment(point(0,0),point(0,1)),lineSegment(point(0,0),point(1,1)),false);
	testing(lineSegment(point(0,0),point(1,0)),lineSegment(point(0,1),point(1,0)),false);

	testing(lineSegment(point(0,0),point(2,1)),lineSegment(point(0,2),point(4,0)),false);
	testing(lineSegment(point(0,0),point(2,1)),lineSegment(point(-1,0),point(0,4)),false);

	testing(lineSegment(point(0,0),point(1,1)),lineSegment(point(0,1),point(1,0)),true);
	testing(lineSegment(point(1,1),point(2,0)),lineSegment(point(1,0),point(8,7)),true);

	cout << "number of fails: " << numberOfFails << ", number of success: " << numberOfSuccess << endl;
}
