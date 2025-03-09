    #include <cmath>

#include <iostream>
using namespace std;
int main() {
    int i = 0 ;
    while (i < 5 ) {
        cout << "value of i is"  << " " << i << "\n";
    i = i+1 ;
}
    cout << "Execution of cpp code starts here" << endl;
    // this is a comment of cpp
    for(int i = 0; i<5; i++){
        for(int j = 0; j<=i; j++){
            cout << "* ";
        }cout << endl;
    }
    cout << "The square root of 16 is " << sqrt(16) << endl;
    cout << "Execution of cpp code ends here" << endl;
    return 0;
}
