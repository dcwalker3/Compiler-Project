//
// Created by Dakota on 9/12/2022.
//

#include "../Header Files/Compiler.h"

#include <fstream>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;



        /// Constructor
        /// \param filePath -> The path to the file to be compiled. Will have a .dak extension
        Compiler::Compiler(std::string filePath) {
            this->filePath = filePath;
            this->fileName = filePath.substr(filePath.find_last_of("/") + 1);
            this->fileLines = readFile();
            this->fileLines = removeComments(this->fileLines);
            run();
        }

        /// Split the file into lines to be read and executed
        /// \param filePath -> The path to the file
        /// \return A vector of strings, each string is a line of code
        vector <string> Compiler::readFile() {
            vector <string> fileLines;
            ifstream file(filePath);
            string line;
            while (getline(file, line)) {
                fileLines.push_back(line);
            }
            return fileLines;
        }

        /// Remove all comment lines from the file for easier reading
        /// \param fileLines -> The vector of strings that contains the lines of code
        /// \return A new vector of strings that does not contain any comment lines
        vector <string> Compiler::removeComments(vector <string> fileLines) {
            vector <string> newFileLines;
            for (int i = 0; i < fileLines.size(); i++) {
                if (fileLines[i].find("//") != string::npos) {
                    fileLines[i] = fileLines[i].substr(0, fileLines[i].find("//"));
                }
                if (fileLines[i] != "") {
                    newFileLines.push_back(fileLines[i]);
                }
            }
            return newFileLines;
        }

        /// Take variables and their values and store them in a map
        /// \param variable -> The name of the variable
        /// \param value -> The value of the variable
        void Compiler::addVariable(string variable, string value) {
            this->variables[variable] = value;
        }

        /// Check if a variable exists in the map
        /// \param variable -> The name of the variable
        /// \return True if the variable exists, false if it does not
        bool Compiler::variableExists(string variable) {
            if (this->variables.find(variable) != this->variables.end()) {
                return true;
            }
            return false;
        }

        void Compiler::printVariables(){
            for (auto const& x : this->variables)
            {
                std::cout << x.first << ": " << x.second << std::endl;
            }
        }

        /// Get the datatype of a variable
        /// \param variableName -> The name of the variable
        /// \return The datatype of the variable
        string Compiler::getVariableType(string variableName) {
            // Try and find the most appropriate variable type
            // If the variable is a string, return "string"
            // If the variable is an integer, return "int"
            // If the variable is a float, return "float"
            // If the variable is a boolean, return "bool"

            string variable = variables[variableName];
            if (variable[0] == '"' && variable[variable.length() - 1] == '"') {
                return "string";
            } else if (variable == "true" || variable == "false") {
                return "bool";
            } else if (variable.find(".") != string::npos) {
                return "float";
            }
            else if(variable.find_first_not_of("0123456789") == string::npos) {
                return "int";
            }
            else {
                return "ERROR: Invalid Type for Variable " + variableName;
            }
        }

        /// Replace a string containing a variable with the value of the variable
        /// Variables are denoted by ${variableName}
        /// \param line -> The line of code to be replaced
        /// \return The line of code with the variable replaced with its value
        string Compiler::replaceVariable(string line) {
            // If the line contains a variable, replace it with the value of the variable
            // If the variable does not exist, return an error
            if (line.find("${") != string::npos) {
                string variableName = line.substr(line.find("${") + 2, line.find("}") - line.find("${") - 2);
                if (variableExists(variableName)) {
                    line = line.replace(line.find("${" + variableName + "}"), variableName.length() + 3, variables[variableName]);
                } else {
                    return "ERROR: Variable " + variableName + " does not exist";
                }
            }
            return line;
        }

        /// Run Function
        /// \param codeLine -> The line of code to be executed
        /// \return Output of the line of code
        string Compiler::runFunction(string codeLine){
            // Check what codeLine contains
            // If it contains a variable return the value of the variable
            // If it contains a function, run the function
            if(codeLine.find("type(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("type(") + 5, codeLine.find(")") - codeLine.find("type(") - 5);
                return getVariableType(variableName);
            }
            else if(codeLine.find("print(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("print(") + 6, codeLine.find(")") - codeLine.find("print(") - 6);
                return variables[variableName];
            }
            else if(codeLine.find("int(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("int(") + 4, codeLine.find(")") - codeLine.find("int(") - 4);
                return to_string(stoi(variables[variableName]));
            }
            else if(codeLine.find("float(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("float(") + 6, codeLine.find(")") - codeLine.find("float(") - 6);
                return to_string(stof(variables[variableName]));
            }
            else if(codeLine.find("string(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("string(") + 7, codeLine.find(")") - codeLine.find("string(") - 7);
                return variables[variableName];
            }
            else if(codeLine.find("bool(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("bool(") + 5, codeLine.find(")") - codeLine.find("bool(") - 5);
                return variables[variableName];
            }
            else if(codeLine.find("sqrt(") != string::npos){
                string variableName = codeLine.substr(codeLine.find("sqrt(") + 5, codeLine.find(")") - codeLine.find("sqrt(") - 5);
                return to_string(sqrt(stof(variables[variableName])));
            }
        }

        /// Print Statement Function
        /// \param output -> The output to be printed
        /// \param newLine -> Whether or not to print a new line after the output. Defaults to true
        void Compiler::print(string output, bool newLine = true) {
            // Only get what is inside print()
            output = output.substr(output.find("(") + 1, output.find_last_of(")") - output.find("(") - 1);

            // If output contains a function, run the function
            if (output.find("(") != string::npos) {
                output = runFunction(output);
            }

            // Replace variables in the output first.
            output = replaceVariable(output);

            // Remove quotes from the output
            if (output[0] == '"' && output[output.length() - 1] == '"') {
                output = output.substr(1, output.length() - 2);
            }

            if (newLine) {
                cout << output << endl;
            } else {
                cout << output;
            }
        }

        /// Input Statement Function
        /// \param printStatement -> The statement to be printed before the input
        /// \returns The input from the user as a string
        string Compiler::input(string printStatement) {
            printStatement = replaceVariable(printStatement);
            print(printStatement, false);
            string input;
            getline(cin, input);
            return input;
        }

        /// Look for variable assignments in the line of code.
        /// Will look like 'variableName = value'
        /// \param line -> The line of code to be checked
        /// \return Nothing. Will add the variable to the map if it is a valid assignment
        void Compiler::checkVariableAssignment(string line) {
            // Check if the line is a variable assignment
            // If it is, add the variable to the map
            if (line.find("=") != string::npos) {

                string variableName = line.substr(0, line.find("="));
                string variableValue = line.substr(line.find("=") + 1);

                // Strip leading and trailing whitespace
                variableName.erase(remove(variableName.begin(), variableName.end(), ' '), variableName.end());

                if (variableName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") == string::npos) {
                    addVariable(variableName, variableValue);
                } else {
                    print("ERROR: Invalid Variable Name " + variableName);
                }
            }
        }

        /// type() function
        /// \param line -> The line of code to be checked
        /// \return The datatype of the variable
        string Compiler::typeOf(string line) {
            // Get the variable name
            string variableName = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);
            // Remove whitespace
            variableName.erase(remove(variableName.begin(), variableName.end(), ' '), variableName.end());
            // Check if the variable exists
            if (variableExists(variableName)) {
                return getVariableType(variableName);
            } else {
                return "ERROR: Variable " + variableName + " does not exist";
            }
        }

        /// Evaluate a Conditional Statement
        /// \param condition -> The condition to be evaluated
        /// \return True\False
        bool Compiler::checkCondition(string condition){

            if(condition == "true"){
                return true;
            } else if(condition == "false"){
                return false;
            }


        }

        /// Function to Determine line contents and run the appropriate function
        /// \param line -> The line of code to be checked
        /// \return None
        void Compiler::executeLine(string line) {

            // Check if the line is a print statement
            if (line.find("print(") != string::npos) {
                print(line);
                return;
            }

            if(line.find("if(") != string::npos){
                // Get index of the line
                int index = 0;
                for(int i = 0; i < fileLines.size(); i++){
                    if(fileLines[i] == line){
                        index = i;
                    }
                }
                conditionalStatement(index);
                return;
            }

            // Check if the line is an input statement
            if (line.find("input(") != string::npos) {
                string input = this->input(line);
                string variableName = line.substr(0, line.find("="));
                variableName.erase(remove(variableName.begin(), variableName.end(), ' '), variableName.end());
                addVariable(variableName, input);
                return;
            }

            // Check if the line is a type() statement
            if (line.find("type(") != string::npos) {
                // Replace type() with the variable type
                line = line.replace(line.find("type("), line.find_last_of(")") - line.find("type("), typeOf(line));
                // Strip the last of the parentheses and ;
                line = line.substr(0, line.find_last_of(")"));
            }

            // Check if the line is a variable assignment
            checkVariableAssignment(line);

        }

        /// Conditional Statement Function
        /// Conditional Statments are formatted like this:
        /// if (condition) {
        ///     code
        /// }
        /// \param lineNumber -> The line number of the conditional statement
        /// \return None
        void Compiler::conditionalStatement(int lineNumber) {
            vector<string> conditionalLines;
            string conditionalLine = fileLines[lineNumber];

            // Find the line that contains the ending }
            int endLine = lineNumber;
            while (fileLines[endLine].find("}") == string::npos) {
                endLine++;
            }

            // Get the condition
            string condition = conditionalLine.substr(conditionalLine.find("(") + 1, conditionalLine.find_last_of(")") - conditionalLine.find("(") - 1);
        }

        /// Run the code
        void Compiler::run() {
            for (int i = 0; i < fileLines.size(); i++) {
                string line = fileLines[i];
                executeLine(line);
            }
        }

