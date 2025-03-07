#include <iostream>
#include <unordered_map>
#include <variant>

using namespace std;

// Variant to hold multiple data types (int, float, string, bool)
using Value = variant<int, float, string, bool>;

class SymbolTable {
private:
    unordered_map<string, Value> variables;

public:
    void setVariable(string name, Value value) {
        variables[name] = value;
    }

    Value getVariable(string name) {
        if (variables.find(name) != variables.end()) {
            return variables[name];
        } else {
            throw runtime_error("Variable '" + name + "' is not defined");
        }
    }

    void printVariables() {
        for (auto &pair : variables) {
            cout << pair.first << " = ";
            visit([](auto &&val) { cout << val; }, pair.second);
            cout << endl;
        }
    }
};
