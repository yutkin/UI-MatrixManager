#ifndef MATRIXVIEWDIALOG_HPP
#define MATRIXVIEWDIALOG_HPP

#include <QDialog>
#include "MatrixClass.hpp"

namespace Ui {
class MatrixViewDialog;
}

class MatrixViewDialog : public QDialog
{
    Q_OBJECT

public:
    MatrixViewDialog(Matrix<int>&, int, QWidget *parent = nullptr);
    ~MatrixViewDialog();

signals:
    void matrixChanged(Matrix<int>&, int);
    void matrixRemoved(int);

private slots:
    void saveBtnClicked();
    void removeBtnClicked();

private:
    Ui::MatrixViewDialog *ui;
    Matrix<int> data;
    int _pos;
};

#endif // MATRIXVIEWDIALOG_HPP
