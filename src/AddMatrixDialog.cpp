#include "AddMatrixDialog.hpp"
#include "ui_AddMatrixDialog.h"

AddMatrixDialog::AddMatrixDialog(QWidget *parent, Position pos) :
    QDialog(parent),
    ui(new Ui::AddMatrixDialog),
    _pos(pos)
{
    ui->setupUi(this);
    ui->rowsEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->columnsEdit->setAttribute(Qt::WA_MacShowFocusRect, false);

    auto uIntValidator = new QRegExpValidator(QRegExp("[1-9]\\d*"), this);
    ui->rowsEdit->setValidator(uIntValidator);
    ui->columnsEdit->setValidator(uIntValidator);

    ui->matrixTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrixTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->okBtn, SIGNAL(clicked()), SLOT(okBtnClicked()));
    connect(ui->rowsEdit, SIGNAL(editingFinished()), SLOT(rowsEditEditingFinished()));
    connect(ui->columnsEdit, SIGNAL(editingFinished()), SLOT(columnsEditEditingFinished()));
}

AddMatrixDialog::~AddMatrixDialog() {
    delete ui;
}

void AddMatrixDialog::okBtnClicked() {
    auto rows = ui->rowsEdit->text().toUInt();
    auto columns = ui->columnsEdit->text().toUInt();
    Matrix<int> newMatrix {rows, columns, 0};

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            newMatrix(i, j) = ui->matrixTable->item(i, j)->text().toInt();
        }
    }

    emit addedNewMatrix(_pos, newMatrix);
    close();
}

void AddMatrixDialog::rowsEditEditingFinished() {
    auto rows = ui->rowsEdit->text().toUInt();
    auto columns = ui->columnsEdit->text().toUInt();
    if (rows != 0 && columns != 0)
        fillTable(rows, columns);
}

void AddMatrixDialog::columnsEditEditingFinished() {
    auto rows = ui->rowsEdit->text().toUInt();
    auto columns = ui->columnsEdit->text().toUInt();
    if (rows != 0 && columns != 0)
        fillTable(rows, columns);
}

void AddMatrixDialog::fillTable(size_t rows, size_t columns) {
    ui->matrixTable->setRowCount(rows);
    ui->matrixTable->setColumnCount(columns);

    if (rows != 0 && columns != 0) {
        ui->matrixTable->clear();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                auto newItem = new QTableWidgetItem("0");
                newItem->setTextAlignment(Qt::AlignCenter);
                ui->matrixTable->setItem(i, j, newItem);
            }
        }
    }
}
