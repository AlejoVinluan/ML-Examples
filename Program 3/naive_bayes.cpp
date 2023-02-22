#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Length of the titanic project csv
const int titanicProjectLength = 1046;

// Global variables to store values
double pClass[titanicProjectLength];
double survived[titanicProjectLength];
double sex[titanicProjectLength];
double age[titanicProjectLength];

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
            cout << line << endl;
            csvIndex++;
            continue;
        }

        // String stream to parse the current line
        stringstream ss(line);
        
        string currVal;
        
        // Ignore the first value which is "person id"
        getline(ss, currVal, ',');

        // Beging converting values to doubles and storing in global arrays
        getline(ss, currVal, ',');
        pClass[csvIndex-1] = stod(currVal);
        getline(ss, currVal, ',');
        survived[csvIndex-1] = stod(currVal);
        getline(ss, currVal, ',');
        sex[csvIndex-1] = stod(currVal);
        getline(ss, currVal, ',');
        age[csvIndex-1] = stod(currVal);

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
    return 0;
}