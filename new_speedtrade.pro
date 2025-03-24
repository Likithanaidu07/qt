QT  += core gui
QT+=sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_MESSAGELOGCONTEXT
# To disable log uncomment the respetive line for debug, info and warning
#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_INFO_OUTPUT
#DEFINES += QT_NO_WARNING_OUTPUT

CONFIG += c++17

#manually Increment Version Before Build
VERSION = 1.0.31
TARGET = New_Speedtrade_$${VERSION}

version_script = $$PWD/increment_version.py
QMAKE_PRE_LINK = python $$version_script

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Cards/logs_cards.cpp \
    Cards/summary_cards.cpp \
    Cards/watch_cards.cpp \
    CombinedTracker/combined_tracker_table_model.cpp \
    Conversions.cpp \
    Convert_to_Algo/add_algo_con_rev.cpp \
    Convert_to_Algo/add_algo_cr_jelly_bid.cpp \
    Convert_to_Algo/add_algo_f2f.cpp \
    Convert_to_Algo/convert_algo_shared_var.cpp \
    Convert_to_Algo/convert_algo_win.cpp \
    Convert_to_Algo/convert_to_algo_bfly.cpp \
    Convert_to_Algo/convert_to_algo_bfly_bid.cpp \
    Convert_to_Algo/convert_to_algo_box_bid.cpp \
    Convert_to_Algo/convert_to_algo_bs1221_1331.cpp \
    Convert_to_Algo/convert_to_algo_open_2l.cpp \
    Convert_to_Algo/customtablewidget.cpp \
    Convert_to_Algo/eventfilterlistview.cpp \
    F1_F2/f1_f2_buysell.cpp \
    Liners/liners_delegate.cpp \
    Liners/liners_model.cpp \
    Liners/linersheaderview.cpp \
    MissedTrades/missed_trade_table_delegate.cpp \
    MissedTrades/missed_trade_table_model.cpp \
    MissedTrades/missed_trades_headerview.cpp \
    NetPosition/net_position_table_delegate.cpp \
    NetPosition/net_position_table_headerview.cpp \
    NetPosition/net_position_table_model.cpp \
    NetPosition/netpos_searchfilterproxymodel.cpp \
    OpenPosition/open_position_delegate.cpp \
    OpenPosition/open_position_headerview.cpp \
    OpenPosition/open_position_model.cpp \
    OrderBook/table_orderbook_delegate.cpp \
    Order_F1_F2/order_f1_f2_delegate.cpp \
    Order_F1_F2/order_f1_f2_model.cpp \
    Order_F1_F2/order_f1_f2_tableheaderview.cpp \
    PortFolio/PortfolioParser.cpp \
    PortFolio/portfolio_Object.cpp \
    PortFolio/portfolio_searchfilterproxymodel.cpp \
    PortFolio/portfoliocustomsorting.cpp \
    PortFolio/portfolioheaderdelegate.cpp \
    PortFolio/portfolioheaderview.cpp \
    PortFolio/table_portfolios_custom.cpp \
    PortFolio/table_portfolios_delegate.cpp \
    PortFolio/table_portfolios_model.cpp \
    SlowData/slowdata.cpp \
    SlowData/slowdatasocket.cpp \
    SlowDataIndices/slowdata_indices_socket.cpp \
    SlowDataIndices/slowdataindices.cpp \
    SwitchButton/switchbutton.cpp \
    TradePosition/trade_table_model.cpp \
    TradePosition/tradetable_searchfilterproxymodel.cpp \
    TradePosition/tradetableheaderview.cpp \
    backendcomm/backendcomm.cpp \
    cache_file_io.cpp \
    contractdetail.cpp \
    customDelegate.cpp \
    custom_grid_table/gridtableheadermodel.cpp \
    custom_grid_table/gridtableheaderview.cpp \
    custom_grid_table/gridtableview.cpp \
    custom_grid_table/tableheaderitem.cpp \
    custom_q_completer.cpp \
    loadingdatawindow.cpp \
    logger/logger.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mysql_conn.cpp \
    order_detail_popup.cpp \
    settings_window.cpp \
    sortsettingpopup.cpp \
    trade_details_popup.cpp \
    uidockwidget.cpp \
    watch_data_card.cpp \
    watch_data_list_item.cpp

