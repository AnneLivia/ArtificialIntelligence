#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*
    reference: Machine learning mastery
    Gradient decent is a way to calculate linear regression.

    Gradient descent is an optimization algorithm used to find the
    values of parameters (coefficients) of a function (f) that
    minimizes a cost function (cost).

    Gradient descent is best used when the parameters cannot be calculated analytically
    (e.g. using linear algebra) and must be searched for by an optimization algorithm.

    Gradient descent is important because sometimes we're gonna have a dataset with many
    variables, and a close formula is not going to solve it always.

    It does many smalls tries to check if the coefficients are the best fit

    gradient descent does a lot of guess to find the best fit

    error (residual) = y (real value) - guess

    the idea is to minimize the residual.

    we use the error to change the vector in the gradient descent

    this is supervised learning.

    we can start with random values for m and b and it repeats until it gets a good answer

*/

struct Variable {
    // can increase number of variables here
    double x, y;
};

class LinearRegression {
private:
    vector<Variable> data;
    int n;
    double m, b;

public:
    LinearRegression(vector<Variable> data) : data(data) {
        this->n = (int)data.size();
        m = 1; // random value
        b = 0; // random value
    }

    void setData(vector<Variable> data) {
        this->data = data;
    }

    vector<Variable> getData() const {
        return this->data;
    }

    int getSize() const {
        return this->n;
    }

    double getM() {
        return this->m;
    }

    void setM(double m) {
        this->m = m;
    }

    double getB() {
        return this->b;
    }

    void setB(double b) {
        this->b = b;
    }

    pair<double, double> gradientDescent() {
        // need to specify with which power the vector is going to turn i.e
        // how much is going to be the adjustment
        double learning_rate = 0.05;
        // need to run through all values
        // need to look the error (y - guess) and adjust M and B according to the error
        for (int i = 0; i < (int)this->n; i++) {
            double x = this->data[i].x;
            double y = this->data[i].y;
            // for each x and y there's a guess;
            double guess = m * x + b;
            // the real value - the guess
            double error = y - guess;

            // for every single point available, we need to change m and b

            m = (b + error * x) * learning_rate; // m can be adjusted by the error but multiplied by the x value
            b = (b + error)  * learning_rate; // b can be directly adjusted by the error because is just the y interception
        }

        return {this->m, this->b};
    }
};

int main()
{
    vector<Variable> data = {{43, 99}, {21, 65}, {25, 79}, {42, 75}, {57, 87}, {59, 81}};

    LinearRegression lr(data);

    pair<double, double> coeff = lr.gradientDescent();

    cout << "Y = " << coeff.first << "X + " << coeff.second << endl;
    return 0;
}
