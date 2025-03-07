#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include "symbol_table.cpp"

using namespace std;

// Parser Class
class Parser {
private:
    vector<string> tokens;
    int currentIndex;
    SymbolTable &symbolTable;

public:
    Parser(vector<string> inputTokens, SymbolTable &table)
        : tokens(inputTokens), currentIndex(0), symbolTable(table) {}

    void parse() {
        while (currentIndex < tokens.size()) {
            string token = tokens[currentIndex];

            // Assignment: variable = value
            if (isVariable(token) && isNext("=")) {
                string varName = token;
                currentIndex += 2; // Skip variable and '='
                Value val = parseValue();
                symbolTable.setVariable(varName, val);
            }

            // Print statement
            else if (token == "print" && isNextVariable()) {
                currentIndex++;
                string varName = tokens[currentIndex];
                try {
                    Value val = symbolTable.getVariable(varName);
                    printValue(val);
                } catch (runtime_error &e) {
                    cout << "Error: " << e.what() << endl;
                }
            }

            currentIndex++;
        }
    }

private:
    bool isVariable(const string &token) {
        return isalpha(token[0]); // A variable must start with a letter
    }

    bool isNext(const string &expected) {
        return (currentIndex + 1 < tokens.size()) && (tokens[currentIndex + 1] == expected);
    }

    bool isNextVariable() {
        return (currentIndex + 1 < tokens.size()) && isVariable(tokens[currentIndex + 1]);
    }

    Value parseValue() {
        string token = tokens[currentIndex];

        if (isdigit(token[0])) { // Number
            if (token.find('.') != string::npos) return stof(token);
            return stoi(token);
        }
        if (token[0] == '"') return token.substr(1, token.size() - 2); // String (remove quotes)
        if (token == "true") return true;
        if (token == "false") return false;

        throw runtime_error("Unknown value type: " + token);
    }

    void printValue(const Value &val) {
        visit([](auto &&val) { cout << val << endl; }, val);
    }
};

// Example Usage
int main() {
    SymbolTable table;
    vector<string> tokens = {"x", "=", "10", "print", "x"}; // Simulated token stream

    Parser parser(tokens, table);
    parser.parse();

    return 0;
}
