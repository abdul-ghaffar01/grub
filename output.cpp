#include <iostream>
using namespace std;
int main() {
    double x = 10.3 ;
    double y = 20.6 ;
    auto z = x + y * (y-x) ;
    cout << x  << " " << "+"  << " " << y  << " " << "is"  << " " << z << "\n";
    return 0;
}
