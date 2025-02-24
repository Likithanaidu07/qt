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
#include <QElapsedTimer>

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
    QVariant headerData(int section, Qt::Orientation , int role)const override;
    QList <PortfolioObject*> portfolio_data_list;
    QList<int>  portfolio_tokens; //leg1,leg2,leg3,leg4,leg5,leg6 and fut
   // QHash<QString,PortFolioData_Less> getPortFolioDataLess();
   // QHash<QString,int> getPortFoliosLotSize();
    QHash<int,Cell_Cache> editingDataHash; //data to store editing cell
    //void clearTable();
    void setEditingFlg(int row,int val);
    void updatePortFolioStatusValue(int row,QString statusVal);
    QStringList getAllPortfolioNumbers();
    QStringList getActivatedPortfolios();
    void updateNewleyTradeHilgiht(QStringList newleyTraderAlgos);

    PortfolioObject * getPortFolioAt(int idx) const ;

    mutable  QMutex mutex; // Define a QMutex to protect shared data
    void refreshTable();
    int switchstatetest() const;

   // void setColumnWidths(QTableView *tableView) const;
    QStringList getTradedHighlight_ExcludeList();
    QStringList  TradedHighlight_ExcludeList;
    void updateMarketRate(const QHash<QString, MBP_Data_Struct>& data);
    bool removeRowsByIndices(const QList<int> &portFolioIdxToDelete);

    QStringList getHeaders();
    QList<portfolioImportExportData> getPortFolioToExport();
private:
   // QStringList header={"Status","Algo ID","Algo Name","Rate", "Avg", "Diff" ,"TQ", "TTQ", "RQ", "Rate", "Avg", "Diff", "TQ", "TTQ", "RQ", "Odr Qty" ,"Qty Ratio", "Skip/Market Strike", "Bid Leg", "Cost", "Fut Price"};
    QStringList header={"Status","Algo No",                         
                          "Algo Name","Alias","BRate",
                          "BAvg","BDiff","BTQ",
                          "BTTQ","BRQ","SRate",
                          "SAvg", "SDiff","STQ",
                          "STTQ","SRQ","Odr Qty","Max Loss","Depth","Expiry 1", /*"Expiry 2",*/
                          "Cost" ,"Qty Ratio",
                          /*"Bid Leg",*/ "Fut Price"};



    QString double_to_Human_Readable(double num, int precision) const;
    double devicer;
    int decimal_precision;
    bool storeToken(PortfolioObject* p);
    void loadSettings();
    int current_editingRow; //row which is editing currently

    QElapsedTimer slowDataPriceUpdateTimer;



public slots:
    void onItemChanged(const QModelIndex &index);
    void selectionChangedSlot(int currentIdx);
    void updateModelDataList(QList <PortfolioObject*> portfolio_data_list,bool clearTableFlg);
    void editCancelled(int row);

signals:
    void editCompleted(QString text, QModelIndex idx) const;
    void  edit_Started(int row,int col) const;
   // void resizePortFolioTableColWidth(int colWidth);
    void updateDBOnDataChanged(QModelIndex index);
};

#endif // TABLE_PORTFOLIOS_MODEL_H
