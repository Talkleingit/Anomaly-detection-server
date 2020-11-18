
#ifndef TimeSeries_H_
#define TimeSeries_H_
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include<algorithm>
#include <sstream>
using namespace std;

class TimeSeries{
	/**
	 * @brief class TimeSeries - csv handeling.
	 * 
	 */
    vector<float*> m_dataTable;
    vector<string> m_features;
    fstream m_fs;
    int m_numOfRows;
    int m_numOfFeatures;
public:
	/**
	 * @brief Construct a new Time Series object
	 * 
	 * @param CSVfileName 
	 */
	TimeSeries(const char* CSVfileName);
         
    
	/**
      * @brief Getter
      * 
      * @return const vector<float*>& 
      */
    const vector<float*>& get_dataTable() const {
		return this->m_dataTable;
	}
	
	/**
	 * @brief Getter
	 * 
	 * @return const vector<string>& 
	 */
    const vector<string>& get_features() const {
		return this->m_features;
	}

	/**
	 * @brief Get the num of rows
	 * 
	 * @return int 
	 */
    int get_num_of_rows() const {
		return this->m_numOfRows;
	}

	/**
	 * @brief Get the num of features
	 * 
	 * @return int 
	 */
    int get_num_of_features() const {
		return this->m_numOfFeatures;
	}

	/**
	 * @brief Get the col by index
	 * 
	 * @param i 
	 * @return float* 
	 */
    float* get_col_by_index(int i) const;

	/**
	 * @brief fills a row with the values of the ith row in the data table.
	 * 
	 * @param i 
	 * @param row 
	 */
    void fill_row(int i, float* row) const;
	
	/**
	 * @brief adds a row to the data table
	 * 
	 * @param row_to_add 
	 */
    void add_row(float* row_to_add);

    /**
     * @brief Get the index of a feature
     * 
     * @param feature 
     * @return int 
     */
    int get_index_of_feature(const string& feature) const;

    /**
     * @brief Get the value of a feature at a specified time
     * 
     * @param feature 
     * @param time 
     * @return float 
     */
    float get_value_of_feature_at_specified_time(const string& feature,int time) const;
	
	/**
	 * @brief Destroy the Time Series object
	 * 
	 */
    ~TimeSeries();
};

#endif /* TimeSeries_H_ */
