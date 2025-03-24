#include "table_portfolios_delegate.h"
#include "table_portfolios_custom.h"
#include <QLineEdit>
#include "QApplication"
//#include "QCheckBox"
#include "table_portfolios_model.h"
#include "SwitchButton/switchbutton.h"
#include "QPainter"
#include "defines.h"
#include "QSettings"
#include "QDir"
#include <QValidator>
#include <QStandardPaths>
#include <QCheckBox>
#include "qsortfilterproxymodel.h"
#include "portfolio_searchfilterproxymodel.h"

Table_Portfolios_Delegate::Table_Portfolios_Delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
        settings.endGroup();
    }

    quantity_incrementer= 1;
    price_diff_incrementer = 0.01;
    if(market_type=="fo"){
        price_diff_incrementer = FO_PRICE_DIFF_INCREMENTER;
    }
    else{
        price_diff_incrementer = CDS_PRICE_DIFF_INCREMENTER;
    }

}

void Table_Portfolios_Delegate::setHighlightText(const QString &text) {
    // Implement logic to store and handle highlight text
    m_highlightText = text;
}

bool Table_Portfolios_Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease && index.column() == PortfolioData_Idx::_Status)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QRect imageRect = option.rect;
        imageRect.setWidth(option.rect.width());
        imageRect.setHeight(option.rect.height());
        int topPadding = 2;
        int padding = 10;
        imageRect.adjust(padding, topPadding, -padding, padding);

        // Check if the mouse press occurred within the image rectangle
        if (imageRect.contains(mouseEvent->pos()))
        {
            // Handle the click on the image
           // qDebug() << "Image Clicked!";
           // qDebug() << "Model type:" << index.model()->metaObject()->className();
          //  qDebug() << "Column:" << index.column() << "Row:" << index.row();
            //model->setData(index, index.data(Qt::ItemIsUserCheckable).value<Qt::CheckState>(), Qt::ItemIsUserCheckable);
            const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel*>(index.model());

            if (proxyModel) {
                if (!index.isValid()) {
                    qDebug() << "Invalid index passed!";
                    return true;
                }

                QModelIndex sourceIndex = proxyModel->mapToSource(index);

                if (!sourceIndex.isValid()) {
                    qDebug() << "Invalid source index after mapping!";
                    return true;
                }

                proxyModel->sourceModel()->setData(sourceIndex, sourceIndex.data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::CheckStateRole);
            }
            else {
                if (!index.isValid()) {
                    qDebug() << "Invalid index passed to non-proxy model!";
                    return true;
                }

                model->setData(index, index.data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::CheckStateRole);
            }




           // bool checked = index.data(Qt::CheckStateRole).toBool();
           // model->setData(sourceIndex, !checked, Qt::CheckStateRole);
           // model->setData(sourceIndex, sourceIndex.data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::ItemIsUserCheckable);
            return true;

            // If you need to stop further processing of the event, you can use event->accept();
            // event->accept();
        }
        return true;  // Consume the event
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget *Table_Portfolios_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
   // QFont font("Work Sans");
    int c = index.column();
    if ( c == PortfolioData_Idx::_OrderQuantity)
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new IntDiffValidator(1, 1000000000, editor));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
        return editor;
    }
    else if( c == PortfolioData_Idx::_Alias ){
        QLineEdit *editor = new QLineEdit(parent);
        //  editor->setValidator(new QIntValidator(0, 9999));
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));
        return editor;
    }
    else if( c == PortfolioData_Idx::_SellTotalQuantity ||
             c == PortfolioData_Idx::_BuyTotalQuantity ){
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(0, 9999));
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));
        return editor;
    }
    else if( c == PortfolioData_Idx::_Depth ){
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new IntDiffValidator(1, 5,editor));
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));
        return editor;
    }
    else if(c == PortfolioData_Idx::_SellPriceDifference ||
             c == PortfolioData_Idx::_BuyPriceDifference ){
        QLineEdit *editor = new QLineEdit(parent);
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
        if(market_type=="fo")
            editor->setValidator(new DiffValidator(-9999.99, 9999.99, FO_DECIMAL_PRECISION,false, editor));
        else
            editor->setValidator(new DiffValidator(-9.9995, 9.9995, CDS_DECIMAL_PRECISION,true, editor));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));

        return editor;
    }
    else if(c == PortfolioData_Idx::_MaxLoss){
      QLineEdit *editor = new QLineEdit(parent);
        editor->setObjectName("portfolioTableCell_"+QString::number(c));
      if(market_type=="fo")
          editor->setValidator(new DiffValidator(0, 9999.99, FO_DECIMAL_PRECISION,false, editor));
      else
          editor->setValidator(new DiffValidator(0, 9.9995, CDS_DECIMAL_PRECISION,true, editor));
      editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));

      return editor;
  }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void Table_Portfolios_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int c= index.column();

    if(c==PortfolioData_Idx::_SellPriceDifference ||
        c==PortfolioData_Idx::_BuyPriceDifference ||
        c==PortfolioData_Idx::_SellTotalQuantity ||
        c==PortfolioData_Idx::_BuyTotalQuantity ||
        c==PortfolioData_Idx::_OrderQuantity ||
        c==PortfolioData_Idx::_Alias||
        c==PortfolioData_Idx::_MaxLoss||
        c==PortfolioData_Idx::_Depth)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        // qDebug()<<"Table_Portfolios_Delegate::setEditorData: "<<value;

        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        {
            lineEdit->setText(value);
            currentIndex = index;
            lineEdit->setAlignment(Qt::AlignCenter);
        }
        else
        {
            QStyledItemDelegate::setEditorData(editor, index);
        }

    }

}

