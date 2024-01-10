#ifndef TABLE_PORTFOLIOS_CUSTOM_H
#define TABLE_PORTFOLIOS_CUSTOM_H

#include <QObject>
#include "QTableView"
#include "QWidget"
#include "QKeyEvent"
#include "defines.h"

class table_portfolios_custom :  public QTableView
{
    Q_OBJECT
public:
    explicit table_portfolios_custom(QWidget *parent = Q_NULLPTR);

private:
    int findNextEditableCell(int currentColIdx,nav_direction direction);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;


public slots:
    void handleTabKeyPressFromEditableCell(nav_direction direction);

signals:
    void spaceKeySignal();
};

#endif // TABLE_PORTFOLIOS_CUSTOM_H
