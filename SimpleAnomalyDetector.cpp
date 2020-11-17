#include "SimpleAnomalyDetector.h"
#include "AnomalyDetector.h"
#include <vector>
#include "anomaly_detection_util.h"
#include "TimeSeries.h"
SimpleAnomalyDetector::SimpleAnomalyDetector(){};

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

bool features_already_in(vector<correlatedFeatures> cf, string fi, string fj) {
	for (int i=0; i< cf.size(); i++) {
		if (cf[i].feature1.compare(fj) == 0 && cf[i].feature2.compare(fi) == 0)   {  
			return true;
		}
	}
	return false;
}
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
	//create vector of correlated features
	const int feature_size = ts.get_num_of_rows();
	const int num_of_features = ts.get_num_of_features();
	float features_correlation = 0.0;
	vector<Point**> vec_of_point_arrays; 
	//calculate correlation and add info to member cf
	for (int i =0; i < num_of_features; i++) {
		for (int j=0; j< num_of_features; j++) {
			//not the same feature
			if (i != j) {
				correlatedFeatures info;
				//define threshold
				//info.corrlation = 0.99;
				//calculate correlation between 2 features fi and fj
				float* fi = ts.get_col_by_index(i);
				float* fj = ts.get_col_by_index(j);
				info.corrlation = std::abs(pearson(fi,fj,feature_size));
				if (info.corrlation > 0.99) {
					const string featurei = (ts.get_features())[i];
					const string featurej = (ts.get_features())[j];
					if (features_already_in(SimpleAnomalyDetector::cf,featurei,featurej)) {
						continue;
					}
					info.feature1 = featurei;
					info.feature2 = featurej;
					//add info about the correlated features fi and fj.
					SimpleAnomalyDetector::cf.push_back(info);
				}
			}
		}
	}
	const int NUM_OF_CORRELATED_FEATURES = SimpleAnomalyDetector::cf.size();
	//intialize arrays of 2d points for each couple of correlated features. size of each point array  = feature_size
	for (int i=0; i< NUM_OF_CORRELATED_FEATURES; i++) {
		Point** p_array = new Point*[feature_size];
		string featr1_str = SimpleAnomalyDetector::cf[i].feature1;
		string featr2_str = SimpleAnomalyDetector::cf[i].feature2;
		//find the correct columns in the time series:
		int feat_1_index = ts.get_index_of_feature(featr1_str);
		int feat_2_index = ts.get_index_of_feature(featr2_str);
		//get correct column by index
		float* f1 = ts.get_col_by_index(feat_1_index);
		float* f2 = ts.get_col_by_index(feat_2_index);
		//add values to array --> all the (x,y) such that x is from feature1 and y is from feature2
		for (int j=0; j<feature_size; j++) {
			float x = f1[j];
			float y = f2[j];
			p_array[j] = new Point(x,y);
		}
		vec_of_point_arrays.push_back(p_array);
	}
	
	for (int i=0; i<NUM_OF_CORRELATED_FEATURES; i++) {
		//calculate linear regression for each couple of correlated features and save line.
		SimpleAnomalyDetector::cf[i].lin_reg = linear_reg(vec_of_point_arrays[i], feature_size);
	}

	//for all arrays of points and their linear reg line, calculate the max deviation
	for (int i = 0; i < NUM_OF_CORRELATED_FEATURES; i++) {
		//intialize max deviation for each correlated features.
		float max_deviation = 0.0;
		for (int j = 0; j<feature_size; j++) {
			float deviation;
			Point* p = vec_of_point_arrays[i][j];
			Line l_reg = SimpleAnomalyDetector::cf[i].lin_reg;
			deviation = dev(*p, l_reg);
			if (max_deviation < deviation) {
				max_deviation = deviation;
			}
		}
		//update max deviation for the correlated features.
		SimpleAnomalyDetector::cf[i].threshold = max_deviation; 
	}
	//free allocated point arrays for each couple of correlated features.
	for (int i =0; i< NUM_OF_CORRELATED_FEATURES; i++) {
		delete[] vec_of_point_arrays[i];
	}
	
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	vector<AnomalyReport> j; 
	return j;
}
