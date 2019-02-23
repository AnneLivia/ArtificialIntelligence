#include <iostream>
#include <vector> // Used to store the dataset
#include <algorithm> // used to sort the a data structure
#include <fstream> // used to read a file
#include <cmath> // used to use the function of sqrt to calculated the similarity of classes using euclidean distance

using namespace std;

/*
    K Nearest Neighbors (KNN) - is one of the simplest algorithms from Machine Learning.
    It's a supervised learning algorithm, where, we already have prior knowledge of what
    the output values for our samples should be.
    It's used both for classification and regression problems, although it is far more popularly
    used for classification problems.

    Supervised x unsupervised learning:
        Supervised learning:
            learn a function that, given a sample data and desire outputs
            best approximates the relationship between input and output observable data.
            Supervised is done in the context of classification and regression
            the goal is to find specific relationships or structure in the input data that
            allow us to effectively produce correct output data, using a training data.
        Unsupervised Learning:
            does not have labeled outputs, so its goal is to infer the natural
            structure present within a set of data points.
            It's called unsupervised because it starts with unlabeled data (there's no Y here).
            Unlikely supervised, unsupervised cannot be applied to regression and classification
            because we don't have idea what the values for the output might be, making it impossible for us
            to train the algorithm in the way we normally do with supervised learning problems.
            Unsupervised learning can be used for discovering the underlying structure of the data.
            Since we don't know what the outcomes should be, we cannot determine how accurate the results found
            are.

            The best time to use unsupervised machine learning is when you don't have data on desired outcomes,
            like determining a target market for an entirely new product that your business has never sold before.
            However, if you are trying to get a better understanding of your existing consumer base, supervised
            learning is the optimal technique.

            Unsupervised can be divided into two tasks, Clustering which puts the data into groups
            by similarity and the reducing dimensionality that compress the data while maintaining its
            structure and usefulness.

    Both classification and regression share the common concept of using past data to make
    prediction, or take decisions.

    Classification problems:
        classifies data into different categories
        it predicts a discrete number of values (A discrete data can only take particular values.
        there may be numbers, categories such as red or blue, male or female, etc.
        one example could be the number of patients in a hospital, it cannot half a patient).
        Example of classification problems: whether an email is spam or not, where a cancer is benign
        or malignant, if an image has cats or dogs, etc.
    Types of classification:
        Binary: where there are only two classes to predict, usually 1 or 0 values
        Multi-class: When there are more than two classes labels to predict, we call it
        a multi-classification task. The problem used in this program is a multi-class,
        it predicts 3 types of iris species, we also can have as example of multi-classification, the
        images classification problem where there are more than thousands classes (cat, dog, fish, car, ...).
    Algorithm for classification:
        Decision Trees, logistic regression, naive bayes, linear svc, and many others, the one that we are going
        to use here is also one of them.

    Regression problems:
        predict a value based on past data.
        In it we try to predict continuous valued output (a continuous data can occupy any value over a continuous range,
        in between two continuous data values, there are an infinite number of other, continuous data are always essentially
        numeric, two examples could be: the weight of a person, the dog's weight, price of a house, etc.).
        so given a size of some specific house, predict the price (real value).
    Algorithms:
        Linear regression, regression trees, support vector regression, etc.

    Classification: Discrete values (1,2,3,4).
    Regression: Continuous values (1.22, 222.3, 544.33)

    K Nearest Neighbors:
        It's a non-parametric (does not make any assumption on the underlying data),
        and lazy algorithm (it doesn't learn a discriminative function from the training
        data but memorizes the training dataset instead).
        KNN works by finding the distances between a query and all the examples in the data,
        selecting the specified number examples (K) closest to the query, then votes for the
        most frequent label (in the case of classification) or averages the labels (in the case of regression).
        The algorithm uses "feature similarity" to predict values of any new data points.
        This means that the new point is assigned a value based on how closely it resembles the points in the training set.
        k is therefore just the number of neighbors "voting" on the test example's class.
        Historically, the optimal K for most datasets has been between 3-10.

        The algorithm can be summarized as:
            1. Load the training and test data
            2. Choose the value of K
            3. For each point in test data:
               - find the Euclidean distance to all training data points
               - store the Euclidean distances in a list and sort it
               - choose the first k points
               - assign a class to the test point based on the majority of classes present in the chosen points
            4. End

        Dealing with ties:
            What happens if there is no clear winner in the majority voting?
            One possible way is to have the algorithm automatically increase or decrease k until you get a clear winner.
            It is good to take odd values for binary classification to avoid the ties i.e.
            two classes labels achieving the same score.

        The similarity of one class to its k nearest neighbors is calculated using a distance function, it can be:
            euclidean distance, Manhattan distance, minkowski distance,k and so on;

    The iris data set has 3 classes, where each class refers to a type of iris plant:
        Iris Setosa
        Iris Versicolous
        Iris Virginica
    It uses four attributes to classify the plants:
        Sepal length in cm
        Sepal width in cm
        Petal length in cm
        petal width in cm
    In this program we are going to divide the dataset into two groups:
        training set - a subset to train the model
        test set - a subset to test the trained model

    In Machine Learning, we basically try to create a model to predict on the test data.
    So, we use the training data to fit the model and testing data to test it.

*/

