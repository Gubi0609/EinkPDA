#include "FileWizard.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

using namespace std;

FileWizard::FileWizard() {
    cout << "FileWizard initialized." << endl;
}

void FileWizard::displayDir(const string relativePath) {
    int i = 0;
    filesInDir.clear();

    if (path > "./") {
        cout << "(0) Move one directory back." << endl;
        filesInDir.push_back("moveBack");
        i++;
    }

    for (const auto & entry : filesystem::directory_iterator(relativePath)) {
        cout << "(" << i << ") " << entry.path().filename() << endl;
        filesInDir.push_back(entry.path().filename().string());
        i++;
    }
}

void FileWizard::displayDirNoFiles(const string relativePath) {
    int i = 0;
    filesInDir.clear();

    if (path > "./") {
        cout << "(0) Move one directory back." << endl;
        filesInDir.push_back("moveBack");
        i++;
    }

    for (const auto & entry : filesystem::directory_iterator(relativePath)) {
        string item = entry.path().filename();
        if (item.find(".txt") == string::npos) {
            cout << "(" << i << ") " << item << endl;
            filesInDir.push_back(entry.path().filename().string());
            i++;
        }
    }
}

string FileWizard::navigateInDir() {
    
    while (true) {

        if (path.find(".txt")) {
            size_t lastSlash = path.find_last_of('/');
            if (lastSlash != string::npos) {
                path = path.substr(0, lastSlash + 1); // Keep the slash at the end
            }
        }

        displayDir(path);

        string input;
        cout << "Please enter file/directory index or (N) for new file: ";
        cin >> input;

        if (input == "N" || input == "n") {
            
            return newFile(path);

        } else if (all_of(input.begin(), input.end(), ::isdigit)) {

            int fileNum = stoi(input);
        
            if (fileNum >= filesInDir.size()) {
                cout << "Index not in directory." << endl;
            }

            string fileName = filesInDir[fileNum];
    
            if (fileName == "moveBack") {
                if (!path.empty() && path.back() == '/') {
                    path.pop_back();
                }

                size_t lastSlash = path.find_last_of('/');
                if (lastSlash != string::npos) {
                    path = path.substr(0, lastSlash + 1); // Keep the slash at the end
                }
            } else if(fileName.find(".txt") != string::npos) {
                path += fileName;
                path = fileOption(path);
                break;
            } else if (fileName.find(".") == string::npos) { 
                cout << "moving into " << fileName << endl;
                path += fileName + "/";
            }
        }
    }

    return path;

}

string FileWizard::navigateInDirNoFiles() {
    
    while (true) {

        if (path.find(".txt")) {
            size_t lastSlash = path.find_last_of('/');
            if (lastSlash != string::npos) {
                path = path.substr(0, lastSlash + 1); // Keep the slash at the end
            }
        }

        displayDir(path);

        int fileNum;
        cout << "Please enter directory index: ";
        cin >> fileNum;

        
        
        if (fileNum >= filesInDir.size()) {
            cout << "Index not in directory." << endl;
        }

        string fileName = filesInDir[fileNum];

        if (fileName == "moveBack") {
            if (!path.empty() && path.back() == '/') {
                path.pop_back();
            }

            size_t lastSlash = path.find_last_of('/');
            if (lastSlash != string::npos) {
                path = path.substr(0, lastSlash + 1); // Keep the slash at the end
            }
        } else if (fileName.find(".") == string::npos) { 
            cout << "moving into " << fileName << endl;
            path += fileName + "/";
        }
    }

    return path;
}

