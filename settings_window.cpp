#include "settings_window.h"
#include <QFontDialog>

Settings_Window::Settings_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_Window)
{
    ui->setupUi(this);


    // Set the stylesheet
       ui->tabWidget->setStyleSheet(
           "QTabWidget::pane {"
           "   border: 1px solid #C4C4C3;"
           "   background-color: #F5F5F5;"
           "}"
           "QTabWidget::tab-bar {"
           "   background-color: #46454D;"
           "}"
           "QTabBar::tab {"
           "   background-color: #46454D;"
           "   color: #000000;"
           "   border: 1px solid #C4C4C3;"
           "   padding: 3px;"
           "   min-width: 20px;"
           "   min-height: 30px;"
           "   border-top-left-radius: 2px;"
           "   border-top-right-radius: 2px;"
           "   font-weight: bold;"

           "}"
           "QTabBar::tab:selected {"
           "   background-color: #596167;"
           "   color: #FFFFFF;"
           "   font-weight: bold;"
           "}"
           "QTabBar::tab:hover {"
           "   background-color: #6D747B;"
           "}"
           "QTabBar::tab:!selected {"
           "   margin-top: 2px;"
           "}"
       );

       ui->tabHotkeys->setStyleSheet("background-color: #E9ECEF;");
       ui->tabGeneral->setStyleSheet("background-color: #E9ECEF;");




       model = new QStandardItemModel();
       connect(model, &QStandardItemModel::itemChanged, this, &Settings_Window::treeviewHotKeyItemEdited);

       ShortcutDelegate *delegate = new ShortcutDelegate(ui->treeView);
       ui->treeView->setItemDelegateForColumn(1, delegate); // Assuming the shortcut column is column 1
       // Assign the model to the QTreeView
       ui->treeView->setModel(model);





       //Create App Data folder if not exist

       settingFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data/hotkeys.ini";
       createHotKeyData(settingFile);
       //loadHotkeyFromIniFileAndPopulateTreeView(settingFile);

       initFontSett();

}


