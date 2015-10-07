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

    if (rows == "" || columns == "") {
        auto text = QString("Please, fill all fields!");
        QMessageBox::information(this, "", text);
        return;
    }

    auto searchPattern = QString("%1x%2").arg(rows).arg(columns);
    proxyModel = std::move(std::unique_ptr<QSortFilterProxyModel>(new QSortFilterProxyModel(this)));
    proxyModel->setSourceModel(listModel);
    proxyModel->setFilterRegExp(searchPattern);
    ui->listView->setModel(proxyModel.get());
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

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

void FindMatrixDialog::keyPressEvent(QKeyEvent *event) {
    auto selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
    auto len = selectedIndexes.length();

    if (event->key() == Qt::Key_Backspace && len > 0) {
        QString request = (len == 1) ? "Do you really want to remove this matrix?" :
            (len > 1)  ? QString("Do you really want to remove %1 matrixes?").arg(len) : "";

        auto reply = QMessageBox::question(this, "", request);

        if (reply == QMessageBox::Yes) {
            foreach(const QModelIndex &index, selectedIndexes){
                emit matrixRemoved(index.row());
            }
            ui->listView->selectionModel()->clear();
        }
    }
}