/*
void Table_Portfolios_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const
{
    int c= index.column();

    if(c==PortfolioData_Idx::_SellPriceDifference ||
        c==PortfolioData_Idx::_BuyPriceDifference ||
        c==PortfolioData_Idx::_SellTotalQuantity ||
        c==PortfolioData_Idx::_BuyTotalQuantity ||
        c==PortfolioData_Idx::_OrderQuantity
        )
    {
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        {
            bool ok;
            QString str = lineEdit->text();
            //qDebug()<<"Table_Portfolios_Delegate::setModelData: "<<str;
            double value = str.toDouble(&ok);
            if (ok)
            {
                // Use const_cast here to remove the const qualifier from the model
                mutableModel = const_cast<QAbstractItemModel *>(model);
                mutableModel->setData(index, value, Qt::EditRole);
                mutableModel = nullptr;
                lineEdit->setAlignment(Qt::AlignCenter);

            }
        }

        else
        {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }
    else if(c==PortfolioData_Idx::_Alias){
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        {
            bool ok;
            QString str = lineEdit->text();
            // Use const_cast here to remove the const qualifier from the model
            mutableModel = const_cast<QAbstractItemModel *>(model);
            mutableModel->setData(index, str, Qt::EditRole);
            mutableModel = nullptr;
            lineEdit->setAlignment(Qt::AlignCenter);
        }
        else
        {
            QStyledItemDelegate::setModelData(editor, model, index);
        }

    }

}*/
void Table_Portfolios_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel *>(model);
    QModelIndex sourceIndex = index;

    // If the model is a proxy model, map to the source index
    if (proxyModel) {
        sourceIndex = proxyModel->mapToSource(index);
        model = const_cast<QAbstractItemModel *>(proxyModel->sourceModel());
    }

    int c = sourceIndex.column();

    // Process numerical columns
    if (c == PortfolioData_Idx::_SellPriceDifference ||
        c == PortfolioData_Idx::_BuyPriceDifference ||
        c == PortfolioData_Idx::_SellTotalQuantity ||
        c == PortfolioData_Idx::_BuyTotalQuantity ||
        c == PortfolioData_Idx::_OrderQuantity ||
        c == PortfolioData_Idx::_MaxLoss||
        c == PortfolioData_Idx::_Depth)
    {
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor)) {
            bool ok;
            QString str = lineEdit->text();
            double value = str.toDouble(&ok);

            if (ok) {
                model->setData(sourceIndex, value, Qt::EditRole);
                lineEdit->setAlignment(Qt::AlignCenter);
            }
        } else {
            QStyledItemDelegate::setModelData(editor, model, sourceIndex);
        }
    }
    // Process alias column
    else if (c == PortfolioData_Idx::_Alias) {
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor)) {
            QString str = lineEdit->text();
            model->setData(sourceIndex, str, Qt::EditRole);
            lineEdit->setAlignment(Qt::AlignCenter);
        } else {
            QStyledItemDelegate::setModelData(editor, model, sourceIndex);
        }
    }
}