HEADERS += \
    Cards/logs_cards.h \
    Cards/summary_cards.h \
    Cards/watch_cards.h \
    CombinedTracker/combined_tracker_table_model.h \
    Common/macros.h \
    Conversions.h \
    Convert_to_Algo/add_algo_con_rev.h \
    Convert_to_Algo/add_algo_cr_jelly_bid.h \
    Convert_to_Algo/add_algo_f2f.h \
    Convert_to_Algo/convert_algo_shared_var.h \
    Convert_to_Algo/convert_algo_win.h \
    Convert_to_Algo/convert_to_algo_bfly.h \
    Convert_to_Algo/convert_to_algo_bfly_bid.h \
    Convert_to_Algo/convert_to_algo_box_bid.h \
    Convert_to_Algo/convert_to_algo_bs1221_1331.h \
    Convert_to_Algo/convert_to_algo_open_2l.h \
    Convert_to_Algo/customtablewidget.h \
    Convert_to_Algo/eventfilterlistview.h \
    F1_F2/f1_f2_buysell.h \
    Liners/liners_delegate.h \
    Liners/liners_model.h \
    Liners/linersheaderview.h \
    MissedTrades/missed_trade_table_delegate.h \
    MissedTrades/missed_trade_table_model.h \
    MissedTrades/missed_trades_headerview.h \
    NetPosition/net_position_table_delegate.h \
    NetPosition/net_position_table_headerview.h \
    NetPosition/net_position_table_model.h \
    NetPosition/netpos_searchfilterproxymodel.h \
    OpenPosition/open_position_delegate.h \
    OpenPosition/open_position_headerview.h \
    OpenPosition/open_position_model.h \
    OrderBook/table_orderbook_delegate.h \
    Order_F1_F2/order_f1_f2_delegate.h \
    Order_F1_F2/order_f1_f2_model.h \
    Order_F1_F2/order_f1_f2_tableheaderview.h \
    PortFolio/PortfolioParser.h \
    PortFolio/portfolio_Object.h \
    PortFolio/portfolio_searchfilterproxymodel.h \
    PortFolio/portfoliocustomsorting.h \
    PortFolio/portfolioheaderdelegate.h \
    PortFolio/portfolioheaderview.h \
    PortFolio/table_portfolios_custom.h \
    PortFolio/table_portfolios_delegate.h \
    PortFolio/table_portfolios_model.h \
    SlowData/Indices.h \
    SlowData/slowdata.h \
    SlowData/slowdatasocket.h \
    SlowDataIndices/slowdata_indices_socket.h \
    SlowDataIndices/slowdataindices.h \
    SwitchButton/switchbutton.h \
    TradePosition/OrderBook_Object.h \
    TradePosition/trade_table_model.h \
    TradePosition/tradetable_searchfilterproxymodel.h \
    TradePosition/tradetableheaderview.h \
    backendcomm/backendcomm.h \
    cache_file_io.h \
    contractdetail.h \
    customDelegate.h \
    custom_double_SpinBox.h \
    custom_grid_table/gridtableheadermodel.h \
    custom_grid_table/gridtableheaderview.h \
    custom_grid_table/gridtableview.h \
    custom_grid_table/tableheaderitem.h \
    custom_q_completer.h \
    defines.h \
    loadingdatawindow.h \
    logger/logger.h \
    loginwindow.h \
    mainwindow.h \
    mysql_conn.h \
    order_detail_popup.h \
    settings_window.h \
    sortsettingpopup.h \
    style_sheet.h \
    trade_details_popup.h \
    uidockwidget.h \
    watch_data_card.h \
    watch_data_list_item.h

FORMS += \
    Cards/logs_cards.ui \
    Cards/summary_cards.ui \
    Cards/watch_cards.ui \
    F1_F2/f1_f2_buysell.ui \
    convert_algo_win.ui \
    loadingdatawindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    order_detail_popup.ui \
    settings_window.ui \
    sortsettingpopup.ui \
    trade_details_popup.ui \
    watch_data_card.ui \
    watch_data_list_item.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Res.qrc

LIBS += -lws2_32
LIBS += -L"$$_PRO_FILE_PWD_/lzo-2.10" -llzo2

INCLUDEPATH += ./lzo-2.10/include/
INCLUDEPATH += ./lzo-2.10/



ADS_OUT_ROOT = $${OUT_PWD}/Qt-Advanced-Docking-System
LIBS += -L$${ADS_OUT_ROOT}/lib
include($${ADS_OUT_ROOT}/ads.pri)
INCLUDEPATH += $${ADS_OUT_ROOT}/src
DEPENDPATH += $${ADS_OUT_ROOT}/src


DISTFILES += \
    Res/Style_Login.qss \
    Resources/ALGO METHODS.png \
    Resources/Am Logomark - Standard.png \
    Resources/RacingSansOne-Regular.ttf \
    Resources/Vector.png \
    Resources/WorkSans-Bold.ttf \
    Resources/WorkSans-ExtraBold.ttf \
    Resources/WorkSans-Medium.ttf \
    Resources/button_close.png \
    Resources/close_window_icon.png \
    Resources/export.png \
    Resources/import.png \
    Resources/left_arrow.png \
    Resources/maximize_window_icon.png \
    Resources/menu.png \
    Resources/minimize_window_group.png \
    Resources/minimize_window_icon.png \
    Resources/minimize_window_icon_background.png \
    Resources/reset.png \
    Resources/right_arrow.png \
    Resources/search.ico \
    Resources/search.png \
    Resources/start_all.png \
    Resources/stop_all.png \
    Resources/test.png

RC_ICONS = Resources/logo.ico

