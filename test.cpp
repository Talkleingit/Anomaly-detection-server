

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
	TimeSeries ts("trainFile1.csv");
	cout <<"number of rows " << ts.get_num_of_rows() << endl;
	cout << ts.get_val_of_feature_at_specified_time(26,"Heading") << endl;
	vector<float> vc;
	vector<float>::iterator it = vc.begin();
	ts.add_row(vc);
	
	cout << ts.get_val_of_feature_at_specified_time(1,"A") << endl;
	vector<float> col = ts.get_col("Heading");
	for (int i=0; i<col.size(); i++) {
		cout<<col[i] <<",";
	}
	 
	
	

	


}