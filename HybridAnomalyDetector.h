

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"

#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "anomaly_detection_util.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector
{
	vector<correlatedFeatures> cf1;

public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	void learnNormal(const TimeSeries &ts);
	//	vector<AnomalyReport> detect(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
