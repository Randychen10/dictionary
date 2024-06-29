#include <fstream>
#include <iostream>
#include<sstream>
#include <unordered_set>
#include <list>
#include <map>
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

list<string> countDefinitions(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    list<string> definitions;
    string line;

    while (getline(ioFile, line)) {

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

void helpMessage() {
    cout << "     |" << endl;
    cout << "      PARAMETER HOW-TO, please enter:" << endl;
    cout << "      1. A search key -then 2. An optional part of speech -then" << endl;
    cout << "      3. An optional 'distinct' -then 4. An optional 'reverse'" << endl;
    cout << "     |" << endl;
}

void dictSearch() {
    fstream ioFile;
    ioFile.open("Data.CS.SFSU.txt", ios::in);

    multimap<string, string> dictionary;

    int searchCount = 1;
    string userInput;

    cout << endl;
    while (true) {
        cout << "Search [" << searchCount << "]: ";
        getline(cin, userInput);

        if (userInput == "!q") {
            break;
        } else if (userInput == "!help" || userInput.empty()) {
            helpMessage();
        }
        searchCount++;
    }
}

int main() {
    unordered_set<string> uniqueKeys = getUniqueKeys("Data.CS.SFSU.txt");
    list<string> definitions = countDefinitions("Data.CS.SFSU.txt");

    intro(uniqueKeys, definitions);
    dictSearch();

    return 0;
}
