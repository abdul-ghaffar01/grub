#ifndef ERROR
#define ERROR

#include <iostream>
using namespace std;

class Error
{
private:
    string message;

public:
    Error(string message)
    {
        this->message = message;
    }
    void log()
    {
        cout << "Error: " << message << endl;
    }
};

#endif