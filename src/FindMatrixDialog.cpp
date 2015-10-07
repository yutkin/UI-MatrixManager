#include <QMessageBox>

#include "src/FindMatrixDialog.hpp"
#include "src/MatrixViewDialog.hpp"
#include "ui_FindMatrixDialog.h"

FindMatrixDialog::FindMatrixDialog(MatrixModel *_model, QWidget *parent) :
    QDialog(parent),
    listModel(_model),
    ui(new Ui::FindMatrixDialog)
{
    ui->setupUi(this);

    ui->listView->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->rowsEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->columnsEdit->setAttribute(Qt::WA_MacShowFocusRect, false);

    auto uIntValidator = new QRegExpValidator(QRegExp("[1-9]\\d*"), this);
    ui->rowsEdit->setValidator(uIntValidator);
    ui->columnsEdit->setValidator(uIntValidator);

    connect(ui->findBtn, SIGNAL(clicked()), SLOT(findBtnClicked()));

    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(matrixItemDblClicked(QModelIndex)));
}

FindMatrixDialog::~FindMatrixDialog() {
    delete ui;
}

void FindMatrixDialog::findBtnClicked() {
    auto rows = ui->rowsEdit->text();
    auto columns = ui->columnsEdit->text();
    auto searchPattern = QString("Matrix %1x%2").arg(rows).arg(columns);

    auto proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(listModel);
    proxyModel->setFilterRegExp(searchPattern);
    ui->listView->setModel(proxyModel);
    if (proxyModel->rowCount() == 0) {
        auto text = QString("There are no %1x%2 matrixes!").arg(rows).arg(columns);
        QMessageBox::information(this, "", text);
    }
}

void FindMatrixDialog::matrixItemDblClicked(QModelIndex index) {
    auto pos = index.row();
    auto selectedMatrix = listModel->getData()[pos];
    auto matrixViewDialog = new MatrixViewDialog(selectedMatrix, pos, this);

    connect(matrixViewDialog, SIGNAL(matrixRemoved(int)), SIGNAL(matrixRemoved(int)));

    matrixViewDialog->setModal(true);
    matrixViewDialog->show();
}
