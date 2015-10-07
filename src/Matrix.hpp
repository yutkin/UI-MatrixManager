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
    Matrix(size_t, size_t,  T initial_val = T());
    Matrix(const Matrix&);
    Matrix(Matrix&&);
    ~Matrix();

    size_t rows() const;
    size_t columns() const;
    size_t size() const;
    T max() const;
    T min() const;
    bool isSquare() const;
    bool isZeroMatrix() const;
    virtual std::string toStr() const;

    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    T&       operator()(const size_t, const size_t);
    const T& operator()(const size_t, const size_t) const;
    Matrix&  operator=(const Matrix&);
    Matrix&  operator=(Matrix&&);
    Matrix   operator*(const Matrix<T>&);
    Matrix   operator+(const Matrix<T>&);
    bool     operator==(const Matrix<T>&);

    Matrix& sort(const std::function<bool(T, T)> f = std::less<T>());

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const Matrix<U>&);

    template <typename U>
    friend std::istream& operator>>(std::istream&, Matrix<U>&);

protected:
    size_t _rows = 0;
    size_t _columns = 0;
    T* data = nullptr;
    T* data_end = nullptr;
};

template <typename T>
Matrix<T>::Matrix(size_t n, size_t m, T initial_val) {
    _rows = n;
    _columns = m;
    size_t sz = _rows * _columns;
    data = new T[sz]();
    data_end = data + sz;
    for (size_t i = 0; i < sz; ++i) {
        data[i] = initial_val;
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix& rMatrix) {
    size_t sz = rMatrix._rows * rMatrix._columns;
    _rows = rMatrix._rows;
    _columns = rMatrix._columns;
    data = new T[sz]();
    data_end = data + sz;
    std::copy(rMatrix.data, rMatrix.data_end, data);
}

template <typename T>
Matrix<T>::Matrix(Matrix&& rMatrix) {
    _rows = rMatrix._rows;
    _columns = rMatrix._columns;
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
inline size_t Matrix<T>::rows() const {
    return _rows;
}

template <typename T>
inline size_t Matrix<T>::columns() const {
    return _columns;
}

template <typename T>
inline size_t Matrix<T>::size() const {
    return _rows * _columns;
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
    return _rows == _columns;
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
    return "Matrix " + to_string(_rows) + "x" + to_string(_columns);
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
    if (i < _rows && i >= 0 && j < _columns && j >= 0) {
        return data[i* _columns + j];
    } else {
        throw std::out_of_range("matrix indices out of range");
    }
}

template <typename T>
const T& Matrix<T>::operator()(const size_t i, const size_t j) const {
    if (i < _rows && i >= 0 && j < _columns && j >= 0) {
        return data[i* _columns + j];
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
    size_t sz = rMatrix._rows * rMatrix._columns;
    _rows = rMatrix._rows;
    _columns = rMatrix._columns;
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
    _rows = rMatrix._rows;
    _columns = rMatrix._columns;
    data = rMatrix.data;
    data_end = rMatrix.data_end;
    rMatrix.data = nullptr;
    rMatrix.data_end = nullptr;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &rMatrix) {
    if (_columns != rMatrix._rows) {
        throw std::domain_error("Multiplication is possible for N x M and M x Q matrixes");
    }
    Matrix<T> new_m {_rows, rMatrix._columns};
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t k = 0; k < rMatrix._rows; ++k) {
            for (size_t j = 0; j < rMatrix._columns; ++j) {
                new_m(i, j) += (*this)(i, k) * rMatrix(k, j);
            }
        }
    }
    return new_m;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &rMatrix) {
    if (_columns != rMatrix._columns || _rows != rMatrix._rows) {
        throw std::domain_error("Addition is possible for N x M and N x M matrixes");
    }
    Matrix<T> new_m {_rows, _columns};
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            new_m(i, j) = (*this)(i, j) + rMatrix(i, j);
        }
    }
    return new_m;
}

template <typename T>
inline bool Matrix<T>::operator==(const Matrix<T> &rMatrix) {
    return _rows == rMatrix._rows && _columns == rMatrix._columns &&
            std::equal(data, data_end, rMatrix.data);
}

template <typename T>
Matrix<T>& Matrix<T>::sort(const std::function<bool(T, T)> f) {
    std::sort(data, data_end, f);
    return *this;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& rMatrix) {
    for (size_t i = 0; i < rMatrix.rows(); ++i) {
        os << "\n";
        for (size_t j = 0; j < rMatrix.columns(); ++j) {
            os << std::left << std::setw(4) << rMatrix(i, j) << " ";
        }
    }
    return os;
}

template <typename U>
std::istream& operator>>(std::istream& is, Matrix<U>& rMatrix) {
    for (size_t i = 0; i < rMatrix.rows(); ++i) {
        for (size_t j = 0; j < rMatrix.columns(); ++j) {
            is >> rMatrix(i, j);
        }
    }
    return is;
}

#endif  // MATRIX_HPP
