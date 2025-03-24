#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QDialog>
#include "ui_settings_window.h"
#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include "QStandardPaths"
#include <QFile>
#include <QSettings>
#include "QMessageBox"
#include "defines.h"


#include <QStyledItemDelegate>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDebug>
#include <QItemDelegate>


class ShortcutDelegate : public QItemDelegate {
    Q_OBJECT
public:
    ShortcutDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setPlaceholderText("Press keys...");
        editor->setReadOnly(true);  // Prevent normal text input
        editor->installEventFilter(const_cast<ShortcutDelegate*>(this));  // Event filter to capture keys
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString shortcut = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            lineEdit->setText(shortcut);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            QString newShortcut = lineEdit->text();
            model->setData(index, newShortcut, Qt::EditRole);
        }
    }
   /* bool eventFilter(QObject *editor, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);

            if (lineEdit) {
                QString keyText;

                int key = keyEvent->key();

                // Handle individual keys (Space, Delete, F1-F12)
                if (key == Qt::Key_Space) {
                    keyText = "Space";
                } else if (key == Qt::Key_Delete) {
                    keyText = "Delete";
                } else if (key >= Qt::Key_F1 && key <= Qt::Key_F12) {
                    keyText = "F" + QString::number(key - Qt::Key_F1 + 1);
                } else {
                    // If the key is not Space, Delete, or F1-F12, ignore it
                    return false;
                }

                // Set the detected key in the line editor
                lineEdit->setText(keyText);

                // Handle Enter key (to commit the change)
                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                    emit commitData(lineEdit);
                    emit closeEditor(lineEdit);
                    return true;
                }

                // Handle Escape key (to revert the change)
                if (keyEvent->key() == Qt::Key_Escape) {
                    emit closeEditor(lineEdit, QAbstractItemDelegate::RevertModelCache);
                    return true;
                }

                return true;  // Consume the event
            }
        }

        return QItemDelegate::eventFilter(editor, event);
    }*/


    bool eventFilter(QObject *editor, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);

            if (lineEdit) {
                QString keyText;

                // Accept only if Ctrl, Alt, or Shift modifiers are held
                bool ctrl = keyEvent->modifiers() & Qt::ControlModifier;
                bool alt = keyEvent->modifiers() & Qt::AltModifier;
                bool shift = keyEvent->modifiers() & Qt::ShiftModifier;

                // Check if a non-modifier key is pressed
                int key = keyEvent->key();
                bool isModifierKey = (key == Qt::Key_Control || key == Qt::Key_Alt || key == Qt::Key_Shift);

                if ((ctrl || alt || shift) && !isModifierKey) {
                    if (ctrl) keyText += "Ctrl+";
                    if (alt) keyText += "Alt+";
                    if (shift) keyText += "Shift+";

                    // Append the actual key (non-modifier)
                    if (key >= Qt::Key_A && key <= Qt::Key_Z) {
                        keyText += QChar(key);  // Convert key to letter
                    } else if (key >= Qt::Key_0 && key <= Qt::Key_9) {
                        keyText += QChar(key);  // Convert key to number
                    } else {
                        // Handle non-alphabetic keys (Enter, Delete, etc.)
                        keyText += QKeySequence(keyEvent->key()).toString();
                    }

                    // Set the text in the line editor
                    lineEdit->setText(keyText);
                }
                else if (key == Qt::Key_Space) {
                    keyText = "Space";
                    lineEdit->setText(keyText);
                }
                else if (key == Qt::Key_Delete) {
                     keyText = "Delete";
                     lineEdit->setText(keyText);
                }
                else if (key >= Qt::Key_F1 && key <= Qt::Key_F12) {
                     keyText = "F" + QString::number(key - Qt::Key_F1 + 1);
                     lineEdit->setText(keyText);
                }


                else if (isModifierKey) {
                    // Ignore if only a modifier key is pressed without other keys
                    return true;
                }

                // Handle Enter key (to commit the change)
                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                    emit commitData(lineEdit);
                    emit closeEditor(lineEdit);
                    return true;
                }

                // Handle Escape key (to revert the change)
                if (keyEvent->key() == Qt::Key_Escape) {
                    emit closeEditor(lineEdit, QAbstractItemDelegate::RevertModelCache);
                    return true;
                }

                return true; // Consume the event
            }
        }

        return QItemDelegate::eventFilter(editor, event);
    }
};




namespace Ui {
class Settings_Window;
}

class Settings_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_Window(QWidget *parent = nullptr);
    ~Settings_Window();
    void createHotKeyData(const QString &fileName);
    void loadHotkeyFromIniFileAndPopulateTreeView();
    QList<HotKeysData_Grouped> hotKeyDataList; // this for UI pourpouse to group in treeview
    QMap<QString, HotKeyDetails> updatedHotKeys; // To store modified hotkeys
    QHash<QString, HotKeyDetails> HotKeysHash; // To store  hotkeys data accross app
    void initFontSett();

private:
    Ui::Settings_Window *ui;
    QStandardItemModel *model;
    QString settingFile;

signals:
    void updateHotKeysSignal(QStringList);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButtonSaveHotKeys_clicked();
    void treeviewHotKeyItemEdited(QStandardItem *item);

    void on_pushButtonChangeFont_clicked();
};

#endif // SETTINGS_WINDOW_H
