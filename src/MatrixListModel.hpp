#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H

#include <QAbstractListModel>
#include <deque>
#include "MatrixClass.hpp"

class MatrixModel : public QAbstractListModel {
public:
    explicit MatrixModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    std::deque<Matrix<int>>& getData();

private:
    std::deque<Matrix<int>> matrixes;
};

Q_DECLARE_METATYPE(Matrix<int>);

#endif // MATRIXMODEL_H
