//
// Created by dakot on 9/7/2022.
//

#include "../Header Files/FileFunctions.h"

#include <fstream>
#include <map>
#include <vector>

using namespace std;

vector<string> readFile(string filePath) {
    vector<string> fileLines;
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        fileLines.push_back(line);
    }
    return fileLines;
}


