#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct Variable {
    double x, y;

    operator < (const Variable& v) {
        return this->x < v.x;
    }
};

int main()
{
    ifstream dataset("data/house_data.csv");
    ofstream savedata("data/values.txt");

    vector<Variable> values;

    int x = 0, y = 0;
    string x_name = "price";
    string y_name = "sqft_living";

    bool foundxandy = false;

    string line;
    while(dataset >> line) {

        stringstream str(line);
        string data;
        int i = 1;
        Variable aux = {0, 0};
        // what is the impact of the area in the price
        while(getline(str, data, ',')) {
            if (x == 0 || y == 0) {
                if (data == x_name) {
                    x = i;
                }

                if (data == y_name) {
                    y = i;
                }


            } else {
                foundxandy = true;

                // already found x and y coll
                if (x == i) {
                    aux.x = stod(data);
                }

                if (y == i) {
                    aux.y = stod(data);
                }

            }
            i++;
        }

        if (foundxandy)
            values.push_back(aux);
    }

    double sum_xmeanx_ymeany = 0.0, sum_xmeanx_pow2 = 0.0, meanx = 0.0, meany = 0.0;

    for (int i = 0; i < 500; i++) {
        savedata << values[i].x << '\r';
        meanx+=values[i].x;
        meany+=values[i].y;
    }


    meanx/=500;
    meany/=500;

    for (int i = 0; i < 500; i++) {
        sum_xmeanx_ymeany+=((values[i].x - meanx) * (values[i].y - meany));
        sum_xmeanx_pow2+=pow(values[i].x - meanx, 2);
    }

    double m = sum_xmeanx_ymeany / sum_xmeanx_pow2;

    double b = meany - m * meanx;

    cout << "Y = " << m << "x + " << b << endl;

    savedata.close();
    dataset.close();
    return 0;
}
