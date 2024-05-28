QT       += core gui
QT+=sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_MESSAGELOGCONTEXT
# To disable log uncomment the respetive line for debug, info and warning
#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_INFO_OUTPUT
#DEFINES += QT_NO_WARNING_OUTPUT

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CombinedTracker/combined_tracker_table_model.cpp \
    Conversions.cpp \
    Convert_to_Algo/add_algo_con_rev.cpp \
    Convert_to_Algo/add_algo_f2f.cpp \
    Convert_to_Algo/convert_algo_shared_var.cpp \
    Convert_to_Algo/convert_algo_win.cpp \
    Convert_to_Algo/convert_to_algo_bfly.cpp \
    Convert_to_Algo/convert_to_algo_bfly_bid.cpp \
    Convert_to_Algo/eventfilterlistview.cpp \
    Liners/liners_model.cpp \
    NetPosition/net_position_table_model.cpp \
    OrderBook/table_orderbook_delegate.cpp \
    PortFolio/PortfolioParser.cpp \
    PortFolio/portfolio_Object.cpp \
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
    backendcomm/backendcomm.cpp \
    contractdetail.cpp \
    customDelegate.cpp \
    custom_grid_table/gridtableheadermodel.cpp \
    custom_grid_table/gridtableheaderview.cpp \
    custom_grid_table/gridtableview.cpp \
    custom_grid_table/tableheaderitem.cpp \
    custom_q_completer.cpp \
    logger/logger.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mysql_conn.cpp \
    uidockwidget.cpp \
    watch_data_list_item.cpp

HEADERS += \
    CombinedTracker/combined_tracker_table_model.h \
    Common/macros.h \
    Conversions.h \
    Convert_to_Algo/add_algo_con_rev.h \
    Convert_to_Algo/add_algo_f2f.h \
    Convert_to_Algo/convert_algo_shared_var.h \
    Convert_to_Algo/convert_algo_win.h \
    Convert_to_Algo/convert_to_algo_bfly.h \
    Convert_to_Algo/convert_to_algo_bfly_bid.h \
    Convert_to_Algo/eventfilterlistview.h \
    Liners/liners_model.h \
    NetPosition/net_position_table_model.h \
    OrderBook/table_orderbook_delegate.h \
    PortFolio/PortfolioParser.h \
    PortFolio/portfolio_Object.h \
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
    backendcomm/backendcomm.h \
    contractdetail.h \
    customDelegate.h \
    custom_grid_table/gridtableheadermodel.h \
    custom_grid_table/gridtableheaderview.h \
    custom_grid_table/gridtableview.h \
    custom_grid_table/tableheaderitem.h \
    custom_q_completer.h \
    defines.h \
    logger/logger.h \
    loginwindow.h \
    mainwindow.h \
    mysql_conn.h \
    uidockwidget.h \
    watch_data_list_item.h

FORMS += \
    convert_algo_win.ui \
    loginwindow.ui \
    mainwindow.ui \
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
    Resources/left_arrow.png \
    Resources/maximize_window_icon.png \
    Resources/menu.png \
    Resources/minimize_window_group.png \
    Resources/minimize_window_icon.png \
    Resources/minimize_window_icon_background.png \
    Resources/right_arrow.png \
    Resources/search.ico \
    Resources/search.png \
    Resources/test.png

RC_ICONS = Resources/logo.ico

