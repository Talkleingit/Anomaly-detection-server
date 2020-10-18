#include <iostream>
#include <math.h>
// returns the variance of X and Y
float var(float* x, int size) {
    float sAvg = 0, mue = 0;
    //calculating the sqaured expectancy
    for (int i =0; i< size; i++) {
        sAvg += x[i] * x[i];
    }
    sAvg = sAvg / size;
    //calculating the expectancy
    for (int i=0; i< size; i++) {
        mue += x[i];
    }
    mue = mue / size;
    return sAvg - (mue * mue);
}
// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float xyAvg =0, xAvg =0, yAvg =0;
    //calculating E(XY)
    for (int i=0; i<size; i++) {
        xyAvg += x[i] * y[i];
    }
    xyAvg = xyAvg / size;
    //calculating E(x)
     for (int i=0; i<size; i++) {
        xAvg += x[i];
    }
    xAvg = xAvg / size;
     //calculating E(y)
     for (int i=0; i<size; i++) {
        yAvg += y[i];
    }
    yAvg = yAvg / size;
    return xyAvg - (xAvg * yAvg);
}
// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    float sigmaX = var(x, size);
    float sigmaY = var(y, size);
    float correlation = 0;
    //standard deviations
    sigmaX = sqrt(sigmaX);
    sigmaY = sqrt(sigmaY);
    //pearson correlation
    correlation = cov(x,y,size) / (sigmaX * sigmaY);
    return correlation;

}
class Line{
public:
const float a,b;
Line(float a, float b):a(a),b(b){}
float f(float x){
return a*x+b;
}
};
class Point{
public:
const float x,y;
Point(float x, float y):x(x),y(x){}
};
// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    float* xCoordinate = nullptr;
    float* yCoordinate = nullptr;
    float a =0, b =0, xAvg =0, yAvg =0;
    int k =0;
    //create 2 arrays (one for x coordinates and one for the y coordinates).
    for (int i=0; i<size; i++) {
        for (int j =0; j<size; j++) {
            xCoordinate[k] = points[i,j]->x;
            yCoordinate[k] = points[i,j]->y;
            k = k+1;
        }
    }
    //known identity: slope is the cov(x,y) / var(x). the lengths of both arrays are size^2
    a = cov(xCoordinate, yCoordinate, size * size);
    //calculating avg of X and Y
    for (int i=0; i < size; i++) {
        xAvg += xCoordinate[i];
        yAvg += yCoordinate[i];
    }
    xAvg = xAvg / (size * size);
    yAvg = yAvg / (size * size);
    //known identity
    b = yAvg - a * xAvg;
    //create result line
    Line line(a,b);
    return line;

}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    float deviation = 0;
    for (int i=0; i < size; i++) {
        for (int j=0; j < size; j++) {
            if (p.x == points[i,j] -> x) {
                deviation = abs(points[i,j] ->y - p.y);
            }
        }
    }
    return deviation;
}
// returns the deviation between point p and the line
float dev(Point p,Line l);

void say_hello(){
    std::cout << "Hello, from anomaly_detection_util!\n";
}