void Settings_Window::createHotKeyData(const QString &fileName){

    hotKeyDataList.clear();

    HotKeysData_Grouped GlobalHotkeys;
    GlobalHotkeys.category = "Global";
    GlobalHotkeys.title = "Global Keys";

    HotKeyDetails OPEN_ALGO;
    OPEN_ALGO.action = "OPEN_ALGO";
    OPEN_ALGO.description = "Open Algo Window";
    OPEN_ALGO.shortcut = "Alt+A";
    OPEN_ALGO.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_ALGO);

    HotKeyDetails OPEN_TRADES;
    OPEN_TRADES.action = "OPEN_TRADES";
    OPEN_TRADES.description = "Open Trades Window";
    OPEN_TRADES.shortcut = "Alt+T";
    OPEN_TRADES.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_TRADES);

    HotKeyDetails OPEN_POSITION;
    OPEN_POSITION.action = "OPEN_POSITION";
    OPEN_POSITION.description = "Open Position Window";
    OPEN_POSITION.shortcut = "Alt+P";
    OPEN_POSITION.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_POSITION);


    HotKeyDetails OPEN_LINERS;
    OPEN_LINERS.action = "OPEN_LINERS";
    OPEN_LINERS.description = "Open Liners Window";
    OPEN_LINERS.shortcut = "Alt+L";
    OPEN_LINERS.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_LINERS);


    HotKeyDetails OPEN_HISTORICAL_POSITION;
    OPEN_HISTORICAL_POSITION.action = "OPEN_HISTORICAL_POSITION";
    OPEN_HISTORICAL_POSITION.description = "Open Historical Position Window";
    OPEN_HISTORICAL_POSITION.shortcut = "Alt+H";
    OPEN_HISTORICAL_POSITION.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_HISTORICAL_POSITION);

    HotKeyDetails OPEN_MISSED_TRADES;
    OPEN_MISSED_TRADES.action = "OPEN_MISSED_TRADES";
    OPEN_MISSED_TRADES.description = "Open Missed Trades Window";
    OPEN_MISSED_TRADES.shortcut = "Alt+M";
    OPEN_MISSED_TRADES.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_MISSED_TRADES);

    HotKeyDetails OPEN_TEMPLATES;
    OPEN_TEMPLATES.action = "OPEN_TEMPLATES";
    OPEN_TEMPLATES.description = "Open Templates Window";
    OPEN_TEMPLATES.shortcut = "Alt+Q";
    OPEN_TEMPLATES.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_TEMPLATES);

    HotKeyDetails OPEN_SUMMARY;
    OPEN_SUMMARY.action = "OPEN_SUMMARY";
    OPEN_SUMMARY.description = "Open Summary Card";
    OPEN_SUMMARY.shortcut = "Ctrl+S";
    OPEN_SUMMARY.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_SUMMARY);

    HotKeyDetails OPEN_WATCH;
    OPEN_WATCH.action = "OPEN_WATCH";
    OPEN_WATCH.description = "Open Watch Card";
    OPEN_WATCH.shortcut = "Ctrl+W";
    OPEN_WATCH.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_WATCH);

    HotKeyDetails OPEN_LOGS;
    OPEN_LOGS.action = "OPEN_LOGS";
    OPEN_LOGS.description = "Open Logs Card";
    OPEN_LOGS.shortcut = "Ctrl+L";
    OPEN_LOGS.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_LOGS);


    HotKeyDetails OPEN_SETTINGS;
    OPEN_SETTINGS.action = "OPEN_SETTINGS";
    OPEN_SETTINGS.description = "Open Settings Window";
    OPEN_SETTINGS.shortcut = "Ctrl+T";
    OPEN_SETTINGS.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_SETTINGS);

    HotKeyDetails OPEN_F1_WIN;
    OPEN_F1_WIN.action = "OPEN_F1_WIN";
    OPEN_F1_WIN.description = "Open Manul Order Buy";
    OPEN_F1_WIN.shortcut = "F1";
    OPEN_F1_WIN.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_F1_WIN);

    HotKeyDetails OPEN_F2_WIN;
    OPEN_F2_WIN.action = "OPEN_F2_WIN";
    OPEN_F2_WIN.description = "Open Manul Order Sell";
    OPEN_F2_WIN.shortcut = "F2";
    OPEN_F2_WIN.enabled = true;
    GlobalHotkeys.HotKeyList.append(OPEN_F2_WIN);






    HotKeysData_Grouped Algo_TableHotkeys;
    Algo_TableHotkeys.category = "Algo_Table";
    Algo_TableHotkeys.title = "Algo Table";

    HotKeyDetails OPEN_BUILD_ALOG;
    OPEN_BUILD_ALOG.action = "OPEN_BUILD_ALOG";
    OPEN_BUILD_ALOG.description = "Open BuildAlgo Window";
    OPEN_BUILD_ALOG.shortcut = "Ctrl+B";
    OPEN_BUILD_ALOG.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(OPEN_BUILD_ALOG);

    HotKeyDetails OPEN_ALGO_INFO_WIN;
    OPEN_ALGO_INFO_WIN.action = "OPEN_ALGO_INFO_WIN";
    OPEN_ALGO_INFO_WIN.description = "Open Algo Info Window";
    OPEN_ALGO_INFO_WIN.shortcut = "F3";
    OPEN_ALGO_INFO_WIN.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(OPEN_ALGO_INFO_WIN);

    HotKeyDetails ENABLE_DISABLE_SELECTED_ALGO;
    ENABLE_DISABLE_SELECTED_ALGO.action = "ENABLE_DISABLE_SELECTED_ALGO";
    ENABLE_DISABLE_SELECTED_ALGO.description = "Enable/Disable Selected Algo";
    ENABLE_DISABLE_SELECTED_ALGO.shortcut = "Space";
    ENABLE_DISABLE_SELECTED_ALGO.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(ENABLE_DISABLE_SELECTED_ALGO);

    HotKeyDetails IMPORT_ALGOS;
    IMPORT_ALGOS.action = "IMPORT_ALGOS";
    IMPORT_ALGOS.description = "Import Algos";
    IMPORT_ALGOS.shortcut = "Shift+I";
    IMPORT_ALGOS.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(IMPORT_ALGOS);

    HotKeyDetails EXPORT_ALGOS;
    EXPORT_ALGOS.action = "EXPORT_ALGOS";
    EXPORT_ALGOS.description = "Export Algos";
    EXPORT_ALGOS.shortcut = "Shift+X";
    EXPORT_ALGOS.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(EXPORT_ALGOS);

    HotKeyDetails OPEN_ALGO_SORT_WIN;
    OPEN_ALGO_SORT_WIN.action = "OPEN_ALGO_SORT_WIN";
    OPEN_ALGO_SORT_WIN.description = "Open Algo Sorting";
    OPEN_ALGO_SORT_WIN.shortcut = "Shift+S";
    OPEN_ALGO_SORT_WIN.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(OPEN_ALGO_SORT_WIN);

    HotKeyDetails DELETE_SELECTED_ALGO;
    DELETE_SELECTED_ALGO.action = "DELETE_SELECTED_ALGO";
    DELETE_SELECTED_ALGO.description = "Delete Selected Algo";
    DELETE_SELECTED_ALGO.shortcut = "Delete";
    DELETE_SELECTED_ALGO.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(DELETE_SELECTED_ALGO);


    HotKeyDetails ENABLE_ALL_ALGO;
    ENABLE_ALL_ALGO.action = "ENABLE_ALL_ALGO";
    ENABLE_ALL_ALGO.description = "Enable all Algos";
    ENABLE_ALL_ALGO.shortcut = "Shift+E";
    ENABLE_ALL_ALGO.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(ENABLE_ALL_ALGO);

    HotKeyDetails DISABLE_ALL_ALGO;
    DISABLE_ALL_ALGO.action = "DISABLE_ALL_ALGO";
    DISABLE_ALL_ALGO.description = "Disable all Algos";
    DISABLE_ALL_ALGO.shortcut = "Shift+D";
    DISABLE_ALL_ALGO.enabled = true;
    Algo_TableHotkeys.HotKeyList.append(DISABLE_ALL_ALGO);


    HotKeysData_Grouped Trade_TableHotkeys;
    Trade_TableHotkeys.category = "Trade_Table";
    Trade_TableHotkeys.title = "Trade Table";

    HotKeyDetails OPEN_TRADE_DETAILS_WIN;
    OPEN_TRADE_DETAILS_WIN.action = "OPEN_TRADE_DETAILS_WIN";
    OPEN_TRADE_DETAILS_WIN.description = "Open BuildAlgo Window";
    OPEN_TRADE_DETAILS_WIN.shortcut = "Ctrl+D";
    OPEN_TRADE_DETAILS_WIN.enabled = true;
    Trade_TableHotkeys.HotKeyList.append(OPEN_TRADE_DETAILS_WIN);




    hotKeyDataList.append(GlobalHotkeys);
    hotKeyDataList.append(Algo_TableHotkeys);
    hotKeyDataList.append(Trade_TableHotkeys);


    // Check if the file exists
    QFile file(fileName);
    if (!file.exists()) {
        QSettings settings(fileName, QSettings::IniFormat);

        for(int i=0;i<hotKeyDataList.length();i++){

            for(int j=0;j<hotKeyDataList[i].HotKeyList.length();j++){
                settings.beginGroup(hotKeyDataList[i].HotKeyList[j].action);
                settings.setValue("Shortcut",hotKeyDataList[i].HotKeyList[j].shortcut);
                settings.setValue("Enabled", hotKeyDataList[i].HotKeyList[j].enabled);
                settings.endGroup();
            }
            // Save the settings to the file
            settings.sync();

        }

    }

    //load setting file and update kotkey data
        HotKeysHash.clear();
        QSettings settings(settingFile, QSettings::IniFormat);
         for (int i = 0; i < hotKeyDataList.size(); ++i) {
                HotKeysData_Grouped &category = hotKeyDataList[i];
                // Iterate over each HotKeyDetails in the category
                for (int j = 0; j < category.HotKeyList.size(); ++j) {
                    HotKeyDetails &hotkey = category.HotKeyList[j];
                    QString group = hotkey.action;  // Group name is the action name

                    // Check if the group exists in the ini file
                    if (settings.childGroups().contains(group)) {
                        settings.beginGroup(group);
                        // Read the "Shortcut" and "Enabled" values from the ini file
                        hotkey.shortcut = settings.value("Shortcut", hotkey.shortcut).toString(); // Use existing shortcut if not found
                        hotkey.enabled = settings.value("Enabled", hotkey.enabled).toBool(); // Use existing enabled status if not found
                        settings.endGroup();
                     }
                HotKeysHash.insert(group,hotkey);
        }

   }


}

