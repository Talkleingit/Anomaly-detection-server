#include "SimpleAnomalyDetector.h"
#include <vector>
#include "anomaly_detection_util.h"
SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	

}
vector<vector<string>> get_correlated_features_vector(const TimeSeries& ts) {
	vector<vector<string>> correlated_vec;
	vector<string> features = ts.getFeatures();
	vector<string>::iterator fi = features.begin();
	vector<string>::iterator fj = features.begin();
	for (fi; fi!=features.end(); fi++) {
		for(fj; fj!=features.end(); fj++) {
			if (fi != fj) {
				
			}
		} 
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
}

