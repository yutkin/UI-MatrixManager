#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H

#include <QAbstractListModel>
#include <deque>
#include "Matrix.hpp"
#include "MatrixExtended.hpp"

using std::deque;

class MatrixModel : public QAbstractListModel {
public:
    explicit MatrixModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    deque<std::shared_ptr<Matrix<int>>>& getData();

private:
    deque<std::shared_ptr<Matrix<int>>> matrixes;
};

Q_DECLARE_METATYPE(Matrix<int>);

#endif // MATRIXMODEL_H
