#ifndef TABLE_PORTFOLIOS_MODEL_H
#define TABLE_PORTFOLIOS_MODEL_H
#include <QAbstractTableModel>
#include <QObject>
#include "defines.h"
#include "portfolio_Object.h"
#include "QRegularExpression"
#include "QMutex"
#include "qpixmap.h"
#include "qtableview.h"

class Table_Portfolios_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    Table_Portfolios_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <PortfolioObject*> portfolio_data_list);
    QVariant headerData(int section, Qt::Orientation , int role)const override;
    QList <PortfolioObject*> portfolio_data_list;
    QList<int>  portfolio_tokens; //leg1,leg2,leg3,leg4,leg5,leg6
    QHash<QString,PortFolioData_Less> getPortFolioDataLess();
    QMutex mutex; // Define a QMutex to protect shared data
    void refreshTable();
    int switchstatetest() const;

    void setColumnWidths(QTableView *tableView) const;


private:
   // QStringList header={"Status","Algo ID","Algo Name","Rate", "Avg", "Diff" ,"TQ", "TTQ", "RQ", "Rate", "Avg", "Diff", "TQ", "TTQ", "RQ", "Odr Qty" ,"Qty Ratio", "Skip/Market Strike", "Bid Leg", "Cost", "Fut Price"};
    QStringList header={"Status","Algo No.",
                          " Algo Name","Rate",
                          "Avg","Diff","TQ",
                          "TTQ","RQ","Rate",
                          "Avg", "Diff","TQ",
                          "TTQ","RQ","Expiry 1", /*"Expiry 2",*/
                          "Cost","Odr Qty" ,"Qty Ratio",
                          "Skip/Market Strike", "Bid Leg", "Fut Price"};


    QString double_to_Human_Readable(double num, int precision) const;
    double devicer;
    int decimal_precision;
    bool storeToken(PortfolioObject* p);
    void loadSettings();

public slots:
    void onItemChanged(const QModelIndex &index);

signals:
    void editCompleted(QString text, QModelIndex idx) const;
    void  edit_Started(int row,int col) const;
    void resizePortFolioTableColWidth(int colWidth);
    void updateDBOnDataChanged(QModelIndex index);
};

#endif // TABLE_PORTFOLIOS_MODEL_H
