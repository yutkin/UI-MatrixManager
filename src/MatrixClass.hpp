// Copyright 2015 Yutkin Dmitry

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <functional>
#include <iomanip>

template <typename T>
class Matrix {
public:
    Matrix() = default;
    Matrix(size_t n, size_t m,  T initial_val = T());
    Matrix(const Matrix& rMatrix);
    Matrix(Matrix&& rMatrix);
    ~Matrix();

    Matrix& setNewDims(const size_t newRows, const size_t newCols);
    size_t numRows() const;
    size_t numCols() const;
    size_t size() const;
    T max() const;
    T min() const;
    bool isSquare() const;
    bool isZeroMatrix() const;
    std::string toStr() const;

    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    T&       operator()(const size_t i, const size_t j);
    const T& operator()(const size_t i, const size_t j) const;
    bool     operator==(const Matrix<T>& m);
    Matrix&  operator=(const Matrix& rMatrix);
    Matrix&  operator=(Matrix&& rMatrix);
    Matrix   operator*(const Matrix<T>& m);
    Matrix   operator+(const Matrix<T>& m);

    Matrix& sort(const std::function<bool(T, T)> f = std::less<T>());

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

    template <typename U>
    friend std::istream& operator>>(std::istream& is, Matrix<U>& m);

private:
    size_t rows = 0;
    size_t cols = 0;
    T* data = nullptr;
    T* data_end = nullptr;
};

template <typename T>
Matrix<T>::Matrix(size_t n, size_t m, T initial_val) : rows(n), cols(m) {
    if (rows >= 0 && cols >= 0) {
        size_t sz = rows*cols;
        data = new T[sz]();
        data_end = data + sz;
        for (size_t i = 0; i < sz; ++i) {
            data[i] = initial_val;
        }
    } else {
        throw std::domain_error("matrix dimensions should be positive integers");
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix& rMatrix) {
    size_t sz = rMatrix.rows * rMatrix.cols;
    rows = rMatrix.rows;
    cols = rMatrix.cols;
    data = new T[sz]();
    data_end = data + sz;
    std::copy(rMatrix.data, rMatrix.data_end, data);
}

template <typename T>
Matrix<T>::Matrix(Matrix&& rMatrix) {
    rows = rMatrix.rows;
    cols = rMatrix.cols;
    data = rMatrix.data;
    data_end = rMatrix.data_end;
    rMatrix.data = nullptr;
    rMatrix.data_end = nullptr;
}

template <typename T>
inline Matrix<T>::~Matrix() {
    delete[] data;
}

template <typename T>
Matrix<T>& Matrix<T>::setNewDims(const size_t newRows, const size_t newCols) {
    size_t newsz = newRows * newCols;
    T *tmp_data = new T[newsz]();

    delete[] data;
    data = tmp_data;
    data_end = data + newsz;
    rows = newRows;
    cols = newCols;
    return *this;
}

template <typename T>
inline size_t Matrix<T>::numRows() const {
    return rows;
}

template <typename T>
inline size_t Matrix<T>::numCols() const {
    return cols;
}

template <typename T>
inline size_t Matrix<T>::size() const {
    return rows * cols;
}

template <typename T>
inline T Matrix<T>::max() const {
    return *std::max_element(data, data_end);
}

template <typename T>
inline T Matrix<T>::min() const {
    return *std::min_element(data, data_end);
}

template <typename T>
inline bool Matrix<T>::isSquare() const {
    return rows == cols;
}

template <typename T>
bool Matrix<T>::isZeroMatrix() const {
    for (auto it = data; it != data_end; ++it) {
        if (*it != 0) {
            return false;
        }
    }
    return true;
}

template <typename T>
std::string Matrix<T>::toStr() const {
    using std::to_string;
    return "Matrix " + to_string(rows) + "x" + to_string(cols);
}

template <typename T>
inline typename Matrix<T>::iterator Matrix<T>::begin() {
    return data;
}

template <typename T>
inline typename Matrix<T>::iterator Matrix<T>::end() {
    return data_end;
}

template <typename T>
inline typename Matrix<T>::const_iterator Matrix<T>::cbegin() const {
    return data;
}

template <typename T>
inline typename Matrix<T>::const_iterator Matrix<T>::cend() const {
    return data_end;
}

template <typename T>
T& Matrix<T>::operator()(const size_t i, const size_t j) {
    if (i < rows && i >= 0 && j < cols && j >= 0) {
        return data[i*cols + j];
    } else {
        throw std::out_of_range("matrix indices out of range");
    }
}

template <typename T>
const T& Matrix<T>::operator()(const size_t i, const size_t j) const {
    if (i < rows && i >= 0 && j < cols && j >= 0) {
        return data[i*cols + j];
    } else {
        throw std::out_of_range("matrix indices out of range");
    }
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& rMatrix) {
    if (this == &rMatrix) {
        return *this;
    }
    delete[] data;
    size_t sz = rMatrix.rows * rMatrix.cols;
    rows = rMatrix.rows;
    cols = rMatrix.cols;
    data = new T[sz]();
    data_end = data + sz;
    std::copy(rMatrix.data, rMatrix.data_end, data);
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& rMatrix) {
    if (this == &rMatrix) {
        return *this;
    }
    delete[] data;
    rows = rMatrix.rows;
    cols = rMatrix.cols;
    data = rMatrix.data;
    data_end = rMatrix.data_end;
    rMatrix.data = nullptr;
    rMatrix.data_end = nullptr;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &m) {
    if (cols != m.rows) {
        throw std::domain_error("Multiplication is possible for N x M and M x Q matrixes");
    }
    Matrix<T> new_m {rows, m.cols};
    for (size_t i = 0; i < rows; ++i) {
        for (size_t k = 0; k < m.rows; ++k) {
            for (size_t j = 0; j < m.cols; ++j) {
                new_m(i, j) += (*this)(i, k) * m(k, j);
            }
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m) {
    if (cols != m.cols || rows != m.rows) {
        throw std::domain_error("Addition is possible for N x M and N x M matrixes");
    }
    Matrix<T> new_m {rows, cols};
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            new_m(i, j) = (*this)(i, j) + m(i, j);
        }
    }
    return new_m;
}

template <typename T>
inline bool Matrix<T>::operator==(const Matrix<T> &m) {
    return std::equal(data, data_end, m.data);
}

template <typename T>
Matrix<T>& Matrix<T>::sort(const std::function<bool(T, T)> f) {
    std::sort(data, data_end, f);
    return *this;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& m) {
    for (size_t i = 0; i < m.numRows(); ++i) {
        os << "\n";
        for (size_t j = 0; j < m.numCols(); ++j) {
            os << std::left << std::setw(4) << m(i, j) << " ";
        }
    }
    return os;
}

template <typename U>
std::istream& operator>>(std::istream& is, Matrix<U>& m) {
    for (size_t i = 0; i < m.numRows(); ++i) {
        for (size_t j = 0; j < m.numCols(); ++j) {
            is >> m(i, j);
        }
    }
    return is;
}

#endif  // MATRIX_HPP
