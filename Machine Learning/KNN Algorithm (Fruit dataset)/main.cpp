#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

// map to facilitate calculation when checking class and subclass
map<string, int> map_class_subclass;

struct Fruit {
    string classe, subclasse;
    int mass, fruit_label;
    float width, height, color_score;
};


class Knn {
private:
    vector<Fruit>training_set;
    int k; // k nearest neighbors
public:
    Knn(vector<Fruit>training_set, int k) : training_set(training_set), k(k) {
        // K can't be less than or equal to 0, so assign 1 in that case
        // sometime the best k value is choose in try and error
        // the hight value of K, less chance of error
        // odd number is sometimes used to avoid confusion between 2 classes
        // sqrt(n), where n is the total number of data point can be choose too
        if (k <= 0)
            k = 1;
    }

    void setK(int k) {
        this->k = k;
    }

    int getK() const {
        return this->k;
    }

    void setTraining_set(vector<Fruit>& training_set) {
        this->training_set = training_set;
    }

    vector<Fruit> getTraining_set() {
        return this->training_set;
    }

    // many distance methods can be use instead of euclidean
    double euclidean_distance(Fruit f1, Fruit f2) {
        return sqrt(pow(f2.mass - f1.mass, 2) +
                    pow(f2.width - f1.width, 2) +
                    pow(f2.height - f1.height, 2));
    }

    // return class and subclass that some data belongs to
    string classify(Fruit& f1) {
        // stores distances and index of fruits
        vector<pair<double, int> > distances;

        cout << "For: " << f1.classe << " " << f1.subclasse << endl;
        // running through each element and calculating distance from argument
        for (int i = 0; i < (int)this->training_set.size(); i++) {
            distances.push_back({euclidean_distance(training_set[i], f1), i});
            cout << euclidean_distance(training_set[i], f1) << " " <<
            training_set[i].classe << " " << training_set[i].subclasse << endl;
        }

        cout << endl;

        // sorting by the nearest (that is, the smallest the value, the closer is the data)
        sort(distances.begin(), distances.end());

        map_class_subclass.clear();

        // calculating maximum k number of elements near
        // calculating maximum k number of elements near
        for (int i = 0; i < (int)this->k; i++) {
            string aux = this->training_set[distances[i].second].classe;

            if (map_class_subclass.find(aux) == map_class_subclass.end()) {
                // if it got here, element is not yet in the map to calculate
                map_class_subclass.insert({aux, 1});
            } else {
                // just increment
                map_class_subclass.at(aux)++;
            }
        }

        // when finish, need to check which one of the classes got more closer to the data
        string closer;
        int maxx = 0;
        for(auto m : map_class_subclass) {
            if (maxx < m.second) {
                closer = m.first;
                maxx = m.second;
            }
        }

        return closer;
    }
};
int main()
{
    // reading data and storing in a array of fruit
    vector<Fruit>dataset;
    ifstream datafile("fruit.txt");
    string data;
    int jump = 0;
    while(getline(datafile, data)) {
        // in order to not save the columns name
        if (!jump) {
            jump = 1;
            continue;
        }

        string info;
        Fruit f;
        stringstream str(data);
        int qtd = 0;
        while(getline(str, info, ',')) {
            if (qtd == 1) {
                f.classe = info;
            } else if (qtd == 2) {
                f.subclasse = info;
            } else if (qtd == 3) {
                f.mass = stoi(info);
            } else if (qtd == 4) {
                f.width = stof(info);
            } else if (qtd == 5) {
                f.height = stof(info);
            } else if (qtd == 6) {
                f.color_score = stof(info);
            }
            qtd++;
        }
        dataset.push_back(f);
    }


    int k, training_number, test_number;
    cout << "Type k: ";
    cin >> k;
    cout << "Type training number: ";
    cin >> training_number;

    vector<Fruit>training_data;
    for (int i = 0; i < training_number; i++) {
        training_data.push_back(dataset[i]);
    }

    test_number = (int)dataset.size() - training_number;

    Knn knn(training_data, k);

    int right_answers = 0, test_id = 1;
    for (int i = training_number; i < (int)dataset.size(); i++) {
        string classification = knn.classify(dataset[i]);

        if (classification == dataset[i].classe) {
            right_answers++;
        }

        cout << "Test: " << test_id++ << endl;
        cout << "Right Answer: " << dataset[i].classe << endl;
        cout << "Got: " << classification << endl;

    }

    cout << "Accuracy: " << right_answers * 100.00 / test_number << endl;
    return 0;
}
