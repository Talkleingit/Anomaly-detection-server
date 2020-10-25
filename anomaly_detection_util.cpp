#include <iostream>
#include <cmath>
/**
 * @brief calculates variance
 *
 * @param x
 * @param size
 * @return float
 */
float var(float* x, int size) {
	float sAvg = 0, mue = 0;
	//calculating the sqaured expectancy
	for (int i = 0; i < size; i++) {
		sAvg += x[i] * x[i];
	}
	sAvg = sAvg / size;
	//calculating the expectancy
	for (int i = 0; i < size; i++) {
		mue += x[i];
	}
	mue = mue / size;
	return sAvg - (mue * mue);
}
/**
 * @brief calculates covariance
 *
 * @param x
 * @param y
 * @param size
 * @return float
 */
float cov(float* x, float* y, int size) {
	float xyAvg = 0, xAvg = 0, yAvg = 0;
	//calculating E(XY)
	for (int i = 0; i < size; i++) {
		xyAvg += x[i] * y[i];
	}
	xyAvg = xyAvg / size;
	//calculating E(x)
	for (int i = 0; i < size; i++) {
		xAvg += x[i];
	}
	xAvg = xAvg / size;
	//calculating E(y)
	for (int i = 0; i < size; i++) {
		yAvg += y[i];
	}
	yAvg = yAvg / size;
	return xyAvg - (xAvg * yAvg);
}
/**
 * @brief calculates pearson correlation
 *
 * @param x
 * @param y
 * @param size
 * @return float
 */
float pearson(float* x, float* y, int size) {
	float sigmaX = var(x, size);
	float sigmaY = var(y, size);
	float correlation = 0;
	//standard deviations
	sigmaX = sqrt(sigmaX);
	sigmaY = sqrt(sigmaY);
	//pearson correlation
	correlation = cov(x, y, size) / (sigmaX * sigmaY);
	return correlation;

}
class Line {
public:
	const float a, b;
	Line(float a, float b) :a(a), b(b) {}
	float f(float x) {
		return a * x + b;
	}
};
class Point {
public:
	const float x, y;
	Point(float x, float y) :x(x), y(x) {}
};
/**
 * @brief performs linear regression and returns a line as output.
 *
 * @param points
 * @param size
 * @return Line
 */
Line linear_reg(Point** points, int size) {
	float xCoordinate[size];
	float yCoordinate[size];
	float a = 0, b = 0, xAvg = 0, yAvg = 0;
	//create 2 arrays (one for x coordinates and one for the y coordinates).
	for (int i = 0; i < size; i++) {
		xCoordinate[i] = points[i]->x;
		yCoordinate[i] = points[i]->y;
	}
	//known identity: slope is the cov(x,y) / var(x). the lengths of both arrays are size
	a = cov(xCoordinate, yCoordinate, size) / var(xCoordinate, size);
	//calculating avg of X and Y
	for (int i = 0; i < size; i++) {
		xAvg += xCoordinate[i];
		yAvg += yCoordinate[i];
	}
	xAvg = xAvg / (size);
	yAvg = yAvg / (size);
	//known identity
	b = yAvg - a * xAvg;
	//create result line
	Line line(a, b);
	return line;
}
/**
 * @brief calculates the deviation between a given point and a 2d array of points.
 *
 * @param p
 * @param points
 * @param size
 * @return float
 */
float dev(Point p, Point** points, int size) {
	float deviation = 0;
	Line line = linear_reg(points, size);
	//using x cooridinate from the point p to determine the y value on the line equation.
	float yCooridinate = line.a * p.x + line.b;
	deviation = std::abs(yCooridinate - p.y);
	return deviation;

}
/**
 * @brief returns the deviation between point p and the line
 *
 * @param p
 * @param l
 * @return float
 */
float dev(Point p, Line l) {
	float yCooridinate = l.a * p.x + l.b;
	return std::abs(yCooridinate - p.y);
}
void say_hello() {
	std::cout << "Hello, from anomaly_detection_util!\n";
}
