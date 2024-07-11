#ifndef SORTSETTINGPOPUP_H
#define SORTSETTINGPOPUP_H

#include <QDialog>

namespace Ui {
class SortSettingPopUp;
}

class SortSettingPopUp : public QDialog
{
    Q_OBJECT

public:
    explicit SortSettingPopUp(QWidget *parent = nullptr);
    ~SortSettingPopUp();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButtonRemove_clicked();

    void on_pushButtonAdd_clicked();

signals:
    void reloadSortSettingSignal();

private:
    Ui::SortSettingPopUp *ui;
};

#endif // SORTSETTINGPOPUP_H
