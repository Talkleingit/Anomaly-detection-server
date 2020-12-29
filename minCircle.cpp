#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "minCircle.h"
using namespace std;
/**
 * @brief returns the middle point between 2 points.
 * 
 * @param p1 
 * @param p2 
 * @return Point 
 */
Point mid_point(Point *p1, Point *p2)
{
    float x = (p1->x + p2->x) / 2;
    float y = (p1->y + p2->y) / 2;
    Point mid(x, y);
    return mid;
}

/**
 * @brief Distance between 2 points is defined as the sqrt((x2-x1)^2 + (y2-y1)^2)
 * 
 * @param p1 
 * @param p2 
 * @return float 
 */
float distance(Point p1, Point p2)
{
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) +
                ((p2.y - p1.y) * (p2.y - p1.y)));
}
/**
 * @brief Given three points p1 = (a,b), p2 = (c,d), p3 = (e,f). We can conclude that the distance from the radius of a 
          circle created by the three points to one of the points is the the exact same as to the other 2.
          Let (x,y) be the center of the Circle created by the 3 points. So we can create 2 equations:
          dis((x,y),p1) = dis((x,y),p2) and dis((x,y),p1) = dis((x,y),p3) ---> and therefore isolate x and y.
 * 
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param e 
 * @param f 
 * @return float 
 */
Point *calculate_center(float a, float b, float c, float d, float e, float f)
{
    float expression1 = ((e * e) + (f * f) - (a * a) - (b * b)) * (c - a);
    float expression2 = ((c * c) + (d * d) - (a * a) - (b * b)) * (e - a);
    float numerator = expression1 - expression2;
    float denominator = 2 * (((b - d) * (e - a)) - ((b - f) * (c - a)));
    float y_coordinate = numerator / denominator;                                                                // calcualte the y coordinate of the center point
    float x_coordinate = ((c * c) + (d * d) - (a * a) - (b * b) + y_coordinate * (2 * (b - d))) / (2 * (c - a)); //calculate the x coordinate of the center point
    Point *center = new Point(x_coordinate, y_coordinate);
    return center;
}

/**
 * @brief returns a circle by 3 given points.
 * 
 * @param p1 
 * @param p2 
 * @param p3 
 * @return Circle 
 */
Circle calculate_circle_by_3_points(Point *p1, Point *p2, Point *p3)
{
    Point *center = calculate_center(p1->x, p1->y, p2->x, p2->y, p3->x, p3->y); //calculate center by 3 points
    float radius = distance(*center, *p1);                                      //calculate radius as the distance from the center to one of the points
    Circle circle(*center, radius);
    return circle;
}
/**
 * @brief verifies if the circle c encloses the given point p.
 * 
 * @param c 
 * @param p 
 * @return true 
 * @return false 
 */
bool is_point_inside_circle(Circle c, Point p)
{
    if (distance(c.center, p) > c.radius)
    {
        return false;
    }
    return true;
}
/**
 * @brief recursively calculates the minimum circle enclosing all points in the points vector
 * 
 * @param points 
 * @param circumference 
 * @return Circle 
 */
Circle min_circle(vector<Point *> points, vector<Point *> circumference, int n)
{
    if (n == 0)
    { //base case of the recursion --> array of points is empty
        if (circumference.size() == 0)
        {
            Point *p = new Point(0, 0);
            Circle c(*p, 0);
            return c;
        }
        else if (circumference.size() == 1)
        { //trivial
            Circle mincircle(*(circumference[0]), 0);
            return mincircle;
        }
        else if (circumference.size() == 2)
        { //circle made of 2 points
            Point center = mid_point(circumference[0], circumference[1]);
            float radius = distance(center, *circumference[1]); //radius is the distance from the center to the circumference
            Circle mincircle(center, radius);
            return mincircle;
        }
        else
        {
            Circle c = calculate_circle_by_3_points(circumference[0], circumference[1], circumference[2]); //circle of 3 points
            return c;
        }
    }
    if (circumference.size() == 3)
    { //base case: 3 points that lay on the circumference
        return calculate_circle_by_3_points(circumference[0], circumference[1], circumference[2]);
    }
    Point *p = points[n - 1];                                      //pick some arbitrary point from points
    points.pop_back();                                             //remove the arbitrary point
    n = n - 1;                                                     //update number of points in points
    Circle min_c_without_p = min_circle(points, circumference, n); //find the minimum circle using recursion for the set of points without p
    if (is_point_inside_circle(min_c_without_p, *p))
    {                           //verify if point is within the boundries of the circle
        return min_c_without_p; //then we can conclude the point is not on the circumfernece of the min circle enclosing all points.
    }
    else
    { //we will define that the point p will be set to lay on the circumference of the min circle of all points in the intiall vector points
        circumference.push_back(p);
        return min_circle(points, circumference, n);
    }
}

/**
 * @brief return the minimum circle enclosing all points in the vector points.
 * 
 * @param points 
 * @param size 
 * @return Circle 
 */
Circle findMinCircle(Point **points, size_t size)
{
    vector<Point *> pnts;
    vector<Point *> circumf;
    for (size_t i = 0; i < size; i++)
    { //move all points to a vector
        pnts.push_back(points[i]);
    }
    int n = pnts.size();
    Circle c = min_circle(pnts, circumf, n); //save the minimum circle found by min_circle function
    return c;
}