class Matrix {
private:
    int rows;
    int cols;
    float* data; // 1D array in row-major order

public:
    // Constructor producing a zero-initialised matrix
    Matrix(int rows, int cols); 
    
    // Core Math Operations
    Matrix multiply(const Matrix& other) const;
    Matrix transpose() const;
    Matrix elementwise_multiply(const Matrix& other) const;
    Matrix add(const Matrix& other) const;
    Matrix subtract(const Matrix& other) const;
    Matrix scalar_multiply(float k) const;
    
    // Axis-wise summation
    Matrix sum(int axis) const; 

    // Debugging utility
    void print() const; 
};