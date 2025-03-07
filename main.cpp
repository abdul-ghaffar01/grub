#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unordered_set>
#include "error.cpp"
#include "list.cpp"
#include "utils.cpp"

using namespace std;

void prependToFile(const string &filename, const string &text)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "File not found: " << filename << endl;
        return;
    }

    string existingContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Failed to open file for writing." << endl;
        return;
    }

    outFile << text << endl
            << existingContent; // Prepend text and then append old content
    outFile.close();
}

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
    string includes = "";
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
        string indentation = "    "; // 4 spaces for nested conditions
        bool insideBlock = false;
        unordered_set<string> declaredVariables;

        while (getline(file, line))
        {
            List<string> tokens = split(line);
            if (tokens.getSize() == 0)
                continue; // Ignore empty lines

            if (tokens[0] == "includes{") // Handling include blocks
            {
                while (getline(file, line))
                {
                    if (line == "}")
                    {
                        break; // Stop when closing brace is found
                    }
                    // cppFile << line << "\n"; // Write includes at the top
                    includes += line + '\n';
                }
            }
            else if (tokens[0] == "cpp{" || tokens[0] == "cpp") // Handling raw C++ blocks
            {
                List<char> braceStack; // Stack to track `{ }`
                braceStack.push('{');  // Push the first opening brace

                while (getline(file, line))
                {
                    if (line.find("{") != string::npos)
                    {
                        braceStack.push('{'); // Push if another { is found
                    }
                    if (line.find("}") != string::npos)
                    {
                        braceStack.pop(); // Pop when } is found
                        if (braceStack.empty())
                        {
                            break; // Stop when all { are closed
                        }
                    }
                    cppFile << line << "\n"; // Write raw C++ code
                }
            }

            else if (tokens[0] == "if" || tokens[0] == "elif" || tokens[0] == "else")
            {
                if (tokens[0] == "elif") // else if case
                {
                    cppFile << "    else if (";
                    for (int i = 1; i < tokens.getSize(); i++)
                    {
                        cppFile << tokens[i] << " ";
                    }
                    cppFile << ") \n";
                }
                else if (tokens[0] == "if") // if case
                {
                    cppFile << "    if (";
                    for (int i = 1; i < tokens.getSize(); i++)
                    {
                        cppFile << tokens[i] << " ";
                    }
                    cppFile << ") \n";
                }
                else // else case (NO CONDITION!)
                {
                    cppFile << "    else \n";
                }
            }
            else if (tokens[0] == "while") // Handling while loops
            {
                cppFile << "    while (";
                for (int i = 1; i < tokens.getSize(); i++)
                {
                    cppFile << tokens[i] << " ";
                }
                cppFile << ") {\n";
            }
            else if (tokens[0] == "end")
            {
                cppFile << "}\n";
            }
            else if (tokens[0] == "print") // Inside if-else block
            {
                cppFile << indentation << "    cout << ";
                for (int i = 1; i < tokens.getSize(); i++)
                {
                    if (i > 1)
                        cppFile << " << \" \" << ";

                    if (tokens[i][0] == '"')
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
                cppFile << "<< \"\\n\";\n";
            }
            else if (insideBlock) // Closing if-else block
            {
                cppFile << indentation << "}\n";
                insideBlock = false;
            }
            else if (tokens.getSize() >= 3 && tokens[1] == "=") // Variable assignment
            {
                string varName = tokens[0];
                string expression = "";
                bool containsOperators = false;

                for (int i = 2; i < tokens.getSize(); i++)
                {
                    if (isOperator(tokens[i]))
                        containsOperators = true;
                    expression += tokens[i] + " ";
                }

                if (declaredVariables.find(varName) == declaredVariables.end())
                {
                    string type;
                    if (isStringLiteral(tokens[2]))
                    {
                        type = "string";
                    }
                    else
                    {
                        type = containsOperators ? "auto" : inferType(tokens[2]);
                    }
                    cppFile << indentation << type << " " << varName << " = " << expression << ";\n";
                    declaredVariables.insert(varName);
                }
                else
                {
                    cppFile << indentation << varName << " = " << expression << ";\n";
                }
            }
        }

        cppFile << "    return 0;\n}\n";
        
        cppFile.close();
        file.close();

        // appending the includes at the top
        prependToFile("output.cpp", includes);
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
