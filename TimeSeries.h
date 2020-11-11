
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include<algorithm>
#include <sstream>
using namespace std;

	//print - content	
class TimeSeries{
    vector<string> m_features;
	vector<vector<float>> m_dataTable;
	fstream m_fs;
	float m_numOfRows;
	int get_index_of_feature(const string& feature) const{
		string feat = feature;
		vector<string> vec = m_features;
		//save placement of the feature in the header.
		vector<string>::iterator findFeat = find(vec.begin(), vec.end(), feat);
		//calculate placement index
		int index_of_feature = distance(vec.begin(), findFeat);
		return index_of_feature;
	}
	public:
	TimeSeries(const char* CSVfileName){
		TimeSeries::m_fs.open(CSVfileName);
    bool found_features = false;
    string row; 
    float count =0;
    while (getline(m_fs,row)) {
        std::istringstream strStream(row);
        if (!found_features) {
            vector<string> tmp;
            //update features member.
            while (!strStream.eof()) {
                string featureString;
                //seperate row by comma
                getline(strStream,featureString,',');
                tmp.push_back(featureString);
            }
            //update member features
            TimeSeries::m_features = tmp;
            found_features = true;   
        }
        else {
                vector<float> tmp;
                while (!strStream.eof()) {
                    string value;
                    getline(strStream,value,',');
                    //convert the string to cstring
                    const char* str = value.c_str();
                    //adds numerical value into vector
                    tmp.push_back(std::atof(str));
                }
                //add row of values to the data table member
                TimeSeries::m_dataTable.push_back(tmp);
                //increment size of data table
                count++;
        }
    }
    TimeSeries::m_numOfRows = count;
	}
	int get_num_of_rows() {
		return this->m_numOfRows;
	}
	const vector<vector<float>>& get_dataTable() {
		return this->m_dataTable;
	}
	const vector<string>& getFeatures() const {return m_features;}
	void add_row(vector<float> row_to_add) {
		this->m_dataTable.push_back(row_to_add);
		this->m_numOfRows+=1;
	}
    float get_val_of_feature_at_specified_time(float time, const string& feature) const {
		const double EPSILON = 0.000000000000000000001;
		int index_of_feature = get_index_of_feature(feature);
		vector<vector<float>> dataMat = this->m_dataTable;
		vector<vector<float>>::iterator it = dataMat.begin();
		float found = 0.0;
		//search through first column. When time is found--> reach the correct index of specified feature and return value.
		for (int i=0; i<this->m_numOfRows; i++ ) {
			if (dataMat[i][0] == time) {
				found = dataMat[i][index_of_feature];
				break;
			}
		}
		return found;
	}
	//get row by index
	vector<float> get_row(int i) const {
		const int numOfCols = this->m_features.size();
		vector<vector<float>> dataMat = this->m_dataTable;
		vector<float> vec;
		for (int index = 0; index< this->m_numOfRows; index++) {
			if (index == i) {
				for (int j =0; j<numOfCols; j++) {
					vec.push_back(dataMat[index][j]);
				}
				break;
			}
		}
		return vec;
	}
	const vector<float>* get_col(const string& feature)	 {
		vector<vector<float>> dataMat = m_dataTable;
		vector<float> vec;
		int index_of_feature = get_index_of_feature(feature);
		for (int i=0; i<this->m_numOfRows; i++) {
			vec.push_back(dataMat[i][index_of_feature]);
		}
		return vec;
	}
	void Print() {
		int numOfCols =this->m_features.size();
		for (int i=0; i<4; i++ ) {
			cout<<m_features[i]<<endl
		}
		for (int i=0; i<m_numOfRows; i++){
			for (int j =0; j<numOfCols; j++) {
				if (j==numOfCols-1) {
					cout << this->m_dataTable[i][j] << endl;
				}
				else {
				cout << this->m_dataTable[i][j]<<',';
				}

			}
		}
	}
	//destructor
	~TimeSeries(){
		this->m_fs.close();
	}
};

#endif /* TIMESERIES_H_ */
