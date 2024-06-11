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
    if(market_type=="fo"){
        price_diff_incrementer = FO_PRICE_DIFF_INCREMENTER;
    }
    else{
        price_diff_incrementer = CDS_PRICE_DIFF_INCREMENTER;
    }

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
            qDebug() << "Image Clicked!";
            model->setData(index, index.data(Qt::ItemIsUserCheckable).value<Qt::CheckState>(), Qt::ItemIsUserCheckable);
            // If you need to stop further processing of the event, you can use event->accept();
            // event->accept();
        }
        return true;  // Consume the event
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget *Table_Portfolios_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    int c = index.column();
    if ( c == PortfolioData_Idx::_OrderQuantity)
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new IntDiffValidator(1, 1000000000, editor));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));

        return editor;
    }
    else if( c == PortfolioData_Idx::_SellTotalQuantity ||
             c == PortfolioData_Idx::_BuyTotalQuantity ){
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(0, 9999));
        editor->installEventFilter(const_cast<Table_Portfolios_Delegate *>(this));
        return editor;
    }
    else if(c == PortfolioData_Idx::_SellPriceDifference ||
             c == PortfolioData_Idx::_BuyPriceDifference ){
        QLineEdit *editor = new QLineEdit(parent);
        if(market_type=="fo")
            editor->setValidator(new DiffValidator(-9999.99, 9999.99, FO_DECIMAL_PRECISION,false, editor));
        else
            editor->setValidator(new DiffValidator(-9.9995, 9.9995, CDS_DECIMAL_PRECISION,true, editor));
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
        c==PortfolioData_Idx::_OrderQuantity)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        qDebug()<<"Table_Portfolios_Delegate::setEditorData: "<<value;

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


