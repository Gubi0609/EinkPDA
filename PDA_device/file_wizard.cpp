#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> displayDir(const string relativePath) {
    vector<string> filesInDir;
    int i = 0;

    for (const auto & entry : filesystem::directory_iterator(relativePath)) {
        cout << "(" << i << ") " << entry.path().filename() << endl;
        filesInDir.push_back(entry.path().filename().string());
        i++;
    }

    return filesInDir;
}

void displayFile(const string filePath) {
    ifstream file(filePath);

    if (file.is_open()) {
        
        string line;

        while (getline(file, line)) {
            cout << line << endl;
        }
        
        file.close();
    } else {
        cout << "Error loading file. Please try again." << endl;
    }
}


int main() {

    string path = "./";
    vector<string> filesInDir = displayDir(path);

    int fileNum;
    cout << "Please enter file/directory number: ";
    cin >> fileNum;

    if (fileNum >= filesInDir.size()) {
        cout << "Not in directory." << endl;
        return 1;
    }

    string fileName = filesInDir[fileNum];

    if (find(filesInDir.begin(), filesInDir.end(), fileName) != filesInDir.end()) {
        cout << "File in directory\n";

        if (fileName.find(".txt") != string::npos) {
            cout << "File is .txt" << endl;
            path += fileName;
            displayFile(path);
        } else { 
            cout << "File is not .txt" << endl;
            path += fileName;
            displayDir(path);
        }

    } else {
        cout << "File not in directory\n";
    }

    // ifstream file("./"+filename);

    /*
    if (file.is_open()) {
        

        string line;

        // while(getline(file, line)) {
        //     if (line.find("Name:") != string::npos) {
        //         name = line.substr(line.find(":") + 1);
        //     } else if (line.find("Health:") != string::npos) {
        //         health = stoi(line.substr(line.find(":") + 1));
        //     }else if (line.find("attackPower:") != string::npos) {
        //         attackPower = stoi(line.substr(line.find(":") + 1));
        //     } else if (line.find("XP:") != string::npos) {
        //         xp = stoi(line.substr(line.find(":") + 1));
        //     } else if (line.find("Level:") != string::npos) {
        //         level = stoi(line.substr(line.find(":") + 1));
        //     }
        // }


        file.close();
    } else {
        cout << "Error loading hero from file." << endl;
    }
    */

    return 0;
}