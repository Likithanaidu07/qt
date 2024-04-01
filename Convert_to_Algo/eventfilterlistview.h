#ifndef EVENTFILTERLISTVIEW_H
#define EVENTFILTERLISTVIEW_H
#include <QtCore>
#include <QObject>
#include "QLineEdit"
#include "QTableWidget"
#include <QListView>
#include "QMessageBox"
#include "QDateTime"

class EventFilter : public QObject
{
    Q_OBJECT
public:
    EventFilter(QLineEdit *lineEdit, QListView *listView, QObject *parent = nullptr)
        : QObject(parent), lineEdit(lineEdit), listView(listView) {}

signals:
    void signalItemSelected(const QModelIndex);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QLineEdit *lineEdit;
    QListView *listView;
};

#endif // EVENTFILTERLISTVIEW_H
