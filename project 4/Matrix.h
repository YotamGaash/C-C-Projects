
// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

// You don't have to use the struct. Can help you with MlpNetwork.h
struct matrix_dims {
    int rows, cols;
};


// Insert Matrix class here...
class Matrix {
public:
    // Constructors
    Matrix(int rows, int cols);       // Matrix of size rows x cols
    Matrix();                         //Default constructor (1x1 matrix)
    Matrix(const Matrix& m);         //Copy constructor

    // Destructor
    ~Matrix();

    //Getter functions
    int get_rows() const {return this->numRows;};
    int get_cols() const {return this->numCols;};


    // Transforms a matrix into its transpose matrix
    Matrix& transpose();

private:
    float** data;  // 2D array to store matrix elements
    int numRows, numCols;            // Dimensions of the matrix
};



#endif //MATRIX_H