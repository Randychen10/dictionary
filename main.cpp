#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

// Struct definition for dictionary entries
struct Entry {
    string key;
    string nva;  // Part of speech
    string value; // Definition or value
};

// Function to trim leading and trailing whitespace
string trim(const string& str) {
    size_t front = str.find_first_not_of(" \t");
    size_t back = str.find_last_not_of(" \t");

    if (front == string::npos || back == string::npos) {
        return "";
    }

    return str.substr(front, back - front + 1);
}

// Function to get ordinal suffix for numbers (e.g., 1st, 2nd, 3rd, ...)
string getOrdinal(int number) {
    int m100 = number % 100;
    int m10 = number % 10;
    if (m10 == 1 && m100 != 11) {
        return to_string(number) + "st";
    } else if (m10 == 2 && m100 != 12) {
        return to_string(number) + "nd";
    } else if (m10 == 3 && m100 != 13) {
        return to_string(number) + "rd";
    } else {
        return to_string(number) + "th";
    }
}

// Function to print help message
void printHelpMessage() {
    cout << "     |" << endl;
    cout << "      PARAMETER HOW-TO, please enter:" << endl;
    cout << "      1. A search key -then 2. An optional part of speech -then" << endl;
    cout << "      3. An optional 'distinct' -then 4. An optional 'reverse'" << endl;
    cout << "     |" << endl;
}

// Function to get distinct entries based on value and part of speech
vector<Entry> distinctEntries(const vector<Entry>& entries) {
    unordered_set<string> seen;
    vector<Entry> distinctEntries;
    for (const auto& entry : entries) {
        string key = entry.value + "_" + entry.nva;
        if (seen.find(key) == seen.end()) {
            seen.insert(key);
            distinctEntries.push_back(entry);
        }
    }
    return distinctEntries;
}

// Function to reverse the order of entries
vector<Entry> reverseEntries(const vector<Entry>& entries) {
    vector<Entry> reversedEntries = entries;
    reverse(reversedEntries.begin(), reversedEntries.end()); // Use reverse
    return reversedEntries;
}


// Function to filter entries by part of speech
vector<Entry> filterEntriesByPartOfSpeech(const vector<Entry>& entries, const string& partOfSpeech) {
    vector<Entry> filteredEntries;
    for (const auto& entry : entries) {
        if (entry.nva == partOfSpeech) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}

// Function to apply commands (distinct, reverse) to entries
vector<Entry> applyCommands(vector<Entry> entries, const vector<string>& commands) {
    if (find(commands.begin(), commands.end(), "distinct") != commands.end()) {
        entries = distinctEntries(entries);
    }
    if (find(commands.begin(), commands.end(), "reverse") != commands.end()) {
        entries = reverseEntries(entries);
    }
    return entries;
}

unordered_set<string> getUniqueKeys(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    unordered_set<string> uniqueKeys;
    string line;

    while (getline(ioFile, line)) {
        uniqueKeys.insert(trim(line));
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
                    definitions.push_back(trim(item.substr(arrowPosItem + 4)));  // +4 to skip "-=>>"
                }
            }
        }
    }

    ioFile.close();
    return definitions;
}

// Consolidated help message function
void helpMessage() {
    cout << "     |" << endl;
    cout << "      PARAMETER HOW-TO, please enter:" << endl;
    cout << "      1. A search key -then 2. An optional part of speech -then" << endl;
    cout << "      3. An optional 'distinct' -then 4. An optional 'reverse'" << endl;
    cout << "     |" << endl;
}

// Function to search dictionary and handle commands
void dictSearch(const multimap<string, pair<string, string>>& dictionary) {
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
        } else {
            stringstream ss(userInput);
            string keyword, pos, option;
            ss >> keyword;

            vector<pair<string, string>> results;
            auto range = dictionary.equal_range(keyword);
            for (auto it = range.first; it != range.second; ++it) {
                results.push_back(it->second);
            }

            while (ss >> option) {
                if (option == "distinct") {
                    set<pair<string, string>> uniqueResults(results.begin(), results.end());
                    results.assign(uniqueResults.begin(), uniqueResults.end());
                } else if (option == "reverse") {
                    std::reverse(results.begin(), results.end());
                } else {
                    pos = option;
                    vector<pair<string, string>> filteredResults;
                    for (const auto& result : results) {
                        if (result.first == pos) {
                            filteredResults.push_back(result);
                        }
                    }
                    results = filteredResults;
                }
            }

            if (!results.empty()) {
                cout << "| " << endl;
                for (const auto& result : results) {
                    cout << keyword << " [" << result.first << "] : " << result.second << endl;
                }
                cout << "| " << endl;
            } else {
                cout << "| No results found for '" << keyword << "'" << endl;
                cout << "| " << endl;
            }
        }
        searchCount++;
    }
}

// Function to create dictionary from file
multimap<string, pair<string, string>> createDictionary(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    multimap<string, pair<string, string>> dictionary;
    string line;

    while (getline(ioFile, line)) {
        string keyword, rest;
        stringstream ss(line);
        getline(ss, keyword, '|');
        while (getline(ss, rest, '|')) {
            size_t arrowPos = rest.find("-=>>");
            if (arrowPos != string::npos) {
                string pos = trim(rest.substr(0, arrowPos));
                string def = trim(rest.substr(arrowPos + 4));  // +4 to skip "-=>>"
                dictionary.insert({keyword, {pos, def}});
            }
        }
    }

    ioFile.close();
    return dictionary;
}

// Function to print introductory message
void intro(const unordered_set<string>& uniqueKeys, const list<string>& definitions) {
    cout << "====== DICTIONARY 340 C++ ======" << endl;
    cout << "------ Keywords: " << uniqueKeys.size() << endl;
    cout << "------ Definitions: " << definitions.size() << endl;
}

// Main function
int main() {
    const string filename = "Data.CS.SFSU.txt";

    unordered_set<string> uniqueKeys = getUniqueKeys(filename);
    list<string> definitions = countDefinitions(filename);
    multimap<string, pair<string, string>> dictionary = createDictionary(filename);

    intro(uniqueKeys, definitions);
    dictSearch(dictionary);

    return 0;
}