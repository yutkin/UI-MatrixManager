#ifndef MATRIXEXTENDED_HPP
#define MATRIXEXTENDED_HPP

#include "Matrix.hpp"

template <typename T>
class MatrixExtended : public Matrix<T> {
public:
    MatrixExtended() = default;

    MatrixExtended(size_t n, size_t m,  T initial_val = T()) :
            Matrix<T>(n, m, initial_val) { };

    MatrixExtended(const MatrixExtended& rMatrix) :
            Matrix<T>(rMatrix) {};

    MatrixExtended(MatrixExtended&& rMatrix) :
            Matrix<T>(rMatrix) {};

    std::string toStr() const override;
};


#endif // MATRIXEXTENDED_HPP

template <typename T>
inline std::string MatrixExtended<T>::toStr() const {
    return "Matrix Extended " + std::to_string(this->_rows) + "x" + std::to_string(this->_columns);
}
