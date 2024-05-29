#include "convert_algo_shared_var.h"
#include "QSettings"
#include "QDir"
#include <QStandardPaths>

AddAlgoSharedVar& AddAlgoSharedVar::getInstance() {
    static AddAlgoSharedVar instance;
    return instance;
}

AddAlgoSharedVar::AddAlgoSharedVar() {
    // Initialize variables if needed
    unique_id = 0;
    ROW_LIMIT = 100;

    //currenntly hardcoded for FO
    strike_price_devider = FO_DEVICER;
    /*QString market_type = "fo";
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
        settings.endGroup();
    }
    if(market_type=="fo"){
        strike_price_devider = FO_DEVICER;
    }
    else{
        strike_price_devider = CDS_DEVICER;
    }*/


}

