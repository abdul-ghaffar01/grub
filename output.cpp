#include <iostream>
using namespace std;
int main() {
    int x = 10 ;
    int y = 10 ;
    auto z = x + y * (y-x) ;
    if (x > y ) 
        cout << "x is greater" << "\n";
    else if (x < y ) 
        cout << "y is greater" << "\n";
    else 
        cout << "x and y are equal" << "\n";
        cout << x  << " " << "+"  << " " << y  << " " << "is"  << " " << z << "\n";
    return 0;
}
