#ifndef MATRIXVIEWDIALOG_HPP
#define MATRIXVIEWDIALOG_HPP

#include <QDialog>
#include "Matrix.hpp"

namespace Ui {
class MatrixViewDialog;
}

class MatrixViewDialog : public QDialog
{
    Q_OBJECT

public:
    MatrixViewDialog(std::shared_ptr<Matrix<int>>&, int, QWidget *parent = nullptr);
    ~MatrixViewDialog();

signals:
    void matrixRemoved(int);

private slots:
    void saveBtnClicked();
    void removeBtnClicked();

private:
    Ui::MatrixViewDialog *ui;
    std::shared_ptr<Matrix<int>> data;
    int _pos;
};

#endif // MATRIXVIEWDIALOG_HPP
