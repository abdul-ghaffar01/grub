#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "error.cpp"
#include "list.cpp"
#include "utils.cpp"

using namespace std;

// Function to check if a string is a number
bool isNumber(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c) && c != '.' && c != '-')
            return false;
    }
    return true;
}

// Function to infer C++ data types from GRUB values
string inferType(const string &value)
{
    if (value == "true" || value == "false")
        return "bool";
    if (value.find('.') != string::npos)
        return "double";
    if (isdigit(value[0]) || value[0] == '-')
        return "int";
    return "string";
}

// Function to check if a token is an operator
bool isOperator(const string &token)
{
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

// Function to check if a string is a string literal (enclosed in quotes)
bool isStringLiteral(const string &token)
{
    return token.length() >= 2 && token.front() == '"' && token.back() == '"';
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc == 1)
            throw Error("No source file provided");

        string filename = argv[1];
        if (filename.substr(filename.find_last_of(".") + 1) != "gr")
            throw Error("Invalid file type");

        ifstream file(filename);
        if (!file)
            throw Error("File not found: " + filename);

        ofstream cppFile("output.cpp");
        if (!cppFile)
            throw Error("Failed to create output.cpp");

        // Writing C++ boilerplate
        cppFile << "#include <iostream>\nusing namespace std;\nint main() {\n";

        string line;
        while (getline(file, line))
        {
            List<string> tokens = split(line);
            if (tokens.getSize() == 0)
                continue; // Ignore empty lines

            if (tokens[0] == "print")
            {
                cppFile << "    cout << ";

                for (int i = 1; i < tokens.getSize(); i++)
                {
                    if (i > 1) // Add << only **before** the next token
                        cppFile << " << \" \" << ";

                    if (tokens[i][0] == '"') // Handling multi-word strings
                    {
                        cppFile << tokens[i] << " ";
                        while (tokens[i][tokens[i].length() - 1] != '"')
                        {
                            i++;
                            cppFile << tokens[i] << " ";
                        }
                    }
                    else
                    {
                        cppFile << tokens[i] << " ";
                    }
                }

                cppFile << "<< \"\\n\";\n"; // Ensure newline at the end
            }

            else if (tokens.getSize() >= 3 && tokens[1] == "=")
            {
                // Variable assignment with expressions
                string varName = tokens[0];
                string expression = "";
                bool containsOperators = false;

                for (int i = 2; i < tokens.getSize(); i++)
                {
                    if (isOperator(tokens[i]))
                        containsOperators = true;
                    expression += tokens[i] + " ";
                }

                string type;
                if (isStringLiteral(tokens[2]))
                {
                    type = "string";
                }
                else
                {
                    type = containsOperators ? "auto" : inferType(tokens[2]);
                }

                cppFile << "    " << type << " " << varName << " = " << expression << ";\n";
            }
        }

        cppFile << "    return 0;\n}\n";
        cppFile.close();
        file.close();

        // Compile the transpiled C++ code
        if (system("g++ output.cpp -o output") != 0)
        {
            throw Error("Compilation failed!");
        }

        // Run the compiled program
        if (system("./output") != 0)
        {
            throw Error("Execution failed!");
        }
    }
    catch (Error &e)
    {
        e.log();
    }

    return 0;
}
