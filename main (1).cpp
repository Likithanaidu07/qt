// #include "mainwindow.h"

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }

#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>

#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>

#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPixmap>

// Custom delegate for cell styling
class CustomDelegate : public QStyledItemDelegate {
public:
    explicit CustomDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        painter->save();

        // Alternate row coloring
        painter->fillRect(option.rect, (index.row() % 2 == 0) ? QColor(220, 220, 220) : Qt::white);

        // Draw text
        QString text = index.data(Qt::DisplayRole).toString();
        painter->setPen(Qt::black);
        painter->drawText(option.rect, Qt::AlignCenter, text);

        painter->restore();
    }
};

// Custom header view for sorting with custom image
class CustomHeaderView : public QHeaderView {
public:
    explicit CustomHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QHeaderView(orientation, parent) {
        setSectionsClickable(true);
        setSortIndicatorShown(true);
    }

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override {
        painter->save();

        // Custom header background
        painter->fillRect(rect, QColor(100, 100, 200));

        // Draw header text
        painter->setPen(Qt::white);
        QString text = model()->headerData(logicalIndex, Qt::Horizontal).toString();
        painter->drawText(rect.adjusted(5, 0, -20, 0), Qt::AlignVCenter | Qt::AlignLeft, text);

        // Load custom sorting images
        QPixmap ascPixmap(":/Res/arrow-up.png");  // Make sure the image exists in your resources
        QPixmap descPixmap(":/Res/upload.png");

        if (sortIndicatorSection() == logicalIndex) {
            QPixmap pixmap = (sortIndicatorOrder() == Qt::AscendingOrder) ? ascPixmap : descPixmap;
            QRect iconRect(rect.right() - 18, rect.top() + (rect.height() - pixmap.height()) / 2, pixmap.width(), pixmap.height());
            painter->drawPixmap(iconRect, pixmap);
        }

        painter->restore();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a model
    QStandardItemModel model(5, 3); // 5 rows, 3 columns
    model.setHorizontalHeaderLabels({"Column 1", "Column 2", "Column 3"});

    // Populate model with random data
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            model.setItem(row, col, new QStandardItem(QString::number(row)));
        }
    }

    // Create a proxy model for sorting
    QSortFilterProxyModel proxyModel;
    proxyModel.setSourceModel(&model);
    proxyModel.setSortCaseSensitivity(Qt::CaseInsensitive);

    // Create a table view
    QTableView tableView;
    tableView.setModel(&proxyModel);
    tableView.setSortingEnabled(true);
    tableView.setItemDelegate(new CustomDelegate(&tableView));

    // Set custom header view
    CustomHeaderView *headerView = new CustomHeaderView(Qt::Horizontal, &tableView);
    tableView.setHorizontalHeader(headerView);
    headerView->setSectionsClickable(true);
    headerView->setSortIndicatorShown(true);
    headerView->setSortIndicator(0, Qt::AscendingOrder); // Default sort order

    tableView.resize(500, 300);
    tableView.show();

    return app.exec();
}


