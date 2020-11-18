#include "SimpleAnomalyDetector.h"
#include "AnomalyDetector.h"
#include <vector>
#include "anomaly_detection_util.h"
#include "TimeSeries.h"
SimpleAnomalyDetector::SimpleAnomalyDetector(){};

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}
/**
 * @brief Determines if the 2 features are already in the vector cf.
 * 
 * @param cf 
 * @param fi 
 * @param fj 
 * @return true 
 * @return false 
 */
bool features_already_in(vector<correlatedFeatures> cf, string fi, string fj) {
	for (int i=0; i< cf.size(); i++) {
		if (cf[i].feature1.compare(fj) == 0 && cf[i].feature2.compare(fi) == 0)   {  
			return true;
		}
	}
	return false;
}

/**
 * @brief Function learns which columns in the time series are correlative and saves its findings in the member cf
 * 
 * @param ts 
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
	const int feature_size = ts.get_num_of_rows();
	const int num_of_features = ts.get_num_of_features();
	float features_correlation = 0.0;
	vector<Point**> vec_of_point_arrays; 
	//calculate correlation and add info to member cf
	for (int i =0; i < num_of_features; i++) {
		for (int j=0; j< num_of_features; j++) {
			if (i != j) {    //not the same feature     
				correlatedFeatures info;
				float* fi = ts.get_col_by_index(i);
				float* fj = ts.get_col_by_index(j);
				info.corrlation = std::abs(pearson(fi,fj,feature_size));  //calculate correlation between 2 features fi and fj
				if (info.corrlation >= 0.9) {   //verifying if the abs correlation of the two features is in fact correlative (over or equal to 0.9)
					const string featurei = (ts.get_features())[i];    //save first feature by name
					const string featurej = (ts.get_features())[j];   // save second feature by name  
					if (features_already_in(SimpleAnomalyDetector::cf,featurei,featurej)) {
						continue;
					}
					info.feature1 = featurei;
					info.feature2 = featurej;
					SimpleAnomalyDetector::cf.push_back(info);    //add info about the correlated features fi and fj.
				}
			}
		}
	}
	const int NUM_OF_CORRELATED_FEATURES = SimpleAnomalyDetector::cf.size();
	//intialize arrays of 2d points for each couple of correlated features. The size of each point array == feature_size.
	for (int i=0; i< NUM_OF_CORRELATED_FEATURES; i++) {
		Point** p_array = new Point*[feature_size];
		string featr1_str = SimpleAnomalyDetector::cf[i].feature1;
		string featr2_str = SimpleAnomalyDetector::cf[i].feature2;
		int feat_1_index = ts.get_index_of_feature(featr1_str);    //find index of feature 1 in the time series 
		int feat_2_index = ts.get_index_of_feature(featr2_str);    //find index of feature 2 in the time series 
		float* f1 = ts.get_col_by_index(feat_1_index);    //get the feature1 (column-data) by index
		float* f2 = ts.get_col_by_index(feat_2_index);    //get the feature1 (column-data) by index
		for (int j=0; j<feature_size; j++) {
			float x = f1[j];
			float y = f2[j];
			p_array[j] = new Point(x,y);    //add point to array --> all the (x,y): such that x is from feature1 && y is from feature2
		}
		vec_of_point_arrays.push_back(p_array);    //save array of all points for feature1 and feature2 in a vector.
	}
	for (int i=0; i<NUM_OF_CORRELATED_FEATURES; i++) {
		SimpleAnomalyDetector::cf[i].lin_reg = linear_reg(vec_of_point_arrays[i], feature_size);    //save the linear regression for each couple of correlated features.
	}
	//for all arrays of points and their linear regression line, calculate the max deviation:
	for (int i = 0; i < NUM_OF_CORRELATED_FEATURES; i++) {
		float max_deviation = 0.0;    //intialize max deviation for each correlated features.
		for (int j = 0; j<feature_size; j++) {
			float deviation;
			deviation = dev(*(vec_of_point_arrays[i][j]), SimpleAnomalyDetector::cf[i].lin_reg);    //find deviation between a point from the point array and the linear regression of 2 correlative features.
			if (max_deviation < deviation) {
				max_deviation = deviation;
			}
		}
		SimpleAnomalyDetector::cf[i].threshold = 1.1 * max_deviation;    //update max deviation for the correlated features * 1.1 - add 10 percent to the deviation
	}
	for (int i =0; i< NUM_OF_CORRELATED_FEATURES; i++) {
		delete[] vec_of_point_arrays[i];    //free allocated point array
	}	
}
/**
 * @brief Searches for anomalies in a given time series object. If an anomaly is found a report is saved in a vector of AnomalyReport
 * 
 * @param ts 
 * @return vector<AnomalyReport> 
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	vector<AnomalyReport> vec_report;
	const int size = ts.get_num_of_rows();
	const int row_size = ts.get_num_of_features();
	for (int i =0; i< cf.size(); i++) {    //iterate over all the elements in the cf member
		if (SimpleAnomalyDetector::cf[i].corrlation >= 0.9) {
			for (int j =1; j<= size; j++) {
				 float* row = new float[row_size];
				 ts.fill_row(j , row);    //get row j
				 const int feature1_index = ts.get_index_of_feature(SimpleAnomalyDetector::cf[i].feature1);    //find the index in which feature1 is placed in the row
			     const int feature2_index = ts.get_index_of_feature(SimpleAnomalyDetector::cf[i].feature2);    //find the index in which feature2 is placed in the row   
				 float x = row[feature1_index];
				 float y = row[feature2_index];
				 Point* point = new Point(x,y);    //create a point that the x value is from feature 1 and y value from feature2
				 float deviation  = dev(*point,SimpleAnomalyDetector::cf[i].lin_reg);    //calculate deviation from the linear regression line that was calculated in the learnNormal function 
				 delete point;    //free allocated Point.
				 if (deviation > SimpleAnomalyDetector::cf[i].threshold) {    //verifying if the deviation is bigger than the threshold found in the learnNormal function
					 const string description = SimpleAnomalyDetector::cf[i].feature1 + "-" + SimpleAnomalyDetector::cf[i].feature2;   
					 const long timestep = j;    //update the time step to be the index of the line in the file that is read at the moment
					 AnomalyReport report(description,timestep);    //create report about the anomaly found
					 vec_report.push_back(report);    //add report to the reports vector.
				 }
				 delete[] row;    //free allocated space.	 
			}
		}
	}
	return vec_report;
}
