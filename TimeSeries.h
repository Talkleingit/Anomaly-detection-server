
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <cstring>
//using namespace std;

class TimeSeries{
char* m_CSVfileName;
public:
	TimeSeries(const char* CSVfileName){
		int n = strlen(CSVfileName);
		this->m_CSVfileName = new char[n];
		for (int i=0; i< n; i++) {
			this->m_CSVfileName[i] = CSVfileName[i];
		}
	}
	std::string get_features_from_csv() {
		
	}

};



#endif /* TIMESERIES_H_ */
