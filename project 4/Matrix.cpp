

#include "Matrix.h"

// Constructor for a matrix with specific dimensions
Matrix::Matrix(int rows, int cols) : numRows(rows), numCols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Invalid dimensions for matrix");
    }

    // Allocate memory for the rows
    data = new float*[numRows];
    if (!data) {
        throw std::bad_alloc();  //memory allocation failure
    }
    // Allocate memory for each element in each row
    for (int i = 0; i < numRows; i++) {
        data[i] = new float[numCols]();
        if (!data[i]) {
            // Free previously allocated rows in case of allocation failure
            for (int j = 0; j < i; j++) {
                delete[] data[j];
            }
            delete[] data;
            throw std::bad_alloc();
        }
    }
}

// Default constructor calls the constructor for 1x1 matrix
Matrix::Matrix() : Matrix(1, 1) {}

// Copy constructor - const ref to not change the original matrix
Matrix::Matrix(const Matrix& m) : numRows(m.numRows), numCols(m.numCols) {
    // Allocate memory for the new matrix
    data = new float*[numRows];
    if (!data) {
        throw std::bad_alloc();
    }

    // Deep copy the data
    for (int i = 0; i < numRows; i++) {
        data[i] = new float[numCols];
        if (!data[i]) {
            // Free previously allocated rows in case of allocation failure
            for (int j = 0; j < i; j++) {
                delete[] data[j];
            }
            delete[] data;
            throw std::bad_alloc();
        }

        for (int j = 0; j < numCols; j++) {
            data[i][j] = m.data[i][j];
        }
    }
}

// Destructor
Matrix::~Matrix() {
    // Deallocate memory for each row and then the main data array
    for (int i = 0; i < numRows; i++) {
        delete[] data[i];
    }
    delete[] data;
}

Matrix &Matrix::transpose() {
    return;
}