void Table_Portfolios_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const
{
    int c= index.column();
    //clean-code remove code once status coulmn stable
    // if(c == PortfolioData_Idx::_Status ){
    //    QCheckBox* check = static_cast<QCheckBox*>(editor);
    //    int value =check->isChecked();

    //     if(value==SwitchState::Checked)
    //         value = SwitchState::Unchecked;
    //     else if (value==SwitchState::Unchecked)
    //         value = SwitchState::Checked;

    //     model->setData(index, value, Qt::EditRole);

    // }
    if(c==PortfolioData_Idx::_SellPriceDifference ||
        c==PortfolioData_Idx::_BuyPriceDifference ||
        c==PortfolioData_Idx::_SellTotalQuantity ||
        c==PortfolioData_Idx::_BuyTotalQuantity ||
        c==PortfolioData_Idx::_OrderQuantity)
    {
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        {
            bool ok;
            QString str = lineEdit->text();
            qDebug()<<"Table_Portfolios_Delegate::setModelData: "<<str;
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
            PortfolioData_Idx::_OrderQuantity
        }; // these are the editable table cell indexes in the algo table

        if (editableIDx.contains(currentColIdx))
        {
            double incStep = quantity_incrementer;
            if (currentColIdx == PortfolioData_Idx::_SellPriceDifference || currentColIdx == PortfolioData_Idx::_BuyPriceDifference){
                incStep = price_diff_incrementer;
            }
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right) {
                if (currentIndex.isValid()) {
                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj)){
                        QString value = lineEdit->text();
                        bool ok;
                        double incValue = value.toDouble(&ok);
                        if (ok) {
                            int divisor = 5;
                            int result = std::round(incValue * 100);
                            if (keyEvent->key() == Qt::Key_Right){
                                if(std::fmod(result, divisor) == 0.0)
                                    incValue = incValue + incStep;
                                else{
                                    double roundedNumber = std::ceil(incValue / 0.05) * 0.05;
                                    incValue = roundedNumber;
                                }
                            }
                            else if (keyEvent->key() == Qt::Key_Left){
                                if(std::fmod(result, divisor) == 0.0){
                                    incValue = incValue - incStep;
                                }
                                else{
                                    double roundedNumber = std::floor(incValue / 0.05) * 0.05;
                                    incValue = roundedNumber;
                                }
                            }
                            if(currentIndex.column() == PortfolioData_Idx::_BuyTotalQuantity || currentIndex.column() == PortfolioData_Idx::_SellTotalQuantity){
                                if(incValue < 0)
                                    incValue = 0;
                            }
                            if(currentIndex.column() == PortfolioData_Idx::_OrderQuantity){
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
            else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if (currentIndex.isValid())
                {
                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj))
                    {
                        double currentValue = lineEdit->text().toDouble();
                        double multiplied = round(currentValue * 20.0);
                        double newvalue = multiplied / 20.0;
                        mutableModel = const_cast<QAbstractItemModel *>(currentIndex.model());
                        mutableModel->setData(currentIndex, newvalue, Qt::EditRole);
                        // Close the editor and commit the data
                        emit commitData(lineEdit);
                        emit closeEditor(lineEdit);
                        mutableModel = nullptr;
                        emit editFinished(QString::number(currentValue), currentIndex);
                        return true;
                    }
                }
            }
            else if (keyEvent->key() == Qt::Key_Backtab) {
                // Tab key and Shift key are pressed simultaneously, do something here
                emit tabKeyPressed(nav_direction::nav_backward);
            }
            else if (keyEvent->key() == Qt::Key_Tab){
                emit tabKeyPressed(nav_direction::nav_forward);
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
    auto c= index.column();
    auto r= index.row();
    QStyleOptionViewItem op(option);

    if(option.state & QStyle::State_MouseOver) {
        if(c==PortfolioData_Idx::_AlgoName)
            painter->fillRect(option.rect, Qt::black);
        QStyledItemDelegate::paint(painter, op, index);
    }

    if(c == PortfolioData_Idx::_Status)
    {
        QPixmap checkboxPixmap(10,10);
        checkboxPixmap.fill(Qt::transparent); // Fill with transparent background

        QStyleOptionButton checkboxOption;
        checkboxOption.state |= index.data(Qt::ItemIsUserCheckable).toBool() ? QStyle::State_On : QStyle::State_Off;
        checkboxOption.rect = checkboxPixmap.rect(); // Adjust rect for drawing on the pixmap

        // Manually draw the checkbox on the pixmap
        QRect checkboxRect = checkboxOption.rect.adjusted(0, 0, -2, -2);
        QPixmap pixmap(checkboxRect.width(), checkboxRect.height());  // Adjusted pixmap size
        // Draw the pixmap with the updated position
        QRect rect(option.rect.left(), option.rect.top(), checkboxRect.width(), checkboxRect.height());  // Adjusted rect position

        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());

        //42A5F5
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active)
        {
            QColor color("#FFF");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
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
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

            // Load an example image (replace with your logic to get the image)
            QPixmap imagePixmap(":/disable.png");
            int middleColumnX = option.rect.left() + (option.rect.width() - imagePixmap.width()) / 2;
            int middleColumnY = option.rect.top() + (option.rect.height() - imagePixmap.height()) / 2;
            QRect imageRect(QPoint(middleColumnX, middleColumnY), imagePixmap.size());
            painter->drawPixmap(imageRect, imagePixmap);

        }

    }

    if (/*c == PortfolioData_Idx::_Status ||*/ c==PortfolioData_Idx::_PortfolioNumber || c==PortfolioData_Idx::_AlgoName)
    {

        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());

        //42A5F5
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#E0F1FF");
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
            painter->drawLine(p1,p2);
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled)
        {
            QColor color("#E0F1FF");
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FFF");
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_BuyMarketRate || c==PortfolioData_Idx::_BuyAveragePrice || c==PortfolioData_Idx::_BuyPriceDifference || c==PortfolioData_Idx::_BuyTotalQuantity || c==PortfolioData_Idx::_BuyTradedQuantity || c==PortfolioData_Idx::_BuyRemainingQuantity){
        QStyleOptionViewItem op(option);


        if(c==PortfolioData_Idx::_BuyMarketRate){
            double borderWidth = 1;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#D6FCF0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
            painter->drawLine(p1,p2);
        }

        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#D6FCF0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#F0FFFA");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_SellMarketRate || c==PortfolioData_Idx::_SellAveragePrice || c==PortfolioData_Idx::_SellPriceDifference || c==PortfolioData_Idx::_SellTotalQuantity || c==PortfolioData_Idx::_SellTradedQuantity || c==PortfolioData_Idx::_SellRemainingQuantity){
        QStyleOptionViewItem op(option);

        if(c==PortfolioData_Idx::_SellMarketRate){
            double borderWidth = 1;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#FED9D9");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect,  color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
            painter->drawLine(p1,p2);
        }

        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#FED9D9");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FEE");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        QStyledItemDelegate::paint(painter, op, index);
    }
    else if(c==PortfolioData_Idx::_OrderQuantity
               || c == PortfolioData_Idx::_Leg1
               || c==PortfolioData_Idx::_ExpiryDateTime
               || c==PortfolioData_Idx::_Cost
               || c==PortfolioData_Idx::_InstrumentName
               || c==PortfolioData_Idx::_Leg2
               || c==PortfolioData_Idx::_Leg3
               || c==PortfolioData_Idx::_AdditionalData1
               || c==PortfolioData_Idx::_PortfolioType
               || c==PortfolioData_Idx::_Price
               || c==PortfolioData_Idx::_SkipMarketStrike
               || c==PortfolioData_Idx::_QuantityRatio
               || c==PortfolioData_Idx::_BidLeg
               || c==PortfolioData_Idx:: _FuturePrice)
    {
        QStyleOptionViewItem op(option);
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(c==PortfolioData_Idx::_ExpiryDateTime){
            double borderWidth = 1;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());
        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#E0F1FF");
            painter->fillRect(option.rect,  color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled){
            QColor color("#E0F1FF");
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#FFF");
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        QStyledItemDelegate::paint(painter, op, index);
    }
    if (/*c == PortfolioData_Idx::_Status ||*/ c==PortfolioData_Idx::_OrderQuantity)
    {
        Table_Portfolios_Model *model = (Table_Portfolios_Model*) index.model();
        PortfolioObject *portfolio = model->portfolio_data_list.at(index.row());

        //42A5F5
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(portfolio->StatusVal.toInt()==portfolio_status::Active){
            QColor color("#d7cfe8");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
            painter->drawLine(p1,p2);
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::Filled)
        {
            QColor color("#E0F1FF");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
        else if(portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser){
            QColor color("#e5e0f0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);
            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        QStyledItemDelegate::paint(painter, op, index);
    }
}
