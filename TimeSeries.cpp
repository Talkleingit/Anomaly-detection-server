#include "TimeSeries.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
TimeSeries::TimeSeries(const char* CSVfileName) {
    fstream fs;
    fs.open(CSVfileName);
    bool found_features = false;
    string row; 
    while (getline(fs,row)) {
        std::istringstream strStream(row);
        if (!found_features) {
            vector<string> tmp;
            //update features member.
            while (strStream) {
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
                vector<double> tmp;
                while (strStream) {
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
        }
    }
    //update memeber size - number of rows
}



