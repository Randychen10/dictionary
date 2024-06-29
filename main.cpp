#include <fstream>
#include <iostream>
#include<sstream>
#include <unordered_set>
#include <list>
#include <string>

using namespace std;

void intro(const unordered_set<string>& uniqueKeys, const list<string>& definitions) {
    cout << "====== DICTIONARY 340 C++ ======" << endl;
    cout << "------ Keywords: " << uniqueKeys.size() << endl;
    cout << "------ Definitions: " << definitions.size() << endl;
}

unordered_set<string> getUniqueKeys(const string&) {
    fstream ioFile;
    ioFile.open("Data.CS.SFSU.txt", ios::in);

    unordered_set<string> uniqueKeys;
    string line;

    while (getline(ioFile, line)) {
        uniqueKeys.insert(line);
    }

    ioFile.close();
    return uniqueKeys;
}

// Function to trim leading and trailing whitespace
string trim(const string& str) {
    size_t front = str.find_first_not_of(" \t");
    size_t back = str.find_last_not_of(" \t");

    if (front == string::npos || back == string::npos) {
        return "";
    }

    return str.substr(front, back - front + 1);
}

list<string> countDefinitions(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    list<string> definitions;
    string line;

    while (getline(ioFile, line)) {
        // Trim leading and trailing whitespace
        line = trim(line);

        // Check if the line contains the arrow "-=>>"
        size_t arrowPos = line.find("-=>>");

        if (arrowPos != string::npos) {
            // Split the line into definitions by "|"
            stringstream ss(line);
            string item;
            while (getline(ss, item, '|')) {
                // Check if the item contains the arrow "-=>>"
                size_t arrowPosItem = item.find("-=>>");
                if (arrowPosItem != string::npos) {
                    // Add the item to definitions list
                    definitions.push_back(item.substr(arrowPosItem + 4));  // +4 to skip "-=>>"
                }
            }
        }
    }

    ioFile.close();
    return definitions;
}

void helpMessage(){

void dictSearch() {
    cout << endl;
    fstream ioFile;
    ioFile.open("Data.CS.SFSU.txt", ios::in);

    int searchCount = 1;
    string userInput;
    cout << endl;
    cout << "Search [" << searchCount << "]: ";
    while (getline(cin, userInput) && userInput != "!q") {
        searchCount++;
        cout << "Search [" << searchCount << "]: ";
    }
}

int main() {
    unordered_set<string> uniqueKeys = getUniqueKeys("Data.CS.SFSU.txt");
    list<string> definitions = countDefinitions("Data.CS.SFSU.txt");

    intro(uniqueKeys, definitions);
    dictSearch();

    return 0;
}
