#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

// Basic structure of passenger to store in data vector
struct passenger {
    int pClass;
    int survived;
    int sex;
    int age;
};


class NaiveBayes{
private:
    vector<passenger> train;
    vector<passenger> test;

public:
    NaiveBayes(vector<passenger> _train, vector<passenger> _test){
        train = _train;
        test = _test;
    }

    void hello() {
        cout << "Hello!" << endl;
    }

};


// Basic data vector
vector<passenger> data;

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
        data.push_back(temp);
        csvIndex++;
    }

    // Close file and continue
    file.close();
    return 0;
}


int main(int argc, char *argv[]){
    // File checking to ensure there is a file in command line
    if(argc < 2){
        cout << "No file found in command line parameters.\n"; 
        cout << "Please run the program by using `/.naive_bayes <file_name>` in command line." << "\n";
        return 1;
    }

    // Read the file
    readFile(argv[1]);

    // Training vector of the first 800 entries
    vector<passenger> trainingData(data.begin(), data.begin()+800);
    vector<passenger> testingData(data.begin()+800, data.end());

    NaiveBayes nb(trainingData, testingData);
    nb.hello();

    return 0;
}