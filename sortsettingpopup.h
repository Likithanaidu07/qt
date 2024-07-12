#ifndef SORTSETTINGPOPUP_H
#define SORTSETTINGPOPUP_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

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
    void saveConfig();
    bool saveFlg;

protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // SORTSETTINGPOPUP_H
