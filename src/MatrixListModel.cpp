#include "MatrixListModel.hpp"

MatrixModel::MatrixModel(QObject *parent) : QAbstractListModel(parent) {}

int MatrixModel::rowCount(const QModelIndex &parent) const {
    return matrixes.size();
}

QVariant MatrixModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QString::fromStdString(matrixes[index.row()]->toStr());
    else
        return QVariant();
}

deque<std::shared_ptr<Matrix<int>>>& MatrixModel::getData() {
    return matrixes;
}
