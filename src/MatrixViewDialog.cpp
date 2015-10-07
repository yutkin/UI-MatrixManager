#include <QMessageBox>

#include "MatrixViewDialog.hpp"
#include "ui_MatrixViewDialog.h"

MatrixViewDialog::MatrixViewDialog(std::shared_ptr<Matrix<int>>& matrix, int pos, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatrixViewDialog),
    data(matrix),
    _pos(pos)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(matrix->toStr()) + " info");

    ui->tableWidget->setRowCount(matrix->rows());
    ui->tableWidget->setColumnCount(matrix->columns());
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (size_t i = 0; i < matrix->rows(); ++i) {
        for (size_t j = 0; j < matrix->columns(); ++j) {
            auto tmpItem = new QTableWidgetItem;

            auto tmpStr = QString::fromStdString(std::to_string((*matrix)(i,j)));
            tmpItem->setText(tmpStr);
            tmpItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, j, tmpItem);
        }
    }

    connect(ui->saveBtn, SIGNAL(clicked()), SLOT(saveBtnClicked()));
    connect(ui->removeBtn, SIGNAL(clicked()), SLOT(removeBtnClicked()));
}

MatrixViewDialog::~MatrixViewDialog() {
    delete ui;
}

void MatrixViewDialog::saveBtnClicked() {
    for (size_t i = 0; i < data->rows(); ++i) {
        for (size_t j = 0; j < data->columns(); ++j) {
            (*data)(i,j) = ui->tableWidget->item(i,j)->text().toInt();
        }
    }
    close();
}

void MatrixViewDialog::removeBtnClicked() {
    auto reply = QMessageBox::question(this, "", "Do you really want to remove this matrix?");
    if (reply == QMessageBox::Yes) {
        emit matrixRemoved(_pos);
        close();
    }
}
