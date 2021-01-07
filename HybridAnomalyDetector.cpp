
#include "HybridAnomalyDetector.h"
#include "minCircle.h"
HybridAnomalyDetector::HybridAnomalyDetector() : SimpleAnomalyDetector(){};
HybridAnomalyDetector::~HybridAnomalyDetector(){};
/**
 * @brief Updates the member cf according to the minimum circle algorithm
 * 
 * @param ts 
 */
void HybridAnomalyDetector::learnNormal(const TimeSeries &ts)
{
	SimpleAnomalyDetector::learnNormal(ts); // calculate all
	const int feature_size = ts.get_num_of_rows();
	const int num_of_features = ts.get_num_of_features();
	size_t cf_size = SimpleAnomalyDetector::cf.size();
	for (int i = 0; i < cf_size; i++)
	{
		if (SimpleAnomalyDetector::cf[i].corrlation >= 0.9)
		{
			continue;
		}
		int feat_1_index = ts.get_index_of_feature(SimpleAnomalyDetector::cf[i].feature1); // find index of feature 1 in the time series
		int feat_2_index = ts.get_index_of_feature(SimpleAnomalyDetector::cf[i].feature2); // find index of feature 2 in the time series
		float *f1 = ts.get_col_by_index(feat_1_index);									   // get the feature1 (column-data) by index
		float *f2 = ts.get_col_by_index(feat_2_index);									   // get the feature2 (column-data) by index
		Point **points = new Point *[feature_size];
		for (int j = 0; j < feature_size; j++)
		{
			points[j] = new Point(f1[j], f2[j]);
		}

		Circle enclosing_circle = findMinCircle(points, feature_size);
		SimpleAnomalyDetector::cf[i].center = &enclosing_circle.center;
		SimpleAnomalyDetector::cf[i].threshold = enclosing_circle.radius * 1.1; // the radius is the threshold (mult by 1.1 for safety)
		SimpleAnomalyDetector::cf[i].is_hybrid = true;							// this was updated in derived class

		//free space
		for (int j = 0; j < feature_size; j++)
		{
			delete points[j];
		}
		delete points;
	}
}
