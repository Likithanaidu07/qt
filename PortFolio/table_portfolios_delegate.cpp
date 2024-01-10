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


QWidget *Table_Portfolios_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    int c = index.column();
    if(c == PortfolioData_Idx::_Status){
//        QCheckBox* check = new QCheckBox(parent);
//        return check;
        SwitchButton* sbtn = new SwitchButton(parent);
        return sbtn;
    }
    if ( c == PortfolioData_Idx::_OrderQuantity)
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(0, 1000000000));
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
    if(c == PortfolioData_Idx::_Status ){
        int value = index.model()->data(index, Qt::CheckStateRole).toInt();
        qDebug()<<"Table_Portfolios_Delegate::setEditorData: checkbox="<<value;
//        QCheckBox* check = static_cast<QCheckBox*>(editor);
        SwitchButton* sbtn =static_cast<SwitchButton*>(editor); // Default style is Style::ONOFF
        bool current = sbtn->value();
//        sbtn->setValue(!current);
        if(value==0){
//            check->setChecked(false);
            sbtn->setValue(false);
        }
        else{
//            check->setChecked(true);
            sbtn->setValue(true);
        }

    }

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
    if(c == PortfolioData_Idx::_Status ){
//        QCheckBox* check = static_cast<QCheckBox*>(editor);
//        int value =check->isChecked();

        SwitchButton* sbtn = static_cast<SwitchButton*>(editor); // Default style is Style::ONOFF
        bool value = sbtn->value();

        qDebug()<<"Table_Portfolios_Delegate::setModelData: checkbox="<<value;

//        if(value==portfolio_status::Filled)
//            value = portfolio_status::DisabledByUser;
//        else if (value==portfolio_status::DisabledByUser)
//            value = portfolio_status::Active;

        if(value==SwitchState::Checked)
            value = SwitchState::Unchecked;
        else if (value==SwitchState::Unchecked)
            value = SwitchState::Checked;

        model->setData(index, value, Qt::EditRole);

    }
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
        // qDebug()<<"row: "<<currentIndex.row()<<"column: "<<currentIndex.column();

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
            double incStep =quantity_incrementer;
            if(currentColIdx == PortfolioData_Idx::_SellPriceDifference || currentColIdx == PortfolioData_Idx::_BuyPriceDifference){
                incStep = price_diff_incrementer;
            }
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right) {
                //QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
                if (currentIndex.isValid()) {

                    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj)){
                        //  qDebug()<<"incStep: "<<incStep;

                        QString value = lineEdit->text();
                        bool ok;
                        double incValue = value.toDouble(&ok);
                        if (ok) {
                            int divisor = 5;
                            int result = std::round(incValue * 100);
                            if (keyEvent->key() == Qt::Key_Right){
                                if(std::fmod(result, divisor) == 0.0)
                                    incValue = incValue+incStep;
                                else{
                                    double roundedNumber = std::ceil(incValue / 0.05) * 0.05;
                                    incValue=roundedNumber;
                                }
                            }
                            else if (keyEvent->key() == Qt::Key_Left){
                                if(std::fmod(result, divisor) == 0.0){
                                    incValue = incValue-incStep;
                                }
                                else{
                                    double roundedNumber = std::floor(incValue / 0.05) * 0.05;
                                    incValue=roundedNumber;
                                }
                            }
                            if(currentIndex.column()== PortfolioData_Idx::_BuyTotalQuantity||currentIndex.column()== PortfolioData_Idx::_SellTotalQuantity){
                                if(incValue<0)
                                    incValue = 0;
                            }
                            value = QString::number(incValue);
                            lineEdit->setText(value);
                            // currentIndex.model()->setData(currentIndex, value, Qt::EditRole);
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
                        double newvalue= multiplied / 20.0;
                        mutableModel = const_cast<QAbstractItemModel *>(currentIndex.model());
                        mutableModel->setData(currentIndex, newvalue, Qt::EditRole);
                        // Close the editor and commit the data
                        emit commitData(lineEdit);
                        emit closeEditor(lineEdit);
                        mutableModel = nullptr;
                        emit editFinished(QString::number(currentValue),currentIndex);
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
                // qDebug()<<"\n\n----------------------Key_Tab pressed-----------\n\n";
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

    if (c == PortfolioData_Idx::_Status || c==PortfolioData_Idx::_PortfolioNumber || c==PortfolioData_Idx::_AlgoName)
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


    else if(c==PortfolioData_Idx::_Leg1 || c==PortfolioData_Idx::_ExpiryDateTime || c==PortfolioData_Idx::_Cost){
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
}