void Table_Portfolios_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,  const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


bool Table_Portfolios_Delegate::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        int currentColIdx = currentIndex.column();
        QList<int> editableIDx = {
            PortfolioData_Idx::_SellPriceDifference,
            PortfolioData_Idx::_BuyPriceDifference,
            PortfolioData_Idx::_SellTotalQuantity,
            PortfolioData_Idx::_BuyTotalQuantity,
            PortfolioData_Idx::_OrderQuantity,
            PortfolioData_Idx::_MaxLoss,
            PortfolioData_Idx::_Depth,
        };

        if (editableIDx.contains(currentColIdx)) {
            double incStep = quantity_incrementer;
            if (currentColIdx == PortfolioData_Idx::_SellPriceDifference ||
                currentColIdx == PortfolioData_Idx::_BuyPriceDifference ||
                currentColIdx == PortfolioData_Idx::_MaxLoss) {
                incStep = price_diff_incrementer;
            }

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right) {
                if (currentIndex.isValid()) {
                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj)) {
                        QString value = lineEdit->text();
                        bool ok;
                        double incValue = value.toDouble(&ok);

                        if (ok) {
                            //double incrementedValue = incValue;

                            if (keyEvent->key() == Qt::Key_Right) {
                                // Increase value by incStep
                                incValue += incStep;
                            } else if (keyEvent->key() == Qt::Key_Left) {
                                // Decrease value by incStep
                                incValue -= incStep;
                            }

                            // Skip rounding for price differences and MaxLoss
                            if (currentColIdx != PortfolioData_Idx::_SellPriceDifference &&
                                currentColIdx != PortfolioData_Idx::_BuyPriceDifference &&
                                currentColIdx != PortfolioData_Idx::_MaxLoss &&
                                currentColIdx == PortfolioData_Idx::_Depth) {
                                // Round to nearest 0.05 (or appropriate precision)
                                incValue = std::round(incValue * 20.0) / 20.0;
                            }

                            // Ensure certain values do not go below their respective thresholds
                            if (currentColIdx == PortfolioData_Idx::_BuyTotalQuantity ||
                                currentColIdx == PortfolioData_Idx::_SellTotalQuantity ||
                                currentColIdx == PortfolioData_Idx::_MaxLoss ) {
                                if (incValue < 0) incValue = 0;
                            }

                            if(currentIndex.column() == PortfolioData_Idx::_OrderQuantity||
                                currentColIdx == PortfolioData_Idx::_Depth){
                                if(incValue < 1)
                                    incValue = 1;
                            }
                            value = QString::number(incValue);
                            lineEdit->setText(value);
                            //cursor
                            if ( keyEvent->key() == Qt::Key_Left) {
                                lineEdit->setCursorPosition(-1); // Set cursor position outside visible range
                            }
                            lineEdit->setAlignment(Qt::AlignCenter);
                        }
                    }
                }
            }
             else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
                if (currentIndex.isValid()) {
                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj)) {
                        double currentValue = lineEdit->text().toDouble();
                        // Skip rounding for BuyPriceDifference and SellPriceDifference
                        double newValue = currentValue;
                        if (currentColIdx != PortfolioData_Idx::_SellPriceDifference &&
                            currentColIdx != PortfolioData_Idx::_BuyPriceDifference &&
                            currentColIdx != PortfolioData_Idx::_MaxLoss) {
                            double multiplied = std::round(currentValue * 20.0);
                            newValue = multiplied / 20.0;
                        }
                        const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel *>(currentIndex.model());
                        proxyModel->disableSortingWhileEditing();  //this required or elase tab key event will trigger when search text in cell and edit the same cell and press enter. And cause hang state due to mutex
                        QModelIndex sourceIndex = currentIndex;
                        if (proxyModel) {
                            sourceIndex = proxyModel->mapToSource(currentIndex);
                            mutableModel = const_cast<QAbstractItemModel *>(proxyModel->sourceModel());
                        } else {
                            mutableModel = const_cast<QAbstractItemModel *>(currentIndex.model());
                        }
                        if (mutableModel) {
                            mutableModel->setData(sourceIndex, newValue, Qt::EditRole);
                        } else {
                            qDebug() << "Mutable model is null!";
                        }
                        emit commitData(lineEdit);
                        emit closeEditor(lineEdit);
                        mutableModel = nullptr;
                        emit editFinished(QString::number(currentValue), sourceIndex);
                        proxyModel->enableSortingAfterEditing();
                        return true;
                    }
                }
            } else if (keyEvent->key() == Qt::Key_Backtab) {
                emit tabKeyPressed(nav_direction::nav_backward);
            } else if (keyEvent->key() == Qt::Key_Tab) {
                emit tabKeyPressed(nav_direction::nav_forward);
            }
            else if (keyEvent->key() == Qt::Key_Escape) {
                const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel *>(currentIndex.model());
                QModelIndex sourceIndex = currentIndex;
                if (proxyModel) {
                    sourceIndex = proxyModel->mapToSource(currentIndex);
                }
                emit escapeKeyPressed(sourceIndex.row());
             }

        }
        else if (currentColIdx == PortfolioData_Idx::_Alias) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
                if (currentIndex.isValid()) {
                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj)) {
                        QString currentValue = lineEdit->text();
                        const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel *>(currentIndex.model());
                        proxyModel->disableSortingWhileEditing(); //this required or elase tab key event will trigger when search text in cell and edit the same cell and press enter. And cause hang state due to mutex
                        QModelIndex sourceIndex = currentIndex;
                        if (proxyModel) {
                            sourceIndex = proxyModel->mapToSource(currentIndex);
                            mutableModel = const_cast<QAbstractItemModel *>(proxyModel->sourceModel());
                        } else {
                            mutableModel = const_cast<QAbstractItemModel *>(currentIndex.model());
                        }
                        if (mutableModel) {
                            mutableModel->setData(sourceIndex, currentValue, Qt::EditRole);
                        } else {
                            qDebug() << "Mutable model is null!";
                        }
                        emit commitData(lineEdit);
                        emit closeEditor(lineEdit);
                        mutableModel = nullptr;
                        emit editFinished(currentValue, sourceIndex);
                        proxyModel->enableSortingAfterEditing();
                        return true;
                    }
                }
            }
            else if (keyEvent->key() == Qt::Key_Backtab) {
                emit tabKeyPressed(nav_direction::nav_backward);
            }
            else if (keyEvent->key() == Qt::Key_Tab) {
                emit tabKeyPressed(nav_direction::nav_forward);
            }
            else if (keyEvent->key() == Qt::Key_Escape) {
                const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel *>(currentIndex.model());
                QModelIndex sourceIndex = currentIndex;
                if (proxyModel) {
                    sourceIndex = proxyModel->mapToSource(currentIndex);
                }
                emit escapeKeyPressed(sourceIndex.row());
             }

        }
    }
    return QStyledItemDelegate::eventFilter(obj, event);
}





