#include <iostream>
#include <fstream>
#include "error.cpp"
using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        // Checking if the program was run with file name as argument
        if (argc == 1)
        {
            throw Error("No source file provided");
        }

        // checking if the file has extension .gr
        string filename = argv[1];
        if (filename.substr(filename.find_last_of(".") + 1) != "gr")
        {
            throw Error("Invalid file type");
        }
        
        // Checking if the file exists
        ifstream file(filename);
        if (!file)
        {
            throw Error("File not found: " + filename);
        }

        // Reading the contents of the file
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }

        file.close();
    }
    catch (Error e)
    {
        e.log();
    }

    return 0;
}