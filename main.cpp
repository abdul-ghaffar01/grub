#include <iostream>
#include "error.cpp"
using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        // Checking if the program was run with arguments
        if (argc == 1)
        {
            throw Error("No source file provided");
        }
    }
    catch (Error e)
    {
        e.log();
    }

    return 0;
}