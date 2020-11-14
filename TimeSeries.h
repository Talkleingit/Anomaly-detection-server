
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
	vector<float*> m_dataTable;
	fstream m_fs;
	int m_numOfRows;

	/**
	 * @brief Get the index of feature within the feature vector.
	 * 
	 * @param feature 
	 * @return int 
	 */
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
	/**
	 * @brief Construct a new Time Series object
	 * 
	 * @param CSVfileName 
	 */
	TimeSeries(const char* CSVfileName){
		TimeSeries::m_fs.open(CSVfileName);
    bool found_features = false;
    string row; 
    int count =0;
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
				int size = tmp.size();
                float* new_row = new float[size];
				int k = 0;
                while (!strStream.eof()) {
                    string value;
                    getline(strStream,value,',');
                    //convert the string to cstring
                    const char* str = value.c_str();
					//add val to row (convert to float)
					new_row[k] = std::atof(str);
					k++;
                }
                //add row of values to the data table member
                TimeSeries::m_dataTable.push_back(new_row);
                //increment size of data table
                count++;
        }
    }
    this->m_numOfRows = count;
	}
	
	/**
	 * @brief Get the num of rows
	 * 
	 * @return int 
	 */
	int get_num_of_rows() {
		return this->m_numOfRows;
	}
	
	/**
	 * @brief Get the dataTable member
	 * 
	 * @return const vector<float*>& 
	 */
	const vector<float*>& get_dataTable() {
		return this->m_dataTable;
	}
	/**
	 * @brief Get the Features member, the header of the csv file.
	 * 
	 * @return const vector<string>& 
	 */
	const vector<string>& getFeatures() const {return m_features;}
	
	/**
	 * @brief adds a row to the data table.
	 * 
	 * @param row_to_add 
	 */
	void add_row(float* row_to_add) {
		this->m_dataTable.push_back(row_to_add);
		this->m_numOfRows+=1;
	}
    
	/**
	 * @brief Get the val of feature at a specified time
	 * 
	 * @param time 
	 * @param feature 
	 * @return float 
	 */
	float get_val_of_feature_at_specified_time(float time, const string& feature) const {
		int index_of_feature = get_index_of_feature(feature);
		vector<float*> dataMat = this->m_dataTable;
		float found = 0.0;
		//search through first column. When time is found--> reach the correct index of specified feature and return value.
		for (int i=0; i<this->m_numOfRows; i++ ) {
			if (dataMat[i][0] == time -1) {
				found = dataMat[i][index_of_feature];
				break;
			}
		}
		return found;
	}
	
	/**
	 * @brief Get the row according to a specific index.
	 * 
	 * @param i 
	 * @return const float* 
	 */
	const float* get_row(int i) const {
		vector<float*> dataMat = this->m_dataTable;
		const float* row = dataMat[i];
        return row;
	}

	/**
	 * @brief fills an empty float array (col) with the values from the correct colomn in the data table.
	 * 		 
	 * @param feature 
	 * @param col 
	 */
    void fill_col(const string& feature, float* col)	 {
		vector<float*> dataMat = m_dataTable;
		int index_of_feature = get_index_of_feature(feature);
		for (int i=0; i<this->m_numOfRows; i++) {
            col[i] = dataMat[i][index_of_feature];
		}
	}

	/**
	 * @brief Destroy the Time Series object
	 * 
	 */
	~TimeSeries(){
		//delete each row in the data table.
		for (int i=0; i<this->m_numOfRows; i++) {
			delete[] m_dataTable[i];
		}
		this->m_fs.close();
	}
};

#endif /* TIMESERIES_H_ */
