//
// Created by dakot on 9/12/2022.
//

#ifndef COMPILER_PROJECT_COMPILER_H
#define COMPILER_PROJECT_COMPILER_H

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Compiler{
    public:
        map <string, string> variables;
        vector <string> fileLines;
        string filePath;
        string fileName;

        // Class Functions
        Compiler(string filePath);
        vector <string> readFile();
        vector <string> removeComments(vector <string> fileLines);
        void addVariable(string variable, string value);
        bool variableExists(string variable);
        string getVariableType(string variableName);
        string replaceVariable(string line);
        void print(string output, bool newLine);
        string input(string printStatement);
        void checkVariableAssignment(string line);
        void run();
        void printVariables();
        string typeOf(string line);
        void executeLine(string line);
        string runFunction(string codeLine);
        void conditionalStatement(int lineNum);
        bool checkCondition(string condition);
        string removeLeadingZeros(string number);
        bool eval(string expression, string operatorType);
};


#endif //COMPILER_PROJECT_COMPILER_H
