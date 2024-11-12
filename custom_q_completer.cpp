#include "custom_q_completer.h"
const int custom_q_completer::CompleteRole = Qt::UserRole + 1;
#include "QElapsedTimer"

custom_q_completer::custom_q_completer(QObject *parent)
    : QCompleter{parent}
{
}


QString custom_q_completer::pathFromIndex(const QModelIndex &index) const{
    return index.data().toString();
    //return index.data(CompleteRole).toString(); //if you want to dsiplay value instead text enable this
}

QString custom_q_completer::get_foo_token_number_for_selected(const QModelIndex &index) const
{
    QString code = index.data(CompleteRole).toString();
    return code;
}

CustomSearchWidget::CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent)
    : QWidget(parent), listView(customListView), model(_model),debounceTimer(new QTimer(this))
{
    currentText = "";
    sortAscendingOrder = true;

  //  id= QDateTime::currentDateTime().toString("hh:mm:ss");
    // Fill the completionData with initial data
    QStringList completionData;
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column);
            QVariant data = index.data(Qt::DisplayRole);
            if (data.isValid()) {
                completionData << data.toString();
            }
        }
    }

    // Use QStringListModel if necessary
    stringListModel = new QStringListModel(completionData, this);

    // Instantiate the custom proxy model
    CustomSortFilterProxyModel *proxyModel = new CustomSortFilterProxyModel(this);
    //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);  // Set case insensitivity
    proxyModel->setSourceModel(model); // Use the original model

    // Set the proxy model to the list view
    listView->setModel(proxyModel);
    listView->hide();

    debounceTimer->setSingleShot(true);
    connect(debounceTimer, &QTimer::timeout, this, [this]() {
       do_filterItems(currentText); // Assume currentText is the latest text from the line edit
    });

}

void CustomSearchWidget::filterItems(const QString &text){
      // qDebug()<<id<<": filterItems()-----";
       currentText = text; // Store the current text
       //if(currentText!="")
        debounceTimer->start(300); // Restart the timer with a 300ms timeout (adjust as necessary)
}


void CustomSearchWidget::do_filterItems(const QString &text) {

   // qDebug()<<"\n\n"<<QDateTime::currentDateTime().toString("hh:mm:ss")<<" "<<id<<": New Filtering-->"+name;
    CustomSortFilterProxyModel *proxyModel = qobject_cast<CustomSortFilterProxyModel*>(listView->model());
   // QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(listView->model());

    if (!proxyModel)
        return;

    if (text.isEmpty()) {
        // If the search text is empty, clear the filter and hide the list view
      //  qDebug()<<QDateTime::currentDateTime().toString("hh:mm:ss")<<" setFilterRegularExpression before";
       // proxyModel->setFilterRegularExpression("");
        proxyModel->invalidateCustomFilter();

       // qDebug()<<QDateTime::currentDateTime().toString("hh:mm:ss")<<" ListView->hide(): do_filterItems - 1";

        listView->hide();
        return;
    }

    QElapsedTimer timer;
    timer.start();

    // Step 1: Perform the "starts with" search
    QStringList searchTerms = text.split(" ", Qt::SkipEmptyParts);

    QString startsWithPattern;
    int i = 0;
    for (const QString &term : searchTerms) {
        if (i == 0) {
            startsWithPattern += "^" + QRegularExpression::escape(term);
        } else {
            startsWithPattern += "(?=.*" + QRegularExpression::escape(term) + ")";
        }
        i++;
    }

    QRegularExpression startsWithRegex(startsWithPattern, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(startsWithRegex);

    proxyModel->setSortRole(ConvertAlog_Model_Roles::CustomSortingDataRole); // Sort by custom date roleCompositeKeyRole
    // Sort after setting the filter
   // proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive); // Optional: set case sensitivity
    if(sortAscendingOrder)
        proxyModel->sort(0, Qt::AscendingOrder); // Sort by column 0 (adjust column index as needed) in ascending order
    else
        proxyModel->sort(0, Qt::DescendingOrder); // Sort by column 0 (adjust column index as needed) in Descending order

    // Show or hide the list view based on the filter result
    if (proxyModel->rowCount() > 0) {
        if (!listView->isVisible()) {
            listView->show();
           // qDebug()<<"ListView->show() called...";

        }
        //qDebug() <<"text: "<<text<< "Filter time in seconds:  " << timer.elapsed() / 1000.0;
        //qDebug()<<"ListView->hide(): do_filterItems - 2";

        return;  // If matches are found, exit the function
    }

    // Step 2: Perform the "contains" search if no matches are found
    QString containsPattern = ".*";
    for (const QString &term : searchTerms) {
        containsPattern += QRegularExpression::escape(term) + ".*";
    }

    QRegularExpression containsRegex(containsPattern, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(containsRegex);

   // proxyModel->setSortRole(Qt::UserRole + 2); // Sort by custom date role
    // Sort after setting the filter
    //proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive); // Optional: set case sensitivity
    if(sortAscendingOrder)
        proxyModel->sort(0, Qt::AscendingOrder); // Sort by column 0 (adjust column index as needed) in ascending order
    else
         proxyModel->sort(0, Qt::DescendingOrder); // Sort by column 0 (adjust column index as needed) in Descending order

    // Show or hide the list view based on the filter result
    if (proxyModel->rowCount() > 0) {
        if (!listView->isVisible()) {
            listView->show();
        }
    } else {
        listView->hide();
    }
   // qDebug()<<"text: "<<text << "Filter time in seconds:  " << timer.elapsed() / 1000.0;
}

