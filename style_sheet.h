#ifndef STYLE_SHEET_H
#define STYLE_SHEET_H

const char dock_style[] = "QDockWidget > QWidget{"
                          "border-bottom-right-radius: 12px;"
                          "border-bottom-left-radius: 12px;"
                          "border: 3px solid #F8F8F8;"
                          "box-shadow: 0px 1px 3px 0px #A8A8A8;"
                          "background: #FFF;"
                          "}"
                          "QDockWidget::title {"
                          "background-color: #F8F8F8;"
                          "border-top-left-radius: 12px;"
                          "border-top-right-radius: 12px;"
                          "padding: 4px;"
                          "}";
const char DockTitleBar_Style[]="background: #FFF;"
                                "border-top-left-radius :12px;"
                                "border-top-right-radius : 12px;"
                                "border-bottom-left-radius : 0px;"
                                "border-bottom-right-radius : 0px;";
const char lineedit_dock_SS[]="border-radius: 10px;"
                              "border: 1px solid #ADB5BD;"
                              "background: #FFF;";
const char scroll_bar_SS[]="QTableView {"
                             "border-bottom-left-radius : 12px;"
                             "border-bottom-right-radius : 12px;"
                             "border: 4px solid #DAE1E9;"
                             "}"
                            "QScrollBar:vertical {"
                             "    border: 1px solid #FFF;"
                             "    background:transparent;"
                             "    width:5px;    "
                             "    margin: 0px 0px 0px 0px;"
                             "}"
                             "QScrollBar::handle:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    min-height: 0px;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar::add-line:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                             "    height: 0px;"
                             "    subcontrol-position: bottom;"
                             "    subcontrol-origin: margin;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar::sub-line:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                             "    height: 0 px;"
                             "    subcontrol-position: top;"
                             "    subcontrol-origin: margin;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar:horizontal {"
                             "    border: 1px solid #FFF;"
                             "    background: transparent;"
                             "    height: 6px;    "
                             "    margin: 0px 0px 0px 0px;"
                             "}"
                             "QScrollBar::handle:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    min-width: 0px;"
                             "    border-radius: 2px;"
                             "}"
                             "QScrollBar::add-line:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    width: 0px;"
                             "    subcontrol-position: right;"
                             "    subcontrol-origin: margin;"
                             "    border-radius: 2px;"
                             "}"
                             "QScrollBar::sub-line:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    width: 0px;"
                             "    subcontrol-position: left;"
                             "    subcontrol-origin: margin;"
                             "    border-radius: 2px;"
                             "}";
const char tableview_SS[]=  "QHeaderView::section {"
                            "   background: #495867;"
                            "   border: none;"
                            "   color: #FFFFFF;"
                            "   text-align: center; "
                            "   font-size: 12px; "
                            "   font-style: normal; "
                            "   font-weight: 600; "
                            "   line-height: normal;"
                            "}"
                            "QTableView {"
                             "border-bottom-left-radius : 12px;"
                             "border-bottom-right-radius : 12px;"
                             "border: 4px solid #DAE1E9;"
                             "}"
                             "QScrollBar:vertical {"
                             "    border: 1px solid #FFF;"
                             "    background:transparent;"
                             "    width:5px;    "
                             "    margin: 0px 0px 0px 0px;"
                             "}"
                             "QScrollBar::handle:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    min-height: 0px;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar::add-line:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                             "    height: 0px;"
                             "    subcontrol-position: bottom;"
                             "    subcontrol-origin: margin;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar::sub-line:vertical {"
                             "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                             "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                             "    height: 0 px;"
                             "    subcontrol-position: top;"
                             "    subcontrol-origin: margin;"
                             "border-radius: 1px;"
                             "}"
                             "QScrollBar:horizontal {"
                             "    border: 1px solid #FFF;"
                             "    background: transparent;"
                             "    height: 6px;    "
                             "    margin: 0px 0px 0px 0px;"
                             "}"
                             "QScrollBar::handle:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    min-width: 0px;"
                             "    border-radius: 2px;"
                             "}"
                             "QScrollBar::add-line:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    width: 0px;"
                             "    subcontrol-position: right;"
                             "    subcontrol-origin: margin;"
                             "    border-radius: 2px;"
                             "}"
                             "QScrollBar::sub-line:horizontal {"
                             "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                             "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                             "    width: 0px;"
                             "    subcontrol-position: left;"
                             "    subcontrol-origin: margin;"
                             "    border-radius: 2px;"
                             "}";

#endif // STYLE_SHEET_H
