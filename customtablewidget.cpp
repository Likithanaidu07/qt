#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>
#include <QPushButton>

class CustomTableWidget : public QTableWidget {
    Q_OBJECT

public:
    CustomTableWidget(QPushButton *pushButtonUpload, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPushButton *pushButtonUpload;
};

#endif // CUSTOMTABLEWIDGET_H
