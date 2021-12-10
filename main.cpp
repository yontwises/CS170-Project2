#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>

using namespace std; 

//compute the distance of points from training set and then return the class label with the least distance away
int nearestNeighborClassifier(vector<vector<double>> data, int dont_compare, vector<int> features ) {
    int nearestNeighbor = 0; 
    double shortestDistance = std::numeric_limits<double>::max();
    double distance = 0.0;

    // cout << "Dont compare: " << dont_compare << endl;

    for (int i = 0; i < data.size();i++) {
        if(i == dont_compare) { //if we are comparing the same instance 
            continue;
        } 
        else {
            distance = 0; 
            // cout << data.at(i).at(j) << endl << data.at(dont_compare).at(j) << endl;

            for (int j = 0; j < features.size(); j++) {
                distance = distance + pow(data.at(i).at(features.at(j)) - data.at(dont_compare).at(features.at(j)),2);
                // cout << i << ":" << data.at(i).at(j) << endl << dont_compare << ":" << data.at(dont_compare).at(j) << endl;
                
            }
            distance = sqrt(distance);
            if (distance < shortestDistance) {
                nearestNeighbor = i;
                shortestDistance = distance;
            }
        }
    }
    return nearestNeighbor;
}

double leave_one_out_cross_validation(vector<vector<double>> data,vector<int> features) {
    double correct = 0.0; 
    int neighbor = 0;
    int accuracy = 0;
    for(int i = 0; i < data.size();i++) {
        neighbor = nearestNeighborClassifier(data,i,features);
        // cout << "Object " << i + 1 << " is class " << data.at(i).at(0) << endl;
        // cout << "Its nearest neighbor is " << neighbor + 1 << "which is in class " << data.at(neighbor).at(0) << endl;
        if(data.at(i).at(0) == data.at(neighbor).at(0)) {
            ++correct;
        }
    }
    accuracy = (correct / data.size()) * 100;
    return accuracy;
}

void forward_Selection(vector<vector<double>> data) {
    vector<int> current_set_of_features;   
    vector<int> final_set;
    double accuracy = 0.0; 

    for (int i = 0;i < data.at(0).size();i++) {
        //start with an empty set and then add features if it as the highest accuracy
        int feature_to_add_at_this_level; 
        int local_add;
        double best_so_far_accuracy = 0; 

        for(int k = 1; k < data.at(0).size();k++) {
            if(std::find(current_set_of_features.begin(),current_set_of_features.end(),k) == current_set_of_features.end()) {
                accuracy = leave_one_out_cross_validation(data,current_set_of_features);
            }
                cout << "Using feature(s) {";
                for(int x = 0; x < current_set_of_features.size(); ++x) {
                    cout << current_set_of_features.at(x) << ", ";

                }
                cout << " } accuracy is " << accuracy << endl;
                if(accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = k;
                }
        }
        current_set_of_features.push_back(feature_to_add_at_this_level); 
    }           
}

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

    vector<vector<double>> data ;
    vector<double> row; //data structure used to input row aka clas + feature1 + feature 2 + ...
    string stringRow; //used to get string of row  
    string item; //individual class or feature in the row 

    while(getline(fin,stringRow)) {
        stringstream sstream(stringRow);
        while(sstream >> item) {
            row.push_back(stod(item));
        }
        data.push_back(row);
        row.clear();
    }
    fin.close();
    int num_instances = data.size();
    int num_features = data.at(0).size();

    cout << "This dataset has " << num_features - 1 << " features (not including the class atribute), with " << num_instances << " instances " << endl << endl;

    vector<int> all_features; 
    for (int i = 1; i < data.at(0).size();i++) {
        all_features.push_back(i);
    }
    double accuracy = leave_one_out_cross_validation(data,all_features);
    cout << "Running nearest neighbor with all " << num_features - 1 << " features, using leaving-one-out evaluation, I get an accuracy of " << accuracy << "%" << endl; 

    cout << "Beginning search. " << endl << endl;

    forward_Selection(data);
    // for (int i  = 0; i < data.size();i++) {
    //     for(int j = 0; j < data.at(i).size();j++) {
    //         cout << data.at(i).at(j) << endl;
    //     }
    //     cout << endl;
    // }

    // for(int j = 0; j < data.at(0).size();j++) {
    //     cout << data.at(1).at(j) << endl;
    // }
    // cout << endl;


}