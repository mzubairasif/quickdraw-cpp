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
    // 1. Validation Check 
    if (this->cols != other.rows) {
        throw std::invalid_argument("Matrix multiply error: Inner dimensions do not match.");
    }

    Matrix new_matrix(this->rows, other.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            float k_value = 0.0f; // Fixed: Must be float
            
            for (int k = 0; k < this->cols; k++) {
                k_value += this->data[(i * this->cols) + k] * other.data[(k * other.cols) + j];
            }
            
            new_matrix.data[(i * other.cols) + j] = k_value;
        }
    }
    
    return new_matrix;
}

Matrix Matrix::transpose() const {
    // a new matrix with swapped dimensions
    Matrix new_matrix(this->cols, this->rows);
    
    // Loop through the original matrix
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            // Mapping the 1D indices
            // Original index at (i, j)
            int orig_index = (i * this->cols) + j;
            
            // Transposed index at (j, i) 
            // Note that new matrix has 'this->rows' as its column count
            int trans_index = (j * this->rows) + i;
            
            // Assign the value
            new_matrix.data[trans_index] = this->data[orig_index];
        }
    }
    
    return new_matrix;
}

Matrix Matrix::elementwise_multiply(const Matrix& other) const {
    if (this->cols != other.cols || this->rows != other.rows) {
        throw std::invalid_argument("Matrix multiply error: Dimensions do not match.");
    }
    Matrix new_matrix(this->rows, this->cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            new_matrix(i, j) = this->operator()(i, j) * other(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::add(const Matrix& other) const {
    if (this->cols != other.cols || this->rows != other.rows) {
        throw std::invalid_argument("Matrix multiply error: Dimensions do not match.");
    }
    Matrix new_matrix(this->rows, this->cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            new_matrix(i, j) = this->operator()(i, j) + other(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::subtract(const Matrix& other) const {
    if (this->cols != other.cols || this->rows != other.rows) {
        throw std::invalid_argument("Matrix multiply error: Dimensions do not match.");
    }
    Matrix new_matrix(this->rows, this->cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            new_matrix(i, j) = this->operator()(i, j) - other(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::scalar_multiply(float k) const {
    Matrix new_matrix(this->rows, this->cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            new_matrix(i, j) = this->operator()(i, j) * k;
        }
    }
    return new_matrix;
}

Matrix Matrix::sum(int axis) const {
    if (axis != 0 && axis != 1) {
        throw std::invalid_argument("Invalid axis. Axis must be 0 (sum over rows) or 1 (sum over columns).");
    }
    Matrix new_matrix(0, this->cols);
    if (axis==0) {
        new_matrix = Matrix(1, this->cols);
    }
    else {
        new_matrix = Matrix(this->rows, 1);
    }
    for (int i=0; i<new_matrix.rows; i++) { 
        for (int j=0; j<new_matrix.cols; j++) {
            if (axis==0) {
                for (int k=0; k<this->rows; k++) {
                    new_matrix(i, j) += this->operator()(k, j);
                }
            }
        
            if (axis==1) {
                for (int k=0; k<this->cols; k++) {
                    new_matrix(i, j) += this->operator()(i, k);
                }
            }
        }
    }
    return new_matrix;
}