void Settings_Window::loadHotkeyFromIniFileAndPopulateTreeView()
{

    model->clear();
    // Set headers for columns
    model->setHorizontalHeaderLabels({"Name", "Shortcut", "Enable/Disable"});
    ui->treeView->setColumnWidth(0, 250); // Set the width of the first column to 200 pixels


    QSettings settings(settingFile, QSettings::IniFormat);
     for (int i = 0; i < hotKeyDataList.size(); ++i) {
            HotKeysData_Grouped &category = hotKeyDataList[i];

            // Create a top-level item for the group (use title if available)
            QStandardItem *groupItem = new QStandardItem(category.title);
            groupItem->setEditable(false);
            groupItem->setBackground(QBrush(QColor(200, 200, 255))); // Set background color
            groupItem->setSizeHint(QSize(0, 30)); // Adds padding (height)
            groupItem->setFont(QFont("Arial", 10, QFont::Bold)); // Bold font

            // Iterate over each HotKeyDetails in the category
            for (int j = 0; j < category.HotKeyList.size(); ++j) {
                HotKeyDetails &hotkey = category.HotKeyList[j];
                QString group = hotkey.action;  // Group name is the action name

                // Check if the group exists in the ini file
                if (settings.childGroups().contains(group)) {
                    settings.beginGroup(group);
                    // Read the "Shortcut" and "Enabled" values from the ini file
                    hotkey.shortcut = settings.value("Shortcut", hotkey.shortcut).toString(); // Use existing shortcut if not found
                    hotkey.enabled = settings.value("Enabled", hotkey.enabled).toBool(); // Use existing enabled status if not found
                    settings.endGroup();
                }


                        bool enabled = hotkey.enabled == 1;
                        // Create child items for the group
                        QList<QStandardItem *> rowItems;
                        QStandardItem *descriptionItem = new QStandardItem(hotkey.description); // Shortcut
                        descriptionItem->setSizeHint(QSize(0, 25)); // Adds padding (height)
                        descriptionItem->setFont(QFont("Arial", 9, QFont::Normal)); // Bold font
                        descriptionItem->setEditable(false); // Allow editing of shortcut

                        rowItems.append(descriptionItem);

                        QStandardItem *shortcutItem = new QStandardItem(hotkey.shortcut); // Shortcut
                        shortcutItem->setSizeHint(QSize(0, 25)); // Adds padding (height)
                        shortcutItem->setFont(QFont("Arial", 9, QFont::Normal)); // Bold font
                        shortcutItem->setEditable(true); // Allow editing of shortcut
                        shortcutItem->setData(hotkey.action, Qt::UserRole); // Store the action as user data

                        rowItems.append(shortcutItem);

                        QStandardItem *enabledItem = new QStandardItem();
                        enabledItem->setSizeHint(QSize(0, 25)); // Adds padding (height)
                        enabledItem->setFont(QFont("Arial", 9, QFont::Normal)); // Bold font
                        enabledItem->setCheckable(true); // Checkbox
                        enabledItem->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
                        enabledItem->setData(hotkey.action, Qt::UserRole); // Store the action as user data
                        rowItems.append(enabledItem);

                        groupItem->appendRow(rowItems); // Append child row to group


            }

            // Add the group item to the model
            model->appendRow(groupItem);
    }


        // Expand all nodes in the tree view
        ui->treeView->expandAll();

}

