//============================================================================
// Name        : naive_bayes.cpp
// Author      : Alejo Vinluan abv210001
// Version     :
// Copyright   : 
// Description : Naive Bayes from Scratch in C++
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <array>
#include <chrono>
using namespace std;

// Basic structure of passenger to store in data vector
struct passenger {
    int pClass;
    int survived;
    int sex;
    int age;
};


// Naive Bayes Class
class NaiveBayes{
private:
    // Training and testing data stored here
    vector<passenger> train;
    vector<passenger> test;
    // Total number of survived and perished
    int survived;
    int perished;
    // Apriori
    double survivedProbability[2];
    // Average age of survivor and perished
    double ageMean[2];
    // Variance of age by survivor and perished
    double ageVariance[2];
    // Likelihood of survival by class
    double pClass[2][4];
    // Likelihood of survival by sex
    double sex[2][2];
    // Age average of survival and standard deviation
    double age[2][2];
    
public:
    NaiveBayes(vector<passenger> _train, vector<passenger> _test){
        train = _train;
        test = _test;
    }

    // Calculates the total number of perished and survived
    void findSurvived(){
        int total = 0;
        for(int i = 0; i < train.size(); i++){
            if(train[i].survived == 1){
                total++;
            }
        }
        survived = total;
        perished = train.size() - survived;
        survivedProbability[0] = perished / (double)train.size();
        survivedProbability[1] = survived / (double)train.size();
    }

    // Finds the average age of survivor and perished
    void findAgeMean(){
        int agePerished = 0;
        int ageSurvived = 0;
        for(int i = 0; i < train.size(); i++){
            if(train[i].survived == 0){
                agePerished += train[i].age;
            } else {
                ageSurvived += train[i].age;
            }
        }
        ageMean[0] = agePerished / (double)perished;
        ageMean[1] = ageSurvived / (double)survived;
        age[0][0] = ageMean[0];
        age[1][0] = ageMean[1];
    }

    // Vinds variance and standard deviation of age
    void findAgeVar(){
        double survivedSum = 0;
        double perishedSum = 0;
        for(int i = 0; i < train.size(); i++){
            if(train[i].survived == 0){
                perishedSum += pow(train[i].age - ageMean[0], 2);
            } else {
                survivedSum += pow(train[i].age - ageMean[1], 2);
            }
        }
        ageVariance[0] = perishedSum / (perished - 1);
        ageVariance[1] = survivedSum / (survived - 1);
        age[0][1] = sqrt(ageVariance[0]);
        age[1][1] = sqrt(ageVariance[1]);
    }

    // Creates the likelihood tables 
    void findProbabilities(){
        int perishedFemale = 0;
        int perishedMale = 0;
        int oneClassPerished = 0;
        int twoClassPerished = 0;
        int threeClassPerished = 0;
    
        int survivedFemale = 0;
        int survivedMale = 0;
        int oneClassSurvived = 0;
        int twoClassSurvived = 0;
        int threeClassSurvived = 0;


        // Counts total amounts of each class and sex per person by whether or not they survived
        for(int i = 0; i < train.size(); i++){
            passenger currPassenger = train[i];
            if(currPassenger.survived == 0){
                switch(currPassenger.pClass){
                    case 1:
                        oneClassPerished++;
                        break;
                    case 2:
                        twoClassPerished++;
                        break;
                    case 3:
                        threeClassPerished++;
                        break;
                }
                if(currPassenger.sex == 0){
                    perishedFemale++;
                } else {
                    perishedMale++;
                }
            } else {
                switch(currPassenger.pClass){
                    case 1:
                        oneClassSurvived++;
                        break;
                    case 2:
                        twoClassSurvived++;
                        break;
                    case 3:
                        threeClassSurvived++;
                        break;
                }
                if(currPassenger.sex == 0){
                    survivedFemale++;
                } else {
                    survivedMale++;
                }
            }
        }

        pClass[0][1] = oneClassPerished / (double)perished;
        pClass[0][2] = twoClassPerished / (double)perished;
        pClass[0][3] = threeClassPerished / (double)perished;

        pClass[1][1] = oneClassSurvived / (double)survived;
        pClass[1][2] = twoClassSurvived / (double)survived;
        pClass[1][3] = threeClassSurvived / (double)survived;

        sex[0][0] = perishedFemale / (double)(perishedFemale + perishedMale);
        sex[0][1] = perishedMale / (double)(perishedFemale + perishedMale);

        sex[1][0] = survivedFemale / (double)(survivedFemale + survivedMale);
        sex[1][1] = survivedMale / (double)(survivedFemale + survivedMale);
    }

