#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std; 



int main() {
    string fileName; 
    ifstream fin; 

    cout << "Welcome to Kenny Yontwises's Feature Selection Algorithm." << endl; 
    cout << "Type in the name of the file to test: " << endl; 

    cin >> fileName;

    fin.open(fileName);

    //Read from file validation
    if(!fin.is_open()) {
        cout << "File did not open" << endl; 
        return -1;
    }

    vector<vector<double>> testSet ;
    vector<double> row; //data structure used to input row aka clas + feature1 + feature 2 + ...
    string stringRow; //used to get string of row  
    string item; //individual class or feature in the row 

    while(getline(fin,stringRow)) {
        stringstream sstream(stringRow);
        while(sstream >> item) {
            row.push_back(stod(item));
        }
        testSet.push_back(row);
    }
    int num_instances = testSet.size();
    int num_features = testSet.at(0).size();

    cout << "number of instances: " << num_instances << endl; 
    cout << "number of features: " << num_features << endl; 
    // for (int i  = 0; i < testSet.size();i++) {
    //     for(int j = 0; j < testSet.at(i).size();j++) {
    //         cout << testSet.at(i).at(j) << endl;
    //     }
    //     cout << endl;
    // }


}