#include "table_portfolios_custom.h"
#include "qpainter.h"

#include <algorithm>


table_portfolios_custom::table_portfolios_custom(QWidget *parent)
    : QTableView{parent}
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers |QAbstractItemView::DoubleClicked);
    viewport()->installEventFilter(this);

    setMouseTracking(true);
}

void table_portfolios_custom::handleTabKeyPressFromEditableCell(nav_direction direction){

    int nextIDx = findNextEditableCell(currentIndex().column(),direction);
    this->setCurrentIndex(model()->index(currentIndex().row(), nextIDx));
    this->edit(currentIndex());

    return;
}

void table_portfolios_custom::keyPressEvent(QKeyEvent *event)
{
    QModelIndex currentIdx = currentIndex();
    int currentRow = currentIdx.row();
    int currentColumn = currentIdx.column();
    int nextRow = currentRow;
    int nextColumn = currentColumn;

    if (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab)
    {
        nav_direction direction = nav_direction::nav_forward;
        if (event->key() == Qt::Key_Backtab)
            direction = nav_direction::nav_backward;

        nextColumn = findNextEditableCell(currentColumn, direction);
    }
    else if (event->key() == Qt::Key_Down)
    {
        nextRow = currentRow + 1;
        if (nextRow < model()->rowCount())
        {
            // make sure that  next cell in the same column is editable
            if (!(model()->flags(model()->index(nextRow, currentColumn)) & Qt::ItemIsEditable))
            {
                nextColumn = findNextEditableCell(currentColumn, nav_direction::nav_forward);
            }
        }
        else
        {
            nextRow = currentRow;  // Stay same if it's the last row
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        nextRow = currentRow - 1;
        if (nextRow >= 0)
        {
            // Ensure the previous cell in the same column is editable
            if (!(model()->flags(model()->index(nextRow, currentColumn)) & Qt::ItemIsEditable))
            {
                nextColumn = findNextEditableCell(currentColumn, nav_direction::nav_backward);
            }
        }
        else
        {
            nextRow = currentRow;  // Stay in the current row if it's the first row
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
        nextColumn = findNextEditableCell(currentColumn, nav_direction::nav_forward);
    }
    else if (event->key() == Qt::Key_Left)
    {
        nextColumn = findNextEditableCell(currentColumn, nav_direction::nav_backward);
    }
    else
    {
        QTableView::keyPressEvent(event); // Call the base class implementation for other keys
        return;
    }

    if (nextRow != currentRow || nextColumn != currentColumn)
    {
        this->setCurrentIndex(model()->index(nextRow, nextColumn));
        this->scrollTo(currentIndex(), QAbstractItemView::PositionAtCenter);
        this->edit(currentIndex());
    }
}

void table_portfolios_custom::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);

    QModelIndex currentIdx = indexAt(event->pos());
    if (model()->flags(currentIdx) & Qt::ItemIsEditable)
    {
        this->setCurrentIndex(currentIdx);
        this->scrollTo(currentIdx, QAbstractItemView::PositionAtCenter);
        this->edit(currentIdx);
    }
}

int table_portfolios_custom::findNextEditableCell(int currentColIdx, nav_direction direction)
{
    QList<int> editableIDx = {
        PortfolioData_Idx::_SellPriceDifference,
        PortfolioData_Idx::_BuyPriceDifference,
        PortfolioData_Idx::_SellTotalQuantity,
        PortfolioData_Idx::_BuyTotalQuantity,
        PortfolioData_Idx::_Status,
        PortfolioData_Idx::_OrderQuantity,
    }; // These are the editable table cells in algo table
    std::sort(editableIDx.begin(), editableIDx.end());

    int idx = currentColIdx;

    if (direction == nav_direction::nav_forward)
    {
        if (currentColIdx < editableIDx[0])
            idx = editableIDx[0];
        else if (currentColIdx >= editableIDx[editableIDx.size() - 1])
            idx = editableIDx[0];
        else
        {
            while (1)
            {
                idx++;
                if (editableIDx.contains(idx))
                    break;
            }
        }
    }
    else if (direction == nav_direction::nav_backward)
    {
        if (currentColIdx > editableIDx[editableIDx.size() - 1])
            idx = editableIDx[editableIDx.size() - 1];
        else if (currentColIdx <= editableIDx[0])
            idx = editableIDx[editableIDx.size() - 1];
        else
        {
            while (1)
            {
                idx--;
                if (editableIDx.contains(idx))
                    break;
            }
        }
    }

    qDebug() << "currentColIdx=" << currentColIdx << "  Next idx=" << idx;

    return idx;
}

void table_portfolios_custom::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    // Call the base class implementation (important)
    QTableView::selectionChanged(selected, deselected);

    // Implement your custom logic here
   /* qDebug() << "Selection changed";
    qDebug() << "Selected:" << selected.indexes();
    qDebug() << "Deselected:" << deselected.indexes();*/
    int currentRow = -1;
    //int prevRow = -1;
    QModelIndexList indexes = selected.indexes();
    if (!indexes.isEmpty())        currentRow= indexes.last().row();

    emit selectionChangedSignal(currentRow);
}

//void table_portfolios_custom::focusOutEvent(QFocusEvent *event) {
//    QTableView::focusOutEvent(event);
//    clearSelection();
//}



