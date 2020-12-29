//207681354

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "anomaly_detection_util.h"
using namespace std;

// ------------ DO NOT CHANGE -----------
/*
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};
*/
class Circle
{
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};

Point mid_point(Point *p1, Point *p2);
float distance(Point p1, Point p2);
Point *calculate_center(float a, float b, float c, float d, float e, float f);
Circle calculate_circle_by_3_points(Point *p1, Point *p2, Point *p3);
bool is_point_inside_circle(Circle c, Point p);
Circle min_circle(vector<Point *> points, vector<Point *> circumference, int n);
Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
