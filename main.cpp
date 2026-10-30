#include <iostream>
#include "matrix.h"

using namespace std;

int main() {
    // int r,c;
    // cout << "Enter Rows: ";
    // cin >> r;
    // cout << "Enter Columns: ";
    // cin >> c;

    Matrix A(2,3);
    A(0, 0) = 1.0f; A(0, 1) = 2.0f; A(0, 2) = 3.0f;
    A(1, 0) = 4.0f; A(1, 1) = 5.0f; A(1, 2) = 6.0f;

    // int r2,c2;
    // cout << "Enter Matrix B Rows: ";
    // cin >> r2;
    // cout << "Enter Matrix B Columns: ";
    // cin >> c2;

    Matrix B(3,2);
    B(0, 0) = 7.0f;  B(0, 1) = 8.0f;
    B(1, 0) = 9.0f;  B(1, 1) = 1.0f;
    B(2, 0) = 2.0f;  B(2, 1) = 3.0f;

    Matrix C = B;

    A.print();
    B.print();
    C.print();

    cout << "=========Multiply Test=========" << endl;
    Matrix D = A.multiply(B);
    D.print();

    cout << "=========Transpose Test=========" << endl;
    Matrix E = A.transpose();
    E.print();

    cout << "=========Element-wise Multiply Test=========" << endl;
    Matrix F = A.elementwise_multiply(A);
    F.print();

    cout << "=========Add Test=========" << endl;
    Matrix G = A.add(A);
    G.print();

    cout << "=========Subtract Test=========" << endl;
    Matrix H = B.subtract(B);
    H.print();

    cout << "=========Scalar-Multiplication Test=========" << endl;
    Matrix I = B.scalar_multiply(2.0f);
    I.print();
    
    cout << "=========Sum Test (Axis=0)=========" << endl;
    Matrix J = A.sum(0);
    J.print();

    cout << "=========Sum Test (Axis=1)=========" << endl;
    J = A.sum(1);
    J.print();

    //     try {
    //     cout << "=========================================\n";
    //     cout << " TEST 1: Rectangular Matrix Multiply (2x3) * (3x2)\n";
    //     cout << "=========================================\n";

    //     // Matrix A: 2 rows, 3 columns
    //     Matrix A(2, 3);
    //     A(0, 0) = 1.0f; A(0, 1) = 2.0f; A(0, 2) = 3.0f;
    //     A(1, 0) = 4.0f; A(1, 1) = 5.0f; A(1, 2) = 6.0f;

    //     cout << "Matrix A (2x3):\n";
    //     A.print();

    //     // Matrix B: 3 rows, 2 columns (Compatible: A.cols == B.rows)
    //     Matrix B(3, 2);
    //     B(0, 0) = 7.0f;  B(0, 1) = 8.0f;
    //     B(1, 0) = 9.0f;  B(1, 1) = 1.0f;
    //     B(2, 0) = 2.0f;  B(2, 1) = 3.0f;

    //     cout << "Matrix B (3x2):\n";
    //     B.print();

    //     // Expected C (2x2):
    //     // Row 0: [1*7 + 2*9 + 3*2,  1*8 + 2*1 + 3*3] = [31, 19]
    //     // Row 1: [4*7 + 5*9 + 6*2,  4*8 + 5*1 + 6*3] = [85, 55]
    //     Matrix C = A.multiply(B);
    //     cout << "Result Matrix C = A * B (Expected 2x2):\n";
    //     C.print();

    //     cout << "=========================================\n";
    //     cout << " TEST 2: Reverse Rectangular Multiply (3x2) * (2x3)\n";
    //     cout << "=========================================\n";

    //     // B * A produces a 3x3 matrix
    //     Matrix D = B.multiply(A);
    //     cout << "Result Matrix D = B * A (Expected 3x3):\n";
    //     D.print();

    //     cout << "=========================================\n";
    //     cout << " TEST 3: Multiply With Transpose (A * A^T)\n";
    //     cout << "=========================================\n";

    //     // A is (2x3), A.transpose() is (3x2)
    //     Matrix At = A.transpose();
    //     Matrix E = A.multiply(At);
    //     cout << "Result Matrix E = A * A^T (Expected 2x2):\n";
    //     E.print();

    //     cout << "=========================================\n";
    //     cout << " TEST 4: Invalid Dimensions Exception Check\n";
    //     cout << "=========================================\n";

    //     // Multiplying A (2x3) by A (2x3) should fail (3 != 2)
    //     cout << "Attempting invalid multiplication A(2x3) * A(2x3)...\n";
    //     Matrix Invalid = A.multiply(A);
    //     Invalid.print();

    // } catch (const invalid_argument& e) {
    //     cout << "Caught expected invalid_argument: " << e.what() << "\n";
    // } catch (const out_of_range& e) {
    //     cout << "Caught out_of_range exception: " << e.what() << "\n";
    // } catch (const exception& e) {
    //     cout << "Caught generic exception: " << e.what() << "\n";
    // }
}