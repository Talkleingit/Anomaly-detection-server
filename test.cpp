

#include <iostream>
#include <vector>
#include "anomaly_detection_util.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include "TimeSeries.h"

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
	srand (time(NULL));
	float a1=1+rand()%10, b1=-50+rand()%100;
	float a2=1+rand()%20 , b2=-50+rand()%100;
	// test the learned model: (40 points)
	// expected correlations:
	//	A-C: y=a1*x+b1
	//	B-D: y=a2*x+b2

	generateTrainCSV(a1,b1,a2,b2);
	TimeSeries ts("trainFile1.csv");
	int n = ts.get_num_of_rows();
	float v1[n];
	float v2[n];
	float v3[n];
	float v4[n];
	ts.fill_col("Time", v1);
	ts.fill_col("Altitude", v2);
	ts.fill_col("Air speed", v3);
	ts.fill_col("Heading", v4);
	for (int j =0; j<n; j++) {
		cout << v1[j] << ",";
	}
	cout << endl;
	for (int j =0; j<n; j++) {
		cout << v2[j] << ",";
	}
	cout << endl;
	for (int j =0; j<n; j++) {
		cout << v3[j] << ",";
	}
	cout << endl;
	for (int j =0; j<n; j++) {
		cout << v4[j] << ",";
	}
	cout << endl;
	vector<const float*> vk;
	for (int i=0; i< n; i++) {
		vk.push_back(ts.get_row(i));
	}
	for (int i=0; i< n; i++) {
		for (int j=0; j< ts.getFeatures().size(); j++) {
			cout << (vk[i])[j] << ",";
		}
		cout << endl;
	} 


}