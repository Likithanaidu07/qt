#include "table_portfolios_custom.h"
#include "qheaderview.h"
#include "qpainter.h"
#include <QHeaderView>

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
    /*else if (event->key() == Qt::Key_Delete)
    {
        emit portFolioDeleteKeyPressed();
    }*/
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

void table_portfolios_custom::reorderEditableIndicesBasedOnVisualIndex( QList<int>& editableIDx) {
    QList<QPair<int, int>> indexPairs;

    // Map each logical index in 'editableIDx' to its visual index
    for (int logicalIndex : editableIDx) {
        int visualIndex = this->horizontalHeader()->visualIndex(logicalIndex);
        indexPairs.append(qMakePair(visualIndex, logicalIndex));
    }

    // Sort the list based on visual indices
    std::sort(indexPairs.begin(), indexPairs.end(), [](const QPair<int, int>& a, const QPair<int, int>& b) {
        return a.first < b.first;
    });

    // Update 'editableIDx' with the reordered logical indices
    editableIDx.clear();
    for (const QPair<int, int>& pair : indexPairs) {
        editableIDx.append(pair.second);
    }
}

//int table_portfolios_custom::findNextEditableCell(int currentColIdx, nav_direction direction)
//{
//    QList<int> editableIDx = {
//        PortfolioData_Idx::_Alias,
//        PortfolioData_Idx::_SellPriceDifference,
//        PortfolioData_Idx::_BuyPriceDifference,
//        PortfolioData_Idx::_SellTotalQuantity,
//        PortfolioData_Idx::_BuyTotalQuantity,
//        PortfolioData_Idx::_OrderQuantity,
//        PortfolioData_Idx::_MaxLoss,

//    }; // These are the editable table cells in algo table
//    std::sort(editableIDx.begin(), editableIDx.end());
//    qDebug()<<"before sort"<<editableIDx;

//   // reorderEditableIndicesBasedOnVisualIndex(editableIDx);
//    qDebug()<<"after sort"<<editableIDx;

//    int idx = currentColIdx;

//    if (direction == nav_direction::nav_forward)
//    {
//        if (currentColIdx < editableIDx[0])
//            idx = editableIDx[0];
//        else if (currentColIdx >= editableIDx[editableIDx.size() - 1])
//            idx = editableIDx[0];
//        else
//        {
//            while (1)
//            {
//                idx++;
//                if (editableIDx.contains(idx))
//                    break;
//            }
//        }
//    }
//    else if (direction == nav_direction::nav_backward)
//    {
//        if (currentColIdx > editableIDx[editableIDx.size() - 1])
//            idx = editableIDx[editableIDx.size() - 1];
//        else if (currentColIdx <= editableIDx[0])
//            idx = editableIDx[editableIDx.size() - 1];
//        else
//        {
//            while (1)
//            {
//                idx--;
//                if (editableIDx.contains(idx))
//                    break;
//            }
//        }
//    }

//    qDebug() << "currentColIdx=" << currentColIdx << "  Next idx=" << idx;

//    return idx;
//}
int table_portfolios_custom::findNextEditableCell(int currentColIdx, nav_direction direction)
{
    QList<int> editableIDx = {
        PortfolioData_Idx::_Alias,
        PortfolioData_Idx::_SellPriceDifference,
        PortfolioData_Idx::_BuyPriceDifference,
        PortfolioData_Idx::_SellTotalQuantity,
        PortfolioData_Idx::_BuyTotalQuantity,
        PortfolioData_Idx::_OrderQuantity,
        PortfolioData_Idx::_MaxLoss,
        PortfolioData_Idx::_Depth,
    };

    QHeaderView *headerView = this->horizontalHeader();
    if (!headerView) {
        qDebug() << "Header view is not accessible!";
        return -1;
    }

    // Map logical indices to visual indices
    QList<QPair<int, int>> visualToLogical;
    for (int logicalIdx : editableIDx) {
        int visualIdx = headerView->visualIndex(logicalIdx);
        visualToLogical.append({visualIdx, logicalIdx});
    }

    // Sort by visual index
    std::sort(visualToLogical.begin(), visualToLogical.end(),
              [](const QPair<int, int> &a, const QPair<int, int> &b) {
                  return a.first < b.first;
              });

    // Extract sorted logical indices based on visual order
    QList<int> sortedEditableByVisual;
    for (const auto &pair : visualToLogical) {
        sortedEditableByVisual.append(pair.second);
    }

    //qDebug() << "Logical indices sorted by visual order:" << sortedEditableByVisual;

    // Find the visual index of the current column
    int currentVisualIdx = headerView->visualIndex(currentColIdx);
    int idx = currentColIdx;

    if (direction == nav_direction::nav_forward) {
        for (int logicalIdx : sortedEditableByVisual) {
            if (headerView->visualIndex(logicalIdx) > currentVisualIdx) {
                idx = logicalIdx;
                break;
            }
        }
        // Wrap around to the first cell if no suitable one is found
        if (idx == currentColIdx) {
            idx = sortedEditableByVisual[0];
        }
    } else if (direction == nav_direction::nav_backward) {
        for (int i = sortedEditableByVisual.size() - 1; i >= 0; --i) {
            int logicalIdx = sortedEditableByVisual[i];
            if (headerView->visualIndex(logicalIdx) < currentVisualIdx) {
                idx = logicalIdx;
                break;
            }
        }
        // Wrap around to the last cell if no suitable one is found
        if (idx == currentColIdx) {
            idx = sortedEditableByVisual[sortedEditableByVisual.size() - 1];
        }
    }

    qDebug() << "currentColIdx=" << currentColIdx << "  Next idx=" << idx;
    return idx;
}

//To disable the automatic scrolling to the current editing cell in a Qt QTableView,
//you can override the scrollTo function in a custom QTableView subclass.
//This way, you can prevent the default behavior of scrolling to the current editing cell.
void table_portfolios_custom::scrollTo(const QModelIndex &index, ScrollHint hint) {
        // Do nothing to prevent scrolling to the current editing cell
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



