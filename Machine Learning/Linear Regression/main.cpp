#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
    Linear Regression is one of the simplest machine learning algorithm
    In statistics linear regression is a linear approach to model the relationship
    between one or more independent variables.
    Simple linear regression is when we have only one independent variable.
    Multiple linear regression is when we have more than one.

    For simple linear regression:
        having x as independent variable and y as dependent
        we must define the linear relationship between these variables with the equation
        Y = mX + c, where m is the angular coefficient and c is the linear coefficient
        that is, m defines the inclination of the line and c defines where the line intercepts
        the y coordinate

    In this machine learning algorithm, I'm going to use this equation to train a model
    with a given dataset to predict the value of Y for any given value of X.

    so the challenge is to find the value of m and c that minimizes the error for a given dataset.
    that is, the values that gives the best line fit. One way of doing it is by using the least square method.

    to minimize the error, we must find the error value with a loss function.
    A loss function is a machine learning function that measure how different is the predicted
    value to the actual value (that is the residual (observed value - adjusted value).

    there are many ways to calculate it, in this code, I'm going to use Quadratic loss function.
    Quadratic loss function = L(x) =  n
                                      sum(yi - Pi)^2, where yi = actual value, pi = predicted value
                                     i = 1
    the value is being squared to avoid negative values in the total error.

    Least square method: this method is used to minimize the values of m and c.
    there are a lot of math behind (derivative of loss function, equating it to 0 and find m and c)
    the found formula after all the math is:
                     n
               M =  sum  (xi - mean( all value in input x)) * (yi - mean(all value in input y))
                   i = 1
                 ______________________________________________________________________________

                                     n
                                    sum (xi - mean( all value in input x))^2
                                   i = n


              C = mean(all value in input y) - m * mean(all value in input x)

    n is the total number of value

    another way to write the formula found in math is fun is:

    M = n * sum(x,y) - sum(x) * sum(y)
    __________________________________
        n * sum(x^2) - (sum(x)^2)

    C = sum(y) - m * sum(x) / n
*/

class LinearRegression {
private:
    vector<double>x;
    vector<double>y;
    int n; // number of elements (basically x.size())
public:
    LinearRegression(vector<double> x, vector<double> y) : x(x), y(y) {
        this->n = (int)x.size();
    }

    void setX(vector<double> x) {
        this->x = x;
    }

    void setY(vector<double> y) {
        this->y = y;
    }

    vector<double> getX() const {
        return this->x;
    }

    vector<double> getY() const {
        return this->y;
    }

    double quadratic_loss_function(const vector<int>& predictedY) {
        double sum = 0.0;
        for (int i = 0; i < this->n; i++) {
            sum+=pow(this->y[i] - predictedY[i], 2);
        }
        return sum;
    }

    /* correlation:
        Correlation is a measure of how close the line fits the points
        the formula to calculate it is:
        R^2 = (n * sum(x * y) - sum (x) * sum (y)^2
             ___________________________________
             (N sum(x^2) - sum(x)^2) * (N * sum(y^2) - sum(y)^2)
        it varies from -1 to 1
        measure of the strength of a linear relationship between two quantitative variables
    */
    double correlation() {
        double sumxy = 0.0, sumx = 0.0, sumy = 0.0, sumx2 = 0.0, sumy2 = 0.0;
        for (int i = 0; i < this->n; i++) {
            sumxy+=this->x[i] * this->y[i];
            sumx+=this->x[i];
            sumy+=this->y[i];
            sumx2+=pow(this->x[i], 2);
            sumy2+=pow(this->y[i], 2);
        }

        return ((pow(this->n * sumxy - sumx * sumy, 2)) /
                     ((this->n * sumx2 - pow(sumx, 2)) * (this->n * sumy2 - pow(sumy, 2))));
    }

    // calculates mean
    double mean(const vector<double>& variable) {
        int sum = 0.0;
        for (int i = 0; i < this->n; i++) {
            sum+=variable[i];
        }
        return sum / this->n;
    }

    // return m and c from y = mX + c;
    pair<double, double> least_square1() {
        // all those steps bellow is the implementation of the formula above
        // that represents the least square method
        double sum_xmeanx_ymeany = 0.0, sum_xmeanx_pow2 = 0.0;
        for (int i = 0; i < this->n; i++) {
            sum_xmeanx_ymeany+=((this->x[i] - mean(this->x)) * (this->y[i] - mean(this->y)));
            sum_xmeanx_pow2+=pow(this->x[i] - mean(this->x), 2);

        }

        double m = sum_xmeanx_ymeany / sum_xmeanx_pow2;

        double c = (mean(y) - (m * mean(x)));

        return {m, c};
    }

    // return m and c from y = mX + c; formula from math is fun
    pair<double, double> least_square2() {
        // all those steps bellow is the implementation of the formula above
        // that represents the least square method
        double sum_xy = 0.0, sum_x = 0.0, sum_y = 0.0, sum_x2;
        for (int i = 0; i < this->n; i++) {
            sum_xy+=x[i]*y[i];
            sum_x+=x[i];
            sum_y+=y[i];
            sum_x2+=pow(x[i], 2);
        }

        double m = (this->n * sum_xy - sum_x * sum_y) / (this->n * sum_x2 - pow(sum_x, 2));

        double c = (sum_y - m * sum_x) / this->n;

        return {m, c};
    }
};


int main()
{

    // dataset to predict
    vector<double> x;
    vector<double> y;

    // reading data
    ifstream dataset("dataset/sampledata2.csv");

    string data, value;

    int jump = 1;
    while(dataset >> data) {
        stringstream str(data);
        // in order to not get text and try convert to number in sample data3.csv
        if (jump) {
            jump = 0;
            continue;
        }
        // to store first value in x and second in y
        int cont = 1;
        while(getline(str, value, ',')) {

            if (cont == 1) {
                x.push_back(stod(value));
            } else {
                y.push_back(stod(value));
            }
            cont++;
        }
    }

    dataset.close();

    for (int i = 0; i < (int)x.size(); i++) {
        cout << x[i] << " - " << y[i] << endl;
    }


    vector<double>auxx = {43, 21, 25, 42, 57, 59};
    vector<double>auxy = {99, 65, 79, 75, 87, 81};


    x = auxx;
    y = auxy;


    LinearRegression lr(x, y);

    pair<double, double> coefficients = lr.least_square2();

    cout << "Y = " << coefficients.first << " * x + " << coefficients.second << endl;
    cout << "R^2 = " << lr.correlation() << endl;

    // cout << "Y(5) = " << coefficients.first * 5 + coefficients.second << endl;

    Mat graph(Size(400, 400), CV_64FC3, Scalar(255,255,255));
    // y-axis
    line(graph, Point(200, 0), Point(200, 500), Scalar(255, 0, 0), 2);
    // x-axis
    line(graph, Point(0, 200), Point(500, 200), Scalar(255, 0, 0), 2);

    // plotting points
    for (int i = 0; i < (int)x.size(); i++) {
        line(graph, Point(x[i] + 200, 200 - y[i]), Point(x[i] + 200, 200 - y[i]), Scalar(0, 255, 0), 4);
    }

    // draw fit line
    line(graph, Point(x[0] + 200, 200 - (coefficients.first * x[0] + coefficients.second)),
                Point(x[(int)x.size() - 1] + 200, 200 - (coefficients.first * x[(int)x.size() - 1] + coefficients.second)), Scalar(0, 0, 255), 2);

    imshow("Graph", graph);
    waitKey();
    return 0;
}