    // Driver code
    void trainData() {
        findSurvived();
        findAgeMean();
        findAgeVar();
        findProbabilities();
    }

    // Calculates likelood of age from age, average, and variance
    double calcAgeLikelihood(double age, double mean, double var){
        double pi = atan(1)*4;
        return 1 / sqrt(2 * pi * var) * exp(-1*(pow(mean,2)/(2*var)));
    }

    // Creates a probability array of survival and perish
    array<double, 2> calcRawProbability(passenger p){
        int passClass = p.pClass;
        int passSex = p.sex;
        int passAge = p.age;

        double probabilityPerished = pClass[0][passClass] * sex[0][passSex] * survivedProbability[0] * calcAgeLikelihood(passAge, age[0][0], ageVariance[0]);
        double probabilitySurvived = pClass[1][passClass] * sex[1][passSex] * survivedProbability[1] * calcAgeLikelihood(passAge, age[1][0], ageVariance[1]);
        double denominator = pClass[0][passClass] * sex[0][passSex] * survivedProbability[0] * calcAgeLikelihood(passAge, age[0][0], ageVariance[0]) + pClass[1][passClass] * sex[1][passSex] * survivedProbability[1] * calcAgeLikelihood(passAge, age[1][0], ageVariance[1]);
        return {probabilityPerished / denominator, probabilitySurvived / denominator};
    }

    void testData(){
        /*
        TP - Correctly predicted somebody survived
        FP - Incorrectly predicted survival
        FN - Incorrectly predicted perish
        TN - Correct predicted perish
        */
        int TP = 0;
        int FP = 0;
        int FN = 0;
        int TN = 0;

        // Create survival vector for each passenger
        for(int i = 0; i < test.size(); i++){
            array<double, 2> rawProbability = calcRawProbability(test[i]);
            // Predicts death
            if(rawProbability[0] > rawProbability[1]){
                // Correctly predicts death
                if(test[i].survived == 0){
                    TN++;
                } else {
                    FN++;
                }
            } else {
                if(test[i].survived == 1){
                    TP++;
                } else {
                    FP++;
                }
            }
        }
        // Calculate accuracy, sensitivity, and specificity
        double accuracy = (TP + TN) / (double)(TP + TN + FP + FN);
        double sensitivity = TP / (double)(TP + FN);
        double specificity = TN / (double)(TN + FP);

        cout << "Accuracy: " << accuracy * 100 << "%" << endl;
        cout << "Sensitivity: " << sensitivity * 100 << "%" << endl;
        cout << "Specificity: " << specificity * 100 << "%" << endl;
    }

};


// Basic data vector
vector<passenger> passengerData;

int readFile(string file_path){
    ifstream file(file_path);

    // Find if file exists
    if(!file.is_open()){
        cerr << "Unable to find file at " << file_path << "\n";
        return 1;
    }

    string line;
    int csvIndex = 0;
    while(getline(file, line)){
        // Ignore CSV Header
        if(csvIndex == 0){
            csvIndex++;
            continue;
        }

        // String stream to parse the current line
        stringstream ss(line);
        
        string currVal;
        passenger temp;
        
        // Ignore the first value which is "person id"
        getline(ss, currVal, ',');

        // Begin converting values to int and storing in global arrays
        getline(ss, currVal, ',');
        temp.pClass = stoi(currVal);
        getline(ss, currVal, ',');
        temp.survived = stoi(currVal);
        getline(ss, currVal, ',');
        temp.sex = stoi(currVal);
        getline(ss, currVal, ',');
        temp.age = int(round(stod(currVal)));
        passengerData.push_back(temp);
        csvIndex++;
    }

    // Close file and continue
    file.close();
    return 0;
}


int main(int argc, char *argv[]){
    auto startTime = chrono::high_resolution_clock::now();
    // File checking to ensure there is a file in command line
    if(argc < 2){
        cout << "No file found in command line parameters.\n"; 
        cout << "Please run the program by using `/.naive_bayes <file_name>` in command line." << "\n";
        return 1;
    }

    // Read the file
    readFile(argv[1]);

    // Training vector of the first 800 entries
    vector<passenger> trainingData(passengerData.begin(), passengerData.begin()+800);
    // Testing data for the final entries
    vector<passenger> testingData(passengerData.begin()+800, passengerData.end());
    NaiveBayes nb(trainingData, testingData);
    nb.trainData();
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> totalTime = endTime - startTime;
    cout << "Total Training Time: " << totalTime.count() << " seconds" << endl;
    nb.testData();
    return 0;
}