QSize Table_Portfolios_Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Get the text from the model
    QString text = index.data(Qt::DisplayRole).toString();
    // Calculate the size needed for the cell based on the content
    QFontMetrics fm(option.font);
    QSize size = fm.size(0, text);
    // Optionally, add some padding
    size += QSize(10, 10);
    return size;
}
void Table_Portfolios_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem op(option);
    const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel*>(index.model());
    QModelIndex mappedIndex = proxyModel->mapToSource(index);
    const Table_Portfolios_Model *portfolioModel = qobject_cast<const Table_Portfolios_Model*>(proxyModel->sourceModel());
    PortfolioObject *portfolio = portfolioModel->getPortFolioAt(mappedIndex.row());
    auto c= mappedIndex.column();
    if (!portfolio) {
        return;
    }
    QColor textColor(108, 117, 125);
    QPen pen(textColor);

    pen.setWidthF(0.5);
    painter->setPen(pen);
    QColor HighlightColor("#42A5F5");
    int x_add=1;


    if(c == PortfolioData_Idx::_Status)
    {
        QPixmap checkboxPixmap(10,10);
        checkboxPixmap.fill(Qt::transparent); // Fill with transparent background
        QStyleOptionButton checkboxOption;
        checkboxOption.state |= (mappedIndex.data(Qt::CheckStateRole).toInt() == Qt::Checked) ? QStyle::State_On : QStyle::State_Off;

        checkboxOption.rect = checkboxPixmap.rect(); // Adjust rect for drawing on the pixmap

        // Manually draw the checkbox on the pixmap
        QRect checkboxRect = checkboxOption.rect.adjusted(0, 0, -2, -2);
        QPixmap pixmap(checkboxRect.width(), checkboxRect.height());  // Adjusted pixmap size
        // Draw the pixmap with the updated position
        QRect rect(option.rect.left(), option.rect.top(), checkboxRect.width(), checkboxRect.height());  // Adjusted rect position

        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active)
        {
            QColor color("#FFF");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);

            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);

            // Load an example image (replace with your logic to get the image)
            QPixmap imagePixmap(":/enable.png");
            // Assuming 'rect' represents the bounding rectangle of the checkbox
            int middleColumnX = option.rect.left() + (option.rect.width() - imagePixmap.width()) / 2;
            int middleColumnY = option.rect.top() + (option.rect.height() - imagePixmap.height()) / 2;
            QRect imageRect(QPoint(middleColumnX, middleColumnY), imagePixmap.size());
            painter->drawPixmap(imageRect, imagePixmap);

        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser)
        {
            QColor color("#FFF");
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

            // Load an example image (replace with your logic to get the image)
            QPixmap imagePixmap(":/disable.png");
            int middleColumnX = option.rect.left() + (option.rect.width() - imagePixmap.width()) / 2;
            int middleColumnY = option.rect.top() + (option.rect.height() - imagePixmap.height()) / 2;
            QRect imageRect(QPoint(middleColumnX, middleColumnY), imagePixmap.size());
            painter->drawPixmap(imageRect, imagePixmap);
        }
        }

    else if (c==PortfolioData_Idx::_PortfolioNumber || c==PortfolioData_Idx::_AlgoName)
    {

        //  Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        //   PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());

        //42A5F5
       op.palette.setColor(QPalette::Highlight , HighlightColor);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active)
                painter->fillRect(option.rect, HighlightColor);
            else
                painter->fillRect(option.rect, QColor("#E0F1FF"));
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled)
        {
            QColor color("#E0F1FF");
            if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active)
                painter->fillRect(option.rect, HighlightColor);
            else
                painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FFF");
            if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active)
                painter->fillRect(option.rect, HighlightColor);
            else
                painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }


        if(portfolio->TradedHighlight== true ){
            QColor color("#cfc0ce");
            if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active)
                painter->fillRect(option.rect, HighlightColor);
            else
                painter->fillRect(option.rect, color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }

        //QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_BuyMarketRate || c==PortfolioData_Idx::_BuyAveragePrice || c==PortfolioData_Idx::_BuyPriceDifference || c==PortfolioData_Idx::_BuyTotalQuantity || c==PortfolioData_Idx::_BuyTradedQuantity || c==PortfolioData_Idx::_BuyRemainingQuantity){
        QStyleOptionViewItem op(option);


        if(c==PortfolioData_Idx::_BuyMarketRate){
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#D6FCF0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }

        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#D6FCF0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#F0FFFA");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        //QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_SellMarketRate || c==PortfolioData_Idx::_SellAveragePrice || c==PortfolioData_Idx::_SellPriceDifference || c==PortfolioData_Idx::_SellTotalQuantity || c==PortfolioData_Idx::_SellTradedQuantity || c==PortfolioData_Idx::_SellRemainingQuantity){
        QStyleOptionViewItem op(option);

        if(c==PortfolioData_Idx::_SellMarketRate){
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

        // Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        // PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#FED9D9");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }

        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#FED9D9");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FEE");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        //QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_OrderQuantity
             || c==PortfolioData_Idx::_Alias
             || c==PortfolioData_Idx::_MaxLoss
             || c == PortfolioData_Idx::_Leg1
             || c==PortfolioData_Idx::_ExpiryDateTime
             || c==PortfolioData_Idx::_Cost
             || c==PortfolioData_Idx::_InstrumentName
             || c==PortfolioData_Idx::_Leg2
             || c==PortfolioData_Idx::_Leg3
             || c==PortfolioData_Idx::_AdditionalData1
             || c==PortfolioData_Idx::_PortfolioType
             || c==PortfolioData_Idx::_Price
             || c==PortfolioData_Idx::_Depth
             || c==PortfolioData_Idx::_QuantityRatio
//             || c==PortfolioData_Idx::_BidLeg
             || c==PortfolioData_Idx:: _FuturePrice)
    {
        QStyleOptionViewItem op(option);
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(c==PortfolioData_Idx::_ExpiryDateTime){
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

        // Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        //  PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#E0F1FF");
            painter->fillRect(option.rect,  color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#E0F1FF");
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FFF");
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        if(portfolio->TradedHighlight== true ){
            QColor color("#cfc0ce");
            painter->fillRect(option.rect, color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }



        if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active &&  (c==PortfolioData_Idx::_AlgoName ||
                                                      c==PortfolioData_Idx::_MaxLoss||
                                                      c==PortfolioData_Idx::_Depth||
                                                      c==PortfolioData_Idx::_ExpiryDateTime ||
                                                      c==PortfolioData_Idx::_Cost||
                                                      c==PortfolioData_Idx::_QuantityRatio||
//                                                      c==PortfolioData_Idx::_BidLeg||
                                                      c==PortfolioData_Idx::_FuturePrice)){
            painter->fillRect(option.rect, HighlightColor);
        }


       // QStyledItemDelegate::paint(painter, op, index);
    }
    if (c==PortfolioData_Idx::_OrderQuantity||c==PortfolioData_Idx::_Alias)
    {
        // Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        //  PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());

        //42A5F5
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#d7cfe8");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled)
        {
            QColor color("#E0F1FF");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#e5e0f0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }



    }

    //draw  text section
    if(c!=PortfolioData_Idx::_Status){

        QString text = mappedIndex.data(Qt::DisplayRole).toString();
        int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, mappedIndex);


        // Set a clipping region to ensure drawing only happens within the cell bounds
        painter->save();  // Save the painter state
        painter->setClipRect(opt.rect);  // Set the clipping area to the cell's rectangle
        if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active && (c==PortfolioData_Idx::_AlgoName ||
                                                      c==PortfolioData_Idx::_PortfolioNumber ||
                                                      c==PortfolioData_Idx::_ExpiryDateTime ||
                                                      c==PortfolioData_Idx::_Cost||
                                                      c==PortfolioData_Idx::_QuantityRatio||
                                                     // c==PortfolioData_Idx::_BidLeg||
                                                      c==PortfolioData_Idx::_MaxLoss||
                                                      c==PortfolioData_Idx::_FuturePrice)){
            painter->setPen(opt.palette.color(QPalette::Light));  // Set the text color
        }
        else{
            painter->setPen(opt.palette.color(QPalette::Text));  // Set the text color
        }

        painter->setFont(opt.font);

        //if match found draw text and background hilight color
        if (startIndex >= 0) {
            // Split the text into before, match, and after parts
            QString before = text.left(startIndex);
            QString match = text.mid(startIndex, m_highlightText.length());
            QString after = text.mid(startIndex + m_highlightText.length());

            // Measure the width of the entire text (before + match + after) centered
            QRect fullTextRect = painter->boundingRect(opt.rect, Qt::AlignCenter, text);

            // Calculate the width of the "before" part
            int beforeWidth = painter->fontMetrics().horizontalAdvance(before);

            // Calculate the width of the highlighted "match" part
            int matchWidth = painter->fontMetrics().horizontalAdvance(match);

            // Calculate the position of the highlighted text relative to the centered text
            int highlightStartX = fullTextRect.left() + beforeWidth;
            QRect highlightRect(highlightStartX, opt.rect.top(), matchWidth, opt.rect.height());

            // Fill the background with yellow for the highlighted part
         painter->fillRect(highlightRect, QColor(229, 229, 227));



            // Draw the entire text, centered
            painter->drawText(opt.rect, Qt::AlignCenter, before + match + after);


        }
        else {
            // If no match, just draw the text normally
            painter->drawText(option.rect, Qt::AlignCenter, text);

        }
        painter->restore();

    }

    /*if(c!=PortfolioData_Idx::_Status){
        if (m_highlightText.isEmpty()) {
            QStyledItemDelegate::paint(painter, option, index);
        }
        else {
            QString text = index.data(Qt::DisplayRole).toString();
            QTextOption textOption;
            textOption.setAlignment(option.displayAlignment);
            QRect rect = option.rect;

            // Save painter state
            painter->save();

            // Use the default font and alignment from option
            painter->setFont(option.font);
            painter->setPen(option.palette.color(QPalette::Text));

            // Find the start index of the search text (case-insensitive)
            int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
            if (startIndex >= 0) {
                // Split the text into before, match, and after parts
                QString before = text.left(startIndex);
                QString match = text.mid(startIndex, m_highlightText.length());
                QString after = text.mid(startIndex + m_highlightText.length());

                // Draw the "before" text (non-highlighted)
                QRect beforeRect = painter->boundingRect(rect, Qt::AlignLeft, before);
                painter->drawText(beforeRect, before, textOption);

                // Calculate the rectangle for the highlighted text
                QRect matchRect = painter->boundingRect(rect, Qt::AlignLeft, before + match);

                // Set yellow background for the highlighted text
                QRect highlightRect(beforeRect.right(), rect.top(), matchRect.width() - beforeRect.width(), rect.height());
                painter->fillRect(highlightRect, Qt::yellow);  // Fill with yellow color

                // Draw the highlighted text on top of the yellow background
                painter->setPen(Qt::black);  // Change pen color to black for highlighted text
                painter->drawText(highlightRect, match, textOption);

                // Draw the "after" text (non-highlighted)
                QRect afterRect = painter->boundingRect(rect, Qt::AlignLeft, text);
                afterRect.setLeft(matchRect.right());
                painter->setPen(option.palette.color(QPalette::Text));  // Reset pen to default color
                painter->drawText(afterRect, after, textOption);
            } else {
                // If no match, draw the entire text as normal
                painter->drawText(rect, text, textOption);
            }

            // Restore painter state
            painter->restore();
        }
    }*/


   delete portfolio;
}
