#include <QtWidgets>
#include <QDebug>

#include "FrontView.hpp"
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
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

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

    connect(addMatrixDialog, SIGNAL(addedNewMatrix(Position, std::shared_ptr<Matrix<int>>&)),
            SLOT(addMatrix(Position, std::shared_ptr<Matrix<int>>&)));

    addMatrixDialog->setModal(true);
    addMatrixDialog->show();
}

void FrontView::addToTopButtonClicked() {
    auto addMatrixDialog = new AddMatrixDialog(this, Position::Top);

    connect(addMatrixDialog, SIGNAL(addedNewMatrix(Position, std::shared_ptr<Matrix<int>>&)),
             SLOT(addMatrix(Position, std::shared_ptr<Matrix<int>>&)));

    addMatrixDialog->setModal(true);
    addMatrixDialog->show();
}

void FrontView::addMatrix(Position pos, std::shared_ptr<Matrix<int>>& matrix) {
    if (pos == Position::Bottom)
        listModel->getData().push_back(matrix);
    else if (pos == Position::Top)
        listModel->getData().push_front(matrix);

    updateTotalLabel();

    auto i = (pos == Position::Top) ? 0 : listModel->rowCount() - 1;
    auto indexBegin = listModel->index(i, 0);
    auto indexEnd = listModel->index(i, 0);
    emit listModel->dataChanged(indexBegin, indexEnd);
}

void FrontView::matrixItemDblClicked(QModelIndex index) {
    auto pos = index.row();
    auto selectedMatrix = listModel->getData()[pos];
    auto matrixViewDialog = new MatrixViewDialog(selectedMatrix, pos, this);

    connect(matrixViewDialog, SIGNAL(matrixRemoved(int)), SLOT(removeMatrix(int)));

    matrixViewDialog->setModal(true);
    matrixViewDialog->show();
}

void FrontView::findBtnClicked() {
    if (listModel->rowCount() == 0) {
        auto text = QString("There are no matrixes. Add someone.");
        QMessageBox::information(this, "", text);
        return;
    }

    auto findMatrixDialog = new FindMatrixDialog(listModel, this);

    connect(findMatrixDialog, SIGNAL(matrixRemoved(int)), SLOT(removeMatrix(int)));

    findMatrixDialog->setModal(true);
    findMatrixDialog->show();
}

void FrontView::updateTotalLabel() {
    using std::to_string;
    auto newStr = "Total: " + to_string(listModel->rowCount());
    ui->totalLabel->setText(QString::fromStdString(newStr));
}

void FrontView::removeMatrix(int pos) {
    listModel->getData().erase(listModel->getData().begin() + pos);

    updateTotalLabel();

    auto indexBegin = listModel->index(pos, 0);
    auto indexEnd = listModel->index(pos, 0);
    emit listModel->dataChanged(indexBegin, indexEnd);
}

void FrontView::keyPressEvent(QKeyEvent *event) {
    auto selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
    auto len = selectedIndexes.length();


    if (event->key() == Qt::Key_Backspace && len > 0) {
        QString request = (len == 1) ? "Do you really want to remove this matrix?" :
            (len > 1)  ? QString("Do you really want to remove %1 matrixes?").arg(len) : "";

        auto reply = QMessageBox::question(this, "", request);

        if (reply == QMessageBox::Yes) {
            foreach(const QModelIndex &index, selectedIndexes){
                removeMatrix(index.row());
            }
            ui->listView->selectionModel()->clear();
        }
    }
}
