#ifndef FRONTVIEW_H
#define FRONTVIEW_H

#include <QtWidgets>
#include "MatrixListModel.hpp"
#include "PositionEnum.hpp"

namespace Ui {
    class Widget;
}

class FrontView : public QWidget {
    Q_OBJECT

public:
    explicit FrontView(QWidget *parent = nullptr);
    ~FrontView();

private slots:
    void addButtonClicked();
    void addToTopButtonClicked();
    void findBtnClicked();
    void matrixItemDblClicked(QModelIndex);

    void addMatrix(Position, std::shared_ptr<Matrix<int>>&);
    void removeMatrix(int);

private:
    MatrixModel *listModel;
    Ui::Widget *ui;

    void updateTotalLabel();
};

#endif // FRONTVIEW_H