// Structure to keep track of all the attributes of a Iris
struct Iris {
    double sepalLength, sepalWidth, petalLength, petalWidth;
    string className;
};

// KNN class to perform all functions:
// calculate similarity (Euclidean distance)
// define its class based on the similarity calculation and using the K neighbors
class Knn {
private:
    vector<Iris>trainingSet; // vector to store the training set
    int k; // integer to store the number of neighbors
    int trainingElementsNumber; // integer to store the number of training elements (basically the trainingSet.size())
public:
    Knn(vector<Iris>& trainingSet, int k, int trainingElementsNumber) :
        trainingSet(trainingSet), k(k), trainingElementsNumber(trainingElementsNumber) {
            // It's good to choose a odd number for k, to avoid a tie, so, in case the number if even
            if(k % 2 == 0)
                k--; // decrement it
            if(k <= 0) // if it becomes equal or less than 0
                k = 1; // then assign it to one
        }
    // Start of getter and setter methods
    void setK(int k) {
        this->k = k;
    }

    int getK() {
        return this->k;
    }

    void setTrainingElementsNumber(int trainingElementsNumber) {
        this->trainingElementsNumber = trainingElementsNumber;
    }

    int getTrainingElementsNumber() {
        return this->trainingElementsNumber;
    }

    void setTraining(vector<Iris>& trainingSet) {
        this->trainingSet = trainingSet;
    }

    vector<Iris> getTrainingSet() {
        return this->trainingSet;
    }
    // End of getter and setter methods

    // method to return the euclidean distance which is
    // the square root of the sum of the differences of the attributes to the power of two
    float getEuclideanDistance(Iris ir1, Iris ir2) {
        double dist = sqrt(
            pow(ir1.sepalLength - ir2.sepalLength, 2) +
            pow(ir1.sepalWidth - ir2.sepalWidth, 2) +
            pow(ir1.petalLength - ir2.petalLength, 2) +
            pow(ir1.petalWidth- ir2.petalWidth, 2)
        );
        return dist;
    }

    // Method to return the class of the data pass as argument to the method
    string classify(Iris& ir) {
        // Vector to store the euclidean distance and the index of the data
        vector<pair<double, int> > euclideanDistance;
        // Running through each element of the training set and calculating the similarity of
        // each one with the argument data and inserting it into the vector "euclideanDistance"
        for (int i = 0; i < this->trainingElementsNumber; i++) {
            euclideanDistance.push_back({getEuclideanDistance(trainingSet[i], ir), i});
        }

        // Sort the vector, the lesser the euclidean distance, the closer is the data to the other data
        sort(euclideanDistance.begin(), euclideanDistance.end());

        // Variables to keep track of how many of class there is in the k nearest to the data
        int setosa = 0, virginica = 0, versicolor = 0;

        // Running through each one of the k nearest neighbors
        for (int i = 0; i < this->k; i++) {
            // Getting the index of the iris that is closer to the data passed to the function
            int index = euclideanDistance[i].second;
            // Storing the name of the class that is closer to the data passed to the function
            string plantClass = trainingSet[index].className;
            // Check which one is it and increment the number of the one found
            if(plantClass == "Iris-setosa")
                setosa++;
            else if (plantClass == "Iris-versicolor")
                versicolor++;
            else if (plantClass == "Iris-virginica")
                virginica++;
        }

        // Since its a classification problem, get the one that repeat more and return
        if(virginica > versicolor && virginica > setosa)
            return "Iris-virginica";
        else if (versicolor > virginica && versicolor > setosa)
            return "Iris-versicolor";
        else
            return "Iris-setosa";
    }
};

