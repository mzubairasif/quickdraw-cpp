#include <iostream>
#include "matrix.h"
#include <stdexcept>
#include <iomanip>

using namespace std;

// Constuctor
Matrix::Matrix(int r, int c) 
{      
    rows = r;
    cols = c;
    data = new float[rows*cols];
    for (int i=0; i<(rows*cols); i++) {
        data[i] = 0.0f;   // *(data + i) = 0;
    }
}

// Destructor
Matrix::~Matrix() 
{
    delete[] data;
}

// Constructor For Deep Copy
Matrix::Matrix(const Matrix& other) 
{
    rows = other.rows;
    cols = other.cols;
    data = new float[rows*cols];

    for (int i=0; i<(rows*cols); i++) {
        data[i] = other.data[i];   // *(data + i) = *(other.data + i);
    }
}

// Assignment Operator Overloading
Matrix& Matrix::operator=(const Matrix& other) 
{
    if (this == &other) return *this;   // Handle A = A case 
    delete[] data;      // Free existing memory

    rows = other.rows;
    cols = other.cols;
    data = new float[rows*cols];

    for (int i=0; i<(rows*cols); i++) {
        data[i] = other.data[i];   // *(data + i) = 0;
    }
    return *this;
}

// Set and Get value methods
float& Matrix::operator()(int row, int col)     // Returns modifiable Reference
{
    if (row<0 || row>=rows || col<0 || col>=cols) {
        throw std::out_of_range("Invalid Row or Column, Matrix Index Out of Bounds");
    }
    return data[(row * cols) + col];
}

float Matrix::operator()(int row, int col) const   // Returns value
{
    if (row<0 || row>=rows || col<0 || col>=cols) {
        throw std::out_of_range("Invalid Row or Column, Matrix Index Out of Bounds");
    }
    return data[(row * cols) + col];
}

void Matrix::print() const 
{
    cout << "-----------------------------------------\n";
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++){
            std::cout << std::setw(8) << std::setprecision(4) << this->operator()(i, j) << " ";
        }
    cout << endl;
    }
    cout << "-----------------------------------------\n";
}

Matrix Matrix::multiply(const Matrix& other) const {
    throw std::logic_error("multiply not implemented yet");
}

Matrix Matrix::transpose() const {
    throw std::logic_error("transpose not implemented yet");
}

Matrix Matrix::elementwise_multiply(const Matrix& other) const {
    throw std::logic_error("elementwise_multiply not implemented yet");
}

Matrix Matrix::add(const Matrix& other) const {
    throw std::logic_error("add not implemented yet");
}

Matrix Matrix::subtract(const Matrix& other) const {
    throw std::logic_error("subtract not implemented yet");
}

Matrix Matrix::scalar_multiply(float k) const {
    throw std::logic_error("scalar_multiply not implemented yet");
}

Matrix Matrix::sum(int axis) const {
    throw std::logic_error("sum not implemented yet");
}
