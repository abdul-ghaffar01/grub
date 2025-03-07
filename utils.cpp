#ifndef UTILS
#define UTILS
#include <iostream>
#include "list.cpp"
using namespace std;

bool isReserved(const string &word) {
    const int reservedCount = 5;
    const string reservedWords[reservedCount] = {"if", "else", "while", "for", "return"};
    
    for (int i = 0; i < reservedCount; i++) {
        if (word == reservedWords[i]) {
            return true;
        }
    }
    return false;
}


List<string> split(const string &str, char delimiter = ' ') {
    List<string> tokens;
    string token;
    
    for (char ch : str) {
        if (ch == delimiter) {
            if (!token.empty()) {
                tokens.push(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    
    if (!token.empty()) {
        tokens.push(token);
    }

    return tokens;
}

#endif
