#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

// Utilizes global variables in order to access rm and medv throughout
//  the program.
vector<double> rm(1000);
vector<double> medv(1000);

void readAndProcessCSV(){
    ifstream inFS;
    string line;
    string rm_in, medv_in;

    // Create an input stream and have it open Boston.csv
    inFS.open("data/Boston.csv");
    if(!inFS.is_open()){
        // Throw an error in case Boston.csv cannot be found
        cout << "Could not find or open the file Boston.csv";
        exit(1);
    }

    // Eliminates the first line of Boston.csv to ignore headers
    getline(inFS, line);
    int idx = 0;
    // Loop through the entire CSV, putting the data in their
    //  respective index
    while (inFS.good()){
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(idx) = stof(rm_in);
        medv.at(idx) = stof(medv_in);
        idx++;
    }

    // Resize each vector so that the remaining cells are not 0
    rm.resize(idx);
    medv.resize(idx);
    // Close the input stream
    inFS.close();
}

// Function to find the sum of a vector
//  returns the sum of a vector as a double
double sumOfNumericVector(vector<double> inputVector){
    // Loop through the vector in order to find the sum of the vector
    double total = 0;
    for (double val : inputVector){
        total += val;
    }
    return total;
}

// Function to find the mean of the vector
//  returns the mean of a vector as a double
double meanOfNumericVector(vector<double> inputVector){
    // Find sum of vector
    double total = sumOfNumericVector(inputVector);
    // Divide sum of vector by size of vector
    return total / inputVector.size();
};

// Function to find median of vector
//  returns the median of the vector
double medianOfNumericVector(vector<double> inputVector){
    // Sort the input array using bubble sort
    for(int i = 0; i < inputVector.size() - 1; i++){
        for(int j = i+1; j < inputVector.size(); j++){
            if(inputVector[i] > inputVector[j]){
                double temp = inputVector[i];
                inputVector[i] = inputVector[j];
                inputVector[j] = temp;
            }
        }
    }
    // Return the middle index of the array
    return inputVector[inputVector.size() / 2];
}

// Function to find range of the vector
//  returns the range of the vector
double rangeOfNumericVector(vector<double> inputVector){
    // Edge case in case the input vector is empty
    if (inputVector.size() == 0){
        return 0;
    }

    // Assume that the first element is both the max and min
    double min = inputVector[0];
    double max = inputVector[0];

    // Loop through the entire array, looking for the max and min
    for(double val : inputVector){
        if(val >= max){
            max = val;
        }
        if(val <= min){
            min = val;
        }
    }

    // Return the range (max-min)
    return max-min;
}

// Function to find the covariance of 2 vectors
//  This function assumes that both vectors are of equal size
//  Paramters are vector 1 and vector 2
//  Returns the covariance of the 2 vectors
double covarianceOfVectors(vector<double> x, vector<double> y){
    // Use the mean function written earlier to calculate their mean
    double xMean = meanOfNumericVector(x);
    double yMean = meanOfNumericVector(y);
    // Find covariance using covariance formula
    double covariance = 0;
    // Loop through both vectors, calculating the difference between 
    //  the value within the vector and it's index. The calculations
    //  occur for both vector 1 and vector 2.
    for (int i = 0; i < x.size(); i++){
        covariance += (x[i] - xMean) * (y[i] - yMean);
    }
    // Divide the calculated number by the size of the vector - 1
    covariance /= (x.size() - 1);
    // Return the calculated value
    return covariance;
}

// Function to find the correlation between the 2 vectors
//  Returns the correlation between 2 vectors
double correlationOfvectors(vector<double> x, vector<double> y){
    // Uses the equation Cov(x,y) / (sigma(x) * sigma(y))
    //  This assumes that sigma of a vector is the 
    //  square root of variance(vector,vector)
    return covarianceOfVectors(x,y) / (sqrt(covarianceOfVectors(x,x)) * sqrt(covarianceOfVectors(y,y)));
}

int main() {
    // Find the CSV and store the data into the global variables
    readAndProcessCSV();

    // Prints data from the rm vector
    cout << "Data from rm vector: ";
    cout << "\n\tSum: " << sumOfNumericVector(rm);
    cout << "\n\tMean: " << meanOfNumericVector(rm);
    cout << "\n\tMedian: " << medianOfNumericVector(rm);
    cout << "\n\tRange: " << rangeOfNumericVector(rm) << "\n";

    // Prints data from the medv vector
    cout << "\nData from medv vector: ";
    cout << "\n\tSum: " << sumOfNumericVector(medv);
    cout << "\n\tMean: " << meanOfNumericVector(medv);
    cout << "\n\tMedian: " << medianOfNumericVector(medv);
    cout << "\n\tRange: " << rangeOfNumericVector(medv) << "\n\n";

    // Prints the covariance and correlation of the vectors
    cout << "Covariance of rm and medv vector: " << covarianceOfVectors(rm,medv) << "\n";
    cout << "Correlation of rm and medv vector: " << correlationOfvectors(rm,medv) << "\n";

    // Returns with exit code 0 as successful
    return 0;
}