void Settings_Window::keyPressEvent(QKeyEvent *event) {
        // Ignore the Escape key
        if (event->key() == Qt::Key_Escape) {
            event->ignore();  // Ignore the Escape key
        } else {
            QDialog::keyPressEvent(event);  // Process other keys normally
        }
    }

Settings_Window::~Settings_Window()
{
    delete ui;
}
void Settings_Window::treeviewHotKeyItemEdited(QStandardItem *item) {
    // Retrieve the action from user data
    QString action = item->data(Qt::UserRole).toString();

    if (!action.isEmpty()) {
        // Get the original hotkey from the hash
        HotKeyDetails originalHotkey = HotKeysHash[action];

        // Copy the original hotkey for modification
        HotKeyDetails modifiedHotkey = originalHotkey;

        // Check if the shortcut column has changed
        if (item->column() == 1) {
            modifiedHotkey.shortcut = item->text(); // Update shortcut
        }

        // Check if the enabled column has changed
        if (item->column() == 2) {
            modifiedHotkey.enabled = (item->checkState() == Qt::Checked); // Update enabled status
        }

        // If any changes were made, update the hash and the updatedHotKeys
        if (modifiedHotkey != originalHotkey) {
            HotKeysHash[action] = modifiedHotkey;   // Update in HotKeysHash
            updatedHotKeys[action] = modifiedHotkey; // Store in updatedHotKeys
        }
    }
}
void Settings_Window::on_pushButtonSaveHotKeys_clicked()
{
    QStringList hotKeys;
    QSettings settings(settingFile, QSettings::IniFormat);

        for (auto it = updatedHotKeys.begin(); it != updatedHotKeys.end(); ++it) {
            QString action = it.key();
            HotKeyDetails updatedDetails = it.value();

            // Begin group by action
            settings.beginGroup(action);
            settings.setValue("Shortcut", updatedDetails.shortcut);
            settings.setValue("Enabled", updatedDetails.enabled);
            settings.endGroup();

            hotKeys.append(action);
        }

        // Sync settings to write to the file
        settings.sync();

        // Optionally, clear updatedHotKeys after saving
        updatedHotKeys.clear();

     if(hotKeys.size()>0){
        emit updateHotKeysSignal(hotKeys);
        loadHotkeyFromIniFileAndPopulateTreeView();
     }

}

