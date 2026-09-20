class Matrix {
private:
    int rows;
    int cols;
    float* data; // 1D array in row-major order

public:
    // 1. Constructors & Destructor (Memory Management)
    Matrix(int rows, int cols); 
    ~Matrix(); // Required to free float* data

    // 2. Rule of Three (Prevents shallow-copy crashes)
    Matrix(const Matrix& other);            // Copy Constructor
    Matrix& operator=(const Matrix& other); // Copy Assignment

    // 3. Element Access
    // e.g., matrix(0, 1) to get/set row 0, col 1
    float& operator()(int row, int col);
    float operator()(int row, int col) const;

    // 4. Core Math Operations (Week 1 Requirements)
    Matrix multiply(const Matrix& other) const;
    Matrix transpose() const;
    Matrix elementwise_multiply(const Matrix& other) const;
    Matrix add(const Matrix& other) const;
    Matrix subtract(const Matrix& other) const;
    Matrix scalar_multiply(float k) const;
    
    // Axis-wise summation
    Matrix sum(int axis) const; 

    // 5. Operator Overloading (Project Proposal Requirement)
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const; // Matrix multiplication

    // Debugging utility
    void print() const; 
};