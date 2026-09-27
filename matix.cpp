#include <iostream>
#include <matrix.h>

using namespace std;

class Matrix{
    private:
        int rows;
        int cols;
        float* data; // 1D array in row-major order
    public:
        Matrix(int r, int c) {
            data = new float[rows*cols];
            for (int i=0; i<(rows*cols); i++) {
                data[i] = 0;   // *(data + i) = 0;
            }
        }
        ~Matrix() {
            delete[] data;
        }

        void print() {
            for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++){
                    float element = data[i*rows + j];
                    cout << element << " ";
                }
            cout << endl;
            }
        }
        
}