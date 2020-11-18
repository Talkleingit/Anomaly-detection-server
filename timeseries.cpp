#include "timeseries.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include<algorithm>
#include <sstream>
TimeSeries::TimeSeries(const char* CSVfileName) {
        TimeSeries::m_fs.open(CSVfileName);
         bool found_features = false;
         string row;
         int row_idx = 0;    //row index for each column
         int counter = 0;
         vector<int> row_indexs;
         while (getline(m_fs,row)) {
             counter++;
         }
         TimeSeries::m_numOfRows = counter - 1;    //update member numOfRows, need to reduce 1 from the counter because the first line in the csv contains the features names.
         TimeSeries::m_fs.clear();    
         TimeSeries::m_fs.seekg(0,m_fs.beg);    //return to top of csv
         while (getline(TimeSeries::m_fs,row)) {
            std::istringstream strStream(row);
            if (!found_features) {
             while (!strStream.eof()) {
                 string featureName;
                 getline(strStream, featureName, ',');    //seperate by comma
                 TimeSeries::m_features.push_back(featureName);
            }
            TimeSeries::m_numOfFeatures = TimeSeries::m_features.size();    //update member number of features
            const int n = TimeSeries::m_numOfRows;
            for (int i=0; i<TimeSeries::m_numOfFeatures; i++) {    //number of elements in m_dataTable is the number of elements in the m_features.
                 float* col = new float[n];
                 TimeSeries::m_dataTable.push_back(col);
            }
            found_features = true;
        }
        else {
             while (!strStream.eof()) {
                 for (int col =0; col<TimeSeries::m_numOfFeatures; col++) { 
                         string value;
                         getline(strStream, value, ',');
                         const char* str = value.c_str();
                         TimeSeries::m_dataTable[col][row_idx] = std::atof(str);    //convert value to float and update data table member at correct column
                    }
                    row_idx++;     //increment row index for all columns
                 } 
             }      
        }
}

float* TimeSeries::get_col_by_index(int i) const {
    vector<float*> dt = TimeSeries::get_dataTable();
        return (TimeSeries::get_dataTable())[i];
}

void TimeSeries::fill_row(int i, float* row) const {
    vector<float*> dt = TimeSeries::get_dataTable();
    for (int col =0; col< TimeSeries::get_num_of_features(); col++) {
             //add value to row
             row[col] = dt[col][i-1];
    }
}

void TimeSeries::add_row(float* row_to_add) {
    const int size = TimeSeries::get_num_of_rows() + 1;
    vector<float*> save;
    vector<float*>::iterator col = TimeSeries::m_dataTable.begin();
    for (int i=0; i<TimeSeries::get_num_of_features(); i++) {
        save.push_back(TimeSeries::m_dataTable[i]);
        TimeSeries::m_dataTable.erase(col);
       }
    for (int i=0; i< TimeSeries::get_num_of_features(); i++) {
        float* tmp_col = new float[size];
        TimeSeries::m_dataTable.push_back(tmp_col);
    }
    for (int c =0; c < TimeSeries::get_num_of_features(); c++) {
        for (int r = 0; r < size; r++) {
             if (r < size-1) {
                TimeSeries::m_dataTable[c][r] = save[c][r];
               }
            else {
                   TimeSeries::m_dataTable[c][r] = row_to_add[c];
            }
          }
       }
       TimeSeries::m_numOfRows +=1;

}

int TimeSeries::get_index_of_feature(const string& feature) const {
    string feat = feature;
	vector<string> vec = TimeSeries::m_features;
	//save placement of the feature in the header.
	vector<string>::iterator findFeat = find(vec.begin(), vec.end(), feat);
	//calculate placement index
	int index_of_feature = distance(vec.begin(), findFeat);
	return index_of_feature;
}

float TimeSeries::get_value_of_feature_at_specified_time(const string& feature,int time) const{
    vector<float*> dt = this->get_dataTable();
    vector<string> features = this->get_features();
    int index = get_index_of_feature(feature);
    return dt[index][time -1];
}

TimeSeries::~TimeSeries() {
    for (int i=0; i<TimeSeries::get_num_of_features(); i++) {
        delete[] TimeSeries::m_dataTable[i];    //free allocated column for each feature in the csv file.
    }
    TimeSeries::m_fs.close();
}