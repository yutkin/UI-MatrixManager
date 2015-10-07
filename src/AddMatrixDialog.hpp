#ifndef ADDMATRIXDIALOG_H
#define ADDMATRIXDIALOG_H

#include <QDialog>
#include "PositionEnum.hpp"
#include "Matrix.hpp"
#include "MatrixExtended.hpp"

namespace Ui {
    class AddMatrixDialog;
}

class AddMatrixDialog : public QDialog {
    Q_OBJECT

public:
    AddMatrixDialog(QWidget *, Position);
    ~AddMatrixDialog();
signals:
    void addedNewMatrix(Position, std::shared_ptr<Matrix<int>>&);

private slots:
    void okBtnClicked();
    void rowsEditEditingFinished();
    void columnsEditEditingFinished();
private:
    Ui::AddMatrixDialog *ui;
    Position _pos;

    void fillTable(size_t, size_t);
};

#endif // ADDMATRIXDIALOG_H
