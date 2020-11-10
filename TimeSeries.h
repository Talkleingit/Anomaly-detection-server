
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;


	//print - content	
class TimeSeries{
	vector<string> m_features;
	vector<vector<double>> m_dataTable;
	public:
	TimeSeries(const char* CSVfileName);
	const vector<string>& getFeatures() const {
		return m_features;
	}
	void add_row(vector<double> row_to_add) {
		m_dataTable.push_back(row_to_add);
	}
	const double& get_val_of_feature(double time, string feature) const {
		


	}	
};





#endif /* TIMESERIES_H_ */
