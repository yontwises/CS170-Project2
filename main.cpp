#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <chrono> 

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
    auto start = std::chrono::high_resolution_clock::now();
    vector<int> current_set_of_features;   //initial empty set 
    vector<int> final_set;
    double top_Accuracy = 0.0; //best accuracy for all levels in the tree
    double accuracy = 0.0; 

    for (int i = 0;i < data.at(0).size();i++) {
        //start with an empty set and then add features if it as the highest accuracy
        int feature_to_add_at_this_level = -1; //if this is changed then it means that we have found the feature with the highest accuracy in the level
        int local_add = -1; //feature to add at a local level
        double best_so_far_accuracy = 0; //best accuracy for a certain level in the tree

        for(int k = 1; k < data.at(0).size();k++) {
            if(std::find(current_set_of_features.begin(),current_set_of_features.end(),k) == current_set_of_features.end()) {
                vector<int> temp_current_set_of_features = current_set_of_features; //create a copy of feature subset and input new feature in it
                temp_current_set_of_features.push_back(k);
                accuracy = leave_one_out_cross_validation(data,temp_current_set_of_features);//get accuracy with new feature inserted

                cout << "Using feature(s) {";
                for(int x = 0; x < temp_current_set_of_features.size(); ++x) {
                    cout << temp_current_set_of_features.at(x) << ", ";
                }
                cout << " } accuracy is " << accuracy << endl;

                if(accuracy > top_Accuracy) { 
                    top_Accuracy = accuracy;
                    feature_to_add_at_this_level = k;
                }
                if(accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    local_add = k;
                }
                    
            }
        }

        if (feature_to_add_at_this_level >= 0) {
            current_set_of_features.push_back(feature_to_add_at_this_level);
            final_set.push_back(feature_to_add_at_this_level);
            cout << "\n Feature set {";
            for(int x = 0; x < current_set_of_features.size(); ++x) {
                    cout << current_set_of_features.at(x) << ", ";
            }
            cout << "} was best, accuracy is " << top_Accuracy << endl;
        }
        else {
            cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
            current_set_of_features.push_back(local_add);
            cout << "Feature set {";
            for(int x = 0; x < current_set_of_features.size(); ++x) {
                    cout << current_set_of_features.at(x) << ", ";
            }
            cout << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl << endl;
        }
    } 
    cout << "Finished search!! The best feature subset is {";
    for(int x = 0; x < final_set.size(); ++x) {
        cout << final_set.at(x) << ", ";
    }          
    cout << "} which has an accuracy of " << top_Accuracy << "%" << endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    cout << duration.count() << endl; 
}

//returns index of node to be deleted
int deleteFeature(vector<int> vec, int feature) {
    int indexToDelete;
    for(int z = 0; z <vec.size();z++) {
        if(vec.at(z) == feature) {
            indexToDelete = z;
        }
    }
    return indexToDelete;
}

//similar to forward selection but you start with a set full of features and then eliminate 
void backwardElimnation(vector<vector<double>> data, double all_features_Accuracy) {
    //populate a vector full of features 
    auto start = std::chrono::high_resolution_clock::now();
    
    double top_Accuracy = all_features_Accuracy;
    double accuracy = 0.0;
    vector<int> current_set_of_features;
    for (int i = 1; i < data.at(0).size();i++) {
        current_set_of_features.push_back(i);
    }
    vector<int> final_set = current_set_of_features;

    for (int i = 0; i < data.at(0).size();i++) {
        int feature_to_remove_at_this_level = -1; 
        int local_delete = -1; //feature to add at a local level
        double best_so_far_accuracy = 0.0; //best accuracy for a certain level in the tree
        for(int k = 1; k < data.at(0).size();k++) {
            if(current_set_of_features.empty()) {
                break;
            }
            if(std::find(current_set_of_features.begin(),current_set_of_features.end(),k) != current_set_of_features.end()) {
                vector<int> temp_current_set_of_features = current_set_of_features; //create a copy of feature subset and delete  feature k in it
                temp_current_set_of_features.erase(temp_current_set_of_features.begin() + deleteFeature(temp_current_set_of_features,k));
                accuracy = leave_one_out_cross_validation(data,temp_current_set_of_features);//get accuracy with new feature inserted

                cout << "Using feature(s) {";
                for(int x = 0; x < temp_current_set_of_features.size(); ++x) {
                    cout << temp_current_set_of_features.at(x) << ", ";
                }
                cout << " } accuracy is " << accuracy << endl;

                if(accuracy > top_Accuracy) { 
                    top_Accuracy = accuracy;
                    feature_to_remove_at_this_level = k;
                }
                if(accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    local_delete = k;
                }
            }
        }
        if (feature_to_remove_at_this_level >= 0) {
            current_set_of_features.erase(current_set_of_features.begin() + deleteFeature(current_set_of_features,feature_to_remove_at_this_level) );

            final_set.erase(final_set.begin() + deleteFeature(final_set,feature_to_remove_at_this_level));
            cout << "\n Feature set {";
            for(int x = 0; x < current_set_of_features.size(); ++x) {
                    cout << current_set_of_features.at(x) << ", ";
            }
            cout << "} was best, accuracy is " << top_Accuracy << endl;
        }
        else {
            cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
            if(current_set_of_features.empty()) {
                break;
            }
            current_set_of_features.erase(current_set_of_features.begin() + deleteFeature(current_set_of_features,local_delete));
            cout << "Feature set {";
            for(int x = 0; x < current_set_of_features.size(); ++x) {
                    cout << current_set_of_features.at(x) << ", ";
            }
            cout << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl << endl;
        }
    } 
    cout << "Finished search!! The best feature subset is {";
    for(int x = 0; x < final_set.size(); ++x) {
        cout << final_set.at(x) << ", ";
    }          
    cout << "} which has an accuracy of " << top_Accuracy << "%" << endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    cout << duration.count() << endl; 
}
int main() {
    string fileName; 
    string algorithm;
    ifstream fin; 

    cout << "Welcome to Kenny Yontwises's Feature Selection Algorithm." << endl; 
    cout << "Type in the name of the file to test: " << endl << endl;  
    cin >> fileName;

    cout << "Type the number of the algorithm you want to run. " << endl; 
    cout << "1) Forward Selection" << endl;
    cout << "2) Backward Elimination" << endl << endl; 

    cin >> algorithm;

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

    if(algorithm == "1") {
        forward_Selection(data);
    }
    else if(algorithm == "2") {
        backwardElimnation(data,accuracy);
    }
    else {
        cout << "Invalid input" << endl;
    }
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