string FileWizard::fileOption(const string relativePath) {
    
    int input;
    
    cout << "(0) Edit file" << endl;
    cout << "(1) Rename file" << endl;
    cout << "(2) Move file" << endl;
    cout << "(3) Copy file" << endl;
    cout << "(4) Delete file" << endl;
    cout << "(5) Cancel" << endl;
    cout << "Please choose what to do with file: " << endl;
    cin >> input;

    string choice;

    switch (input) {
        case 0:
            return relativePath;
        case 1:
            return renameFile(relativePath);
        case 2:
            return moveFile(relativePath);
        case 3:
            return copyFile(relativePath);
        case 4:
            cout << "Are you sure, you want to delete this file? (y/n): ";
            cin >> choice;
            if (choice == "y" || choice == "Y") {
                cout << "Deleting file..." << endl;
                return deleteFile(relativePath);
            } else {
                break;
            }
        case 5:
            if (path.find(".txt")) {
                size_t lastSlash = path.find_last_of('/');
                if (lastSlash != string::npos) {
                    path = path.substr(0, lastSlash + 1); // Keep the slash at the end
                }
            }
        return path;
    }
}

bool FileWizard::isAlphaNumeric(const string str) {
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isalnum(c); // Check if each character is alphanumeric
    });
}

string FileWizard::legalName() {
    string fileName;
    while (true) {
        cout << "Rules for name of file:" << endl;
        cout << "No special characters (only alphabetic and numeric)" << endl;
        cout << "Max 16 chars long" << endl; 
        cout << "No funny business or file extensions" << endl;
        cout << "Please enter name of file: ";
        cin >> fileName;

        if (fileName.size() > 16) {
            cout << "Name of file too long. Please name it something else." << endl;
        } else if (fileName.find(".") != string::npos) {
            cout << "Please don't use '.' or try to add a file extension." << endl;
        } else if (!isAlphaNumeric(fileName)) {
            cout << "No special characters allowed. Please name it something else." << endl;
        } else {
            break;
        }
    }

    return fileName;
}

string FileWizard::newFile(const string relativePath) {

    string fileName = legalName();

    ofstream newFile(relativePath+fileName+".txt");
    newFile.close();

    path += fileName+".txt";

    return path;

}

string FileWizard::copyFile(const string path) {

    string newPath = path;
    string fileName = legalName();

    if (newPath.find(".txt")) {
        size_t lastSlash = newPath.find_last_of('/');
        if (lastSlash != string::npos) {
            newPath = newPath.substr(0, lastSlash + 1); // Keep the slash at the end
        }
    }

    string line; 
    ifstream orgFile{path}; // This is the original file 
    ofstream outFile{newPath+fileName+".txt"}; 
    if (orgFile && outFile) { 
  
        while (getline(orgFile, line)) { 
            outFile << line << "\n"; 
        } 
        cout << "Copy Finished \n"; 
    } 
    else { 
        // Something went wrong 
        printf("Cannot read File"); 
    } 
    // Closing file 
    orgFile.close(); 
    outFile.close(); 
}

string FileWizard::deleteFile(const string path) {
    int status = remove(path.c_str());

    // Check if the file has been successfully removed
    if (status != 0) {
        perror("Error deleting file");
    }
    else {
        cout << "File successfully deleted" << endl;
    }

    return path;
}

string FileWizard::renameFile(const string path) {
    const char* oldName = path.c_str();
	const char* newName;

    string newPath = path;

    if (newPath.find(".txt")) {
        size_t lastSlash = newPath.find_last_of('/');
        if (lastSlash != string::npos) {
            newPath = newPath.substr(0, lastSlash + 1); // Keep the slash at the end
        }
    }

    string fileName = legalName();
    string newPath = path + fileName;

    newName = newPath.c_str();

    int status = rename(oldName, newName);
	
	if (status != 0)
		perror("Error renaming file");
	else
		cout << "File renamed successfully" << endl;
}

string FileWizard::moveFile(const string path) {
    const char* source = path.c_str();
	const char* destination;

    // Choose new destination:
    string newPath = navigateInDirNoFiles();

    destination = newPath.c_str();

    int status = rename(source, destination);
	
	if (status != 0)
		perror("Error moving file");
	else
		cout << "File moved successfully" << endl;
}

FileWizard::~FileWizard() {}