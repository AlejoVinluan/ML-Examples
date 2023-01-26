#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<double> rm(1000);
vector<double> medv(1000);

void readAndProcessCSV(){
    ifstream inFS;
    string line;
    string rm_in, medv_in;

    inFS.open("Boston.csv");
    if(!inFS.is_open()){
        cout << "Could not find or open the file Boston.csv";
        exit(1);
    }


}

int main(int argc, char** argv) {
    readAndProcessCSV();
    return 0;
}
