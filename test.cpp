

#include <iostream>
#include <vector>
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
void generateTrainCSV(float a1,float b1, float a2, float b2){
	ofstream out("trainFile1.csv");
	out<<"Time,Altitude,Air speed,Heading"<<endl;
	Line ac(a1,b1);
	Line bd(a2,b2);
	for(int i=1;i<=100;i++){
		float a=i;
		float b=rand()%40;
		out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
	}
	out.close();
}
int main() {
    generateTrainCSV(1.2,2.4,3.5,4.6);
    return 0;
}