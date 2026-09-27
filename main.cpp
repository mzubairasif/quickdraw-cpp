#include <iostream>
#include "matrix.cpp"

using namespace std;

int main() {
    int r,c;
    cout << "Enter Rows: ";
    cin >> r;
    cout << "Enter Columns: ";
    cin >> c;

    Matrix A(r,c);
    int first_element = A(0,0);
    A(0,0) = 1;
    A(1,1) = 1;
    A(2,2) = 1;

    Matrix B(A);
    Matrix C = B;

    A.print();
    B.print();
    C.print();
}