#include <string>
#include <vector>

using namespace std;

class FileWizard {
    public:
        FileWizard();
        void displayDir(const string relativePath);
        void displayDirNoFiles(const string relativePath);
        string navigateInDir();
        string navigateInDirNoFiles();
        string fileOption(const string relativePath);
        
        bool isAlphaNumeric(const string str);
        string legalName();

        string newFile(const string relativePath);
        string copyFile(const string path);
        string deleteFile(const string path);
        string renameFile(const string path);
        string moveFile(const string path);
        
        ~FileWizard();

    private:
        string path = "./";
        vector<string> filesInDir;
};