int main()
{
    /*  ofstream = output, ifstream = input
        ofstream:
            This data type represents the output file
            stream and is used to create files and to
            write information to files.
        ifstream :
            This data type represents the input file stream
            and is used to read information from files.
        fstream :
            This data type represents the file stream
            generally, and has the capabilities of both ofstream
            and ifstream which means it can create files,
            write information to files, and read information from files.
    */

    // Open the file to get the dataset that is going to be used both for train and test
    ifstream f;
    f.open("IrisDataSet.txt");

    // If it were found an error when open the file
    if(f.fail()) {
        cerr << "Data set was not loaded correctly\n";
        exit(1);
    }

    // Variables used to store each attribute from the file
    string className;
    double sl, sw, pl, pw;

    // data structure that keeps track of all data used in this program
    vector<Iris>fullDataSet;

    // While not end of file
    while(!f.eof()) {
        // Getting from the file each attribute
        // Sepal length and width, petal length and width and the iris class name: virginica, versicolor or setosa
        f >> sl >> sw >> pl >> pw >> className;
        // Inserting it into the vector
        fullDataSet.push_back({sl,sw,pl,pw,className});
    }
    // Closing the file since the data was already read and store to be used
    f.close();

    // Getting from the user the number of training data and the number of neighbors to check (k)
    int trainingNumber, k, testNumber;
    cout << "Enter the number of training data: ";
    cin >> trainingNumber;
    cout << "Enter the number of k (neighbors): ";
    cin >> k;

    // The number of tests is going to be what is left, the total number of data provided minus the
    // number of training data
    testNumber = (int)fullDataSet.size() - trainingNumber;

    // if the number of test is less or equal to 0, it means that there's any data left to test, or
    // the number of training data typed is greater than the number of data from the dataset, or the
    // number of data is equal to the number of training typed
    if(testNumber <= 0) {
        cout << "Not enough data to test\n";
        exit(1);
    }

    // In case everything is okay, then create a vector to store the data that is going to be used
    // for the training
    vector<Iris>aux;
    // Running through the first trainingNumber elements
    for (int i = 0; i < trainingNumber; i++) {
        // Inserting each one of the attributes
        aux.push_back(
        {
            fullDataSet[i].sepalLength,
            fullDataSet[i].sepalWidth,
            fullDataSet[i].petalLength,
            fullDataSet[i].petalWidth,
            fullDataSet[i].className
        });
    }

    // creating an instance of the class KNN and pass as argument to the constructor
    // the vector with all the data to be used for the training phase, the k and the
    // number of training elements
    Knn classify(aux, k, trainingNumber);

    // Variable to store the number of correct answers
    int rightAnswer = 0;

    // Running through all elements left from all data set for the test phase
    for (int i = trainingNumber; i < trainingNumber + testNumber; i++) {
        // Getting the name of the class that most appeared in the k nearest for the current i data
        string classPlant = classify.classify(fullDataSet[i]);
        // if the class get is equal to the real one, then increment the number of correct answers
        if(classPlant == fullDataSet[i].className)
            rightAnswer++;

        cout << "Test " << i - trainingNumber + 1 << ":\n";
        cout << "Expected: " << fullDataSet[i].className << ", got: " << classPlant << endl;
    }

    cout << "Number of correct answers: " << rightAnswer << " out of " << testNumber << endl;
    return 0;
}