/**********************Font settings******************/

void Settings_Window::initFontSett(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString theme_fileName = appDataPath + "/Data/theme_sett.ini";

    // Read the saved font from the ini file
    QSettings settings(theme_fileName, QSettings::IniFormat);
    settings.beginGroup("font_sett");
    QString family = settings.value("Family", "Arial").toString();
    int pointSize = settings.value("PointSize", 12).toInt();
    bool bold = settings.value("Bold", false).toBool();
    bool italic = settings.value("Italic", false).toBool();
    settings.endGroup();

    QFont defaultFont(family, pointSize, bold ? QFont::Bold : QFont::Normal, italic);

    QString fontDescription = QString("%1, %2pt%3%4")
                                  .arg(defaultFont.family())
                                  .arg(defaultFont.pointSize())
                                  .arg(defaultFont.bold() ? ", Bold" : "")
                                  .arg(defaultFont.italic() ? ", Italic" : "");

    // Set the label text
    ui->label_font->setText(fontDescription);
}
void Settings_Window::on_pushButtonChangeFont_clicked()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString theme_fileName = appDataPath + "/Data/theme_sett.ini";

    // Read the saved font from the ini file
    QSettings settings(theme_fileName, QSettings::IniFormat);
    settings.beginGroup("font_sett");
    QString family = settings.value("Family", "Arial").toString();
    int pointSize = settings.value("PointSize", 12).toInt();
    bool bold = settings.value("Bold", false).toBool();
    bool italic = settings.value("Italic", false).toBool();
    settings.endGroup();

    QFont defaultFont(family, pointSize, bold ? QFont::Bold : QFont::Normal, italic);

    // Open font dialog with the loaded font as default
    bool ok;
    QFont font = QFontDialog::getFont(&ok, defaultFont, this);

    if (ok) {
        // Save the selected font back to the ini file
        settings.beginGroup("font_sett");
        settings.setValue("Family", font.family());
        settings.setValue("PointSize", font.pointSize());
        settings.setValue("Bold", font.bold());
        settings.setValue("Italic", font.italic());
        settings.endGroup();
        qDebug() << "Font saved:" << font;

        QString fontDescription = QString("%1, %2pt%3%4")
                                      .arg(font.family())
                                      .arg(font.pointSize())
                                      .arg(font.bold() ? ", Bold" : "")
                                      .arg(font.italic() ? ", Italic" : "");

        // Set the label text
        ui->label_font->setText(fontDescription);
    }
}
/*****************************************************/
