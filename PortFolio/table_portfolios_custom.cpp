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
    if(event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab){

        nav_direction direction = nav_direction::nav_forward;
        if( event->key() == Qt::Key_Backtab)
            direction =  nav_direction::nav_backward;

        int nextIDx= findNextEditableCell(currentIndex().column(),direction);
        this->setCurrentIndex(model()->index(currentIndex().row(), nextIDx));
        this->edit(currentIndex());
        return;

    }
    else if(event->key() == Qt::Key_Space){
        //do not send  spaceKeySignal if current index is _Status, because the update signal already send by checkbox itself

        if(currentIndex().column()!=PortfolioData_Idx::_Status)
            emit spaceKeySignal();
    }
    /* else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
            {
                int currentColIdx = currentIndex().column();
                QList<int> editableIDx = {
                    PortfolioData_Idx::_SellPriceDifference,
                    PortfolioData_Idx::_BuyPriceDifference,
                    PortfolioData_Idx::_SellTotalQuantity,
                    PortfolioData_Idx::_BuyTotalQuantity,
                    PortfolioData_Idx::_OrderQuantity
                }; // these are the editable table cell indexes in the algo table

                if (editableIDx.contains(currentColIdx))
                {
                    int increment = (event->key() == Qt::Key_Right) ? 1 : -1;
                    int currentValue = model()->data(currentIndex()).toInt();
                    model()->setData(currentIndex(), currentValue + increment);
                }

                return;
            }*/


    QTableView::keyPressEvent(event);
}
/*
int table_portfolios_custom::findNextEditableCell(int currentColIdx,nav_direction direction){

  QList<int> editableIDx = {PortfolioData_Idx::_SellPriceDifference,
                            PortfolioData_Idx::_BuyPriceDifference,
                            PortfolioData_Idx::_SellTotalQuantity,
                            PortfolioData_Idx::_BuyTotalQuantity,
                           }; // these are the editable table cell in algo table
  std::sort(editableIDx.begin(), editableIDx.end());



  int idx=currentColIdx;
  if(currentColIdx<editableIDx[0])
      idx = editableIDx[0];
  else if(currentColIdx>=editableIDx[editableIDx.size()-1])
      idx = editableIDx[0];
  else{
      while(1){
          idx++;
          if( editableIDx.contains(idx))
              break;

      }
      for(int i=0;i<editableIDx.size();i++){
          if(currentColIdx==editableIDx[i]&&i<editableIDx.size()){
              qDebug()<<"loop.....currentColIdx="<<currentColIdx<<"  Next idx="<<editableIDx[i];

              idx=editableIDx[i+1];
              break;
          }

      }
  }


  qDebug()<<"currentColIdx="<<currentColIdx<<"  Next idx="<<idx;


  return idx;

}*/

int table_portfolios_custom::findNextEditableCell(int currentColIdx, nav_direction direction) {

    QList<int> editableIDx = { PortfolioData_Idx::_SellPriceDifference,
                              PortfolioData_Idx::_BuyPriceDifference,
                              PortfolioData_Idx::_SellTotalQuantity,
                              PortfolioData_Idx::_BuyTotalQuantity,
                              PortfolioData_Idx::_Status,
                              }; // These are the editable table cells in algo table
    std::sort(editableIDx.begin(), editableIDx.end());

    int idx = currentColIdx;

    if (direction == nav_direction::nav_forward) {
        if (currentColIdx < editableIDx[0])
            idx = editableIDx[0];
        else if (currentColIdx >= editableIDx[editableIDx.size() - 1])
            idx = editableIDx[0];
        else {
            while (1) {
                idx++;
                if (editableIDx.contains(idx))
                    break;
            }
        }
    } else if (direction == nav_direction::nav_backward) {
        if (currentColIdx > editableIDx[editableIDx.size() - 1])
            idx = editableIDx[editableIDx.size() - 1];
        else if (currentColIdx <= editableIDx[0])
            idx = editableIDx[editableIDx.size() - 1];
        else {
            while (1) {
                idx--;
                if (editableIDx.contains(idx))
                    break;
            }
        }
    }

    qDebug() << "currentColIdx=" << currentColIdx << "  Next idx=" << idx;

    return idx;
}

