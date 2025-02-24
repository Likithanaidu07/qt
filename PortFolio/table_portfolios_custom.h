#ifndef TABLE_PORTFOLIOS_CUSTOM_H
#define TABLE_PORTFOLIOS_CUSTOM_H

#include <QObject>
#include "PortFolio/table_portfolios_model.h"
#include "QTableView"
#include "QWidget"
#include "QKeyEvent"
#include "defines.h"

class table_portfolios_custom :  public QTableView
{
    Q_OBJECT
public:
    explicit table_portfolios_custom(QWidget *parent = Q_NULLPTR);


    /*void resizeEvent(QResizeEvent *event) override
    {
        QTableView::resizeEvent(event);
        if (model())
        {
            static_cast<Table_Portfolios_Model*>(model())->setColumnWidths(this);
        }
    }*/


private:
    int findNextEditableCell(int currentColIdx,nav_direction direction);
    void reorderEditableIndicesBasedOnVisualIndex( QList<int>& editableIDx);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    //void focusOutEvent(QFocusEvent *event) override;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;

public slots:
    void handleTabKeyPressFromEditableCell(nav_direction direction);
    void mousePressEvent(QMouseEvent *event);


signals:
    //void spaceKeySignal();
    void selectionChangedSignal(int currentRow);
  //  void portFolioDeleteKeyPressed();

};

#endif // TABLE_PORTFOLIOS_CUSTOM_H
