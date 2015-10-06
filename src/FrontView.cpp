#include <QtWidgets>
#include <QDebug>

#include "FrontView.hpp"
#include "PositionEnum.hpp"
#include "AddMatrixDialog.hpp"
#include "MatrixViewDialog.hpp"
#include "FindMatrixDialog.hpp"
#include "ui_FrontView.h"

FrontView::FrontView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->listView->setAttribute(Qt::WA_MacShowFocusRect, false);

    listModel = new MatrixModel(this);
    ui->listView->setModel(listModel);

    connect(ui->addBtn,      SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(ui->addToTopBtn, SIGNAL(clicked()), SLOT(addToTopButtonClicked()));
    connect(ui->findBtn,     SIGNAL(clicked()), SLOT(findBtnClicked()));

    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(matrixItemDblClicked(QModelIndex)));

}

FrontView::~FrontView() {
    delete ui;
}

void FrontView::addButtonClicked() {
    auto addMatrixDialog = new AddMatrixDialog(this, Position::Bottom);

    connect(addMatrixDialog, SIGNAL(addedNewMatrix(Position, Matrix<int>&)),
            SLOT(addMatrix(Position, Matrix<int>&)));

    addMatrixDialog->setModal(true);
    addMatrixDialog->show();
}

void FrontView::addToTopButtonClicked() {
    auto addMatrixDialog = new AddMatrixDialog(this, Position::Top);

    connect(addMatrixDialog, SIGNAL(addedNewMatrix(Position, Matrix<int>&)),
             SLOT(addMatrix(Position, Matrix<int>&)));

    addMatrixDialog->setModal(true);
    addMatrixDialog->show();
}

void FrontView::addMatrix(Position pos, Matrix<int>& matrix) {
    if (pos == Position::Bottom)
        listModel->getData().push_back(matrix);
    else if (pos == Position::Top)
        listModel->getData().push_front(matrix);

    updateTotalLabel();

    auto indexBegin = listModel->index(0, 0);
    auto indexEnd = listModel->index(listModel->rowCount()-1, 0);
    emit listModel->dataChanged(indexBegin, indexEnd);
}

void FrontView::matrixItemDblClicked(QModelIndex index) {
    auto pos = index.row();
    auto selectedMatrix = listModel->getData()[pos];
    auto matrixViewDialog = new MatrixViewDialog(selectedMatrix, pos, this);

    connect(matrixViewDialog, SIGNAL(matrixRemoved(int)), SLOT(removeMatrix(int)));
    connect(matrixViewDialog, SIGNAL(matrixChanged(Matrix<int>&, int)),
            SLOT(changeMatrix(Matrix<int>&, int)));

    matrixViewDialog->setModal(true);
    matrixViewDialog->show();
}

void FrontView::changeMatrix(Matrix<int>& m, int pos) {
    listModel->getData()[pos] = m;

    updateTotalLabel();

    auto indexBegin = listModel->index(0, 0);
    auto indexEnd = listModel->index(listModel->rowCount()-1, 0);
    emit listModel->dataChanged(indexBegin, indexEnd);
}

void FrontView::updateTotalLabel() {
    using std::to_string;
    auto newStr = "Total: " + to_string(listModel->rowCount());
    ui->totalLabel->setText(QString::fromStdString(newStr));
}

void FrontView::removeMatrix(int pos) {
    listModel->getData().erase(listModel->getData().begin() + pos);

    updateTotalLabel();

    auto indexBegin = listModel->index(0, 0);
    auto indexEnd = listModel->index(listModel->rowCount()-1, 0);
    emit listModel->dataChanged(indexBegin, indexEnd);
}

void FrontView::findBtnClicked() {
    if (listModel->rowCount() == 0) {
        auto text = QString("There are no matrixes. Add someone.");
        QMessageBox::information(this, "", text);
        return;
    }

    auto findMatrixDialog = new FindMatrixDialog(listModel, this);

    connect(findMatrixDialog, SIGNAL(matrixRemoved(int)), SLOT(removeMatrix(int)));
    connect(findMatrixDialog, SIGNAL(matrixChanged(Matrix<int>&, int)),
            SLOT(changeMatrix(Matrix<int>&,int)));

    findMatrixDialog->setModal(true);
    findMatrixDialog->show();
}

