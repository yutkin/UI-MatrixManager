#ifndef FINDMATRIXDIALOG_HPP
#define FINDMATRIXDIALOG_HPP

#include <QtWidgets>
#include "MatrixListModel.hpp"

namespace Ui {
class FindMatrixDialog;
}

class FindMatrixDialog : public QDialog
{
    Q_OBJECT

public:
    FindMatrixDialog(MatrixModel *, QWidget *parent = 0);
    ~FindMatrixDialog();

signals:
    void matrixRemoved(int);

private slots:
    void findBtnClicked();
    void matrixItemDblClicked(QModelIndex);

private:
    MatrixModel *listModel;
    std::unique_ptr<QSortFilterProxyModel> proxyModel;
    Ui::FindMatrixDialog *ui;
    void keyPressEvent(QKeyEvent *event);
};

#endif // FINDMATRIXDIALOG_HPP
