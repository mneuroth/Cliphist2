/********************************************************************************
** Form generated from reading ui file 'cliphistwindow.ui'
**
** Created: Wed Oct 14 23:43:38 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CLIPHISTWINDOW_H
#define UI_CLIPHISTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CliphistWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *action_Quit;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionDelete_all_entries;
    QAction *actionDelete_marked_entry;
    QAction *action_Find_text;
    QAction *actionFind_next;
    QAction *action_Edit_entry;
    QAction *action_Activate_cliphist;
    QAction *actionErase_clipboard_contents;
    QAction *action_Lines_per_entry;
    QAction *actionSelect_font;
    QAction *action_Double_click_to_select;
    QAction *actionAutoload_data;
    QAction *actionAutoload_window_position_and_size;
    QAction *actionAbout_Qt;
    QAction *actionAlways_on_top;
    QAction *actionMaximal_number_of_entries;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListWidget *listWidgetLineNumbers;
    QListWidget *listWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menu_Edit;
    QMenu *menuHelp;
    QMenu *menu_Options;

    void setupUi(QMainWindow *CliphistWindow)
    {
        if (CliphistWindow->objectName().isEmpty())
            CliphistWindow->setObjectName(QString::fromUtf8("CliphistWindow"));
        CliphistWindow->resize(600, 400);
        actionOpen = new QAction(CliphistWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(CliphistWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_as = new QAction(CliphistWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        action_Quit = new QAction(CliphistWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionHelp = new QAction(CliphistWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(CliphistWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionDelete_all_entries = new QAction(CliphistWindow);
        actionDelete_all_entries->setObjectName(QString::fromUtf8("actionDelete_all_entries"));
        actionDelete_marked_entry = new QAction(CliphistWindow);
        actionDelete_marked_entry->setObjectName(QString::fromUtf8("actionDelete_marked_entry"));
        action_Find_text = new QAction(CliphistWindow);
        action_Find_text->setObjectName(QString::fromUtf8("action_Find_text"));
        actionFind_next = new QAction(CliphistWindow);
        actionFind_next->setObjectName(QString::fromUtf8("actionFind_next"));
        action_Edit_entry = new QAction(CliphistWindow);
        action_Edit_entry->setObjectName(QString::fromUtf8("action_Edit_entry"));
        action_Activate_cliphist = new QAction(CliphistWindow);
        action_Activate_cliphist->setObjectName(QString::fromUtf8("action_Activate_cliphist"));
        action_Activate_cliphist->setCheckable(true);
        action_Activate_cliphist->setChecked(true);
        actionErase_clipboard_contents = new QAction(CliphistWindow);
        actionErase_clipboard_contents->setObjectName(QString::fromUtf8("actionErase_clipboard_contents"));
        action_Lines_per_entry = new QAction(CliphistWindow);
        action_Lines_per_entry->setObjectName(QString::fromUtf8("action_Lines_per_entry"));
        actionSelect_font = new QAction(CliphistWindow);
        actionSelect_font->setObjectName(QString::fromUtf8("actionSelect_font"));
        action_Double_click_to_select = new QAction(CliphistWindow);
        action_Double_click_to_select->setObjectName(QString::fromUtf8("action_Double_click_to_select"));
        action_Double_click_to_select->setCheckable(true);
        action_Double_click_to_select->setChecked(true);
        actionAutoload_data = new QAction(CliphistWindow);
        actionAutoload_data->setObjectName(QString::fromUtf8("actionAutoload_data"));
        actionAutoload_data->setCheckable(true);
        actionAutoload_data->setChecked(true);
        actionAutoload_window_position_and_size = new QAction(CliphistWindow);
        actionAutoload_window_position_and_size->setObjectName(QString::fromUtf8("actionAutoload_window_position_and_size"));
        actionAutoload_window_position_and_size->setCheckable(true);
        actionAutoload_window_position_and_size->setChecked(true);
        actionAbout_Qt = new QAction(CliphistWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionAlways_on_top = new QAction(CliphistWindow);
        actionAlways_on_top->setObjectName(QString::fromUtf8("actionAlways_on_top"));
        actionAlways_on_top->setCheckable(true);
        actionAlways_on_top->setChecked(false);
        actionMaximal_number_of_entries = new QAction(CliphistWindow);
        actionMaximal_number_of_entries->setObjectName(QString::fromUtf8("actionMaximal_number_of_entries"));
        centralWidget = new QWidget(CliphistWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(2);
        gridLayout->setMargin(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidgetLineNumbers = new QListWidget(centralWidget);
        listWidgetLineNumbers->setObjectName(QString::fromUtf8("listWidgetLineNumbers"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidgetLineNumbers->sizePolicy().hasHeightForWidth());
        listWidgetLineNumbers->setSizePolicy(sizePolicy);
        listWidgetLineNumbers->setMaximumSize(QSize(40, 16777215));
        listWidgetLineNumbers->setFocusPolicy(Qt::NoFocus);
        listWidgetLineNumbers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidgetLineNumbers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidgetLineNumbers->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidgetLineNumbers->setSelectionMode(QAbstractItemView::NoSelection);

        gridLayout->addWidget(listWidgetLineNumbers, 0, 0, 1, 1);

        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget->setEditTriggers(QAbstractItemView::EditKeyPressed);

        gridLayout->addWidget(listWidget, 0, 1, 1, 1);

        CliphistWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CliphistWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menu_Options = new QMenu(menuBar);
        menu_Options->setObjectName(QString::fromUtf8("menu_Options"));
        CliphistWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_Options->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(action_Quit);
        menu_Edit->addAction(actionDelete_all_entries);
        menu_Edit->addAction(actionDelete_marked_entry);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Find_text);
        menu_Edit->addAction(actionFind_next);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Edit_entry);
        menuHelp->addAction(actionHelp);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        menu_Options->addAction(action_Activate_cliphist);
        menu_Options->addAction(actionErase_clipboard_contents);
        menu_Options->addSeparator();
        menu_Options->addAction(actionMaximal_number_of_entries);
        menu_Options->addAction(action_Lines_per_entry);
        menu_Options->addAction(actionSelect_font);
        menu_Options->addSeparator();
        menu_Options->addAction(action_Double_click_to_select);
        menu_Options->addAction(actionAutoload_data);
        menu_Options->addAction(actionAutoload_window_position_and_size);
        menu_Options->addAction(actionAlways_on_top);

        retranslateUi(CliphistWindow);

        QMetaObject::connectSlotsByName(CliphistWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CliphistWindow)
    {
        CliphistWindow->setWindowTitle(QApplication::translate("CliphistWindow", "Clipboard History 2", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("CliphistWindow", "&Load Data...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("CliphistWindow", "&Save Data", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("CliphistWindow", "Save Data &as...", 0, QApplication::UnicodeUTF8));
        actionSave_as->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("CliphistWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        action_Quit->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("CliphistWindow", "&Help...", 0, QApplication::UnicodeUTF8));
        actionHelp->setShortcut(QApplication::translate("CliphistWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("CliphistWindow", "&About...", 0, QApplication::UnicodeUTF8));
        actionDelete_all_entries->setText(QApplication::translate("CliphistWindow", "Delete &all entries...", 0, QApplication::UnicodeUTF8));
        actionDelete_marked_entry->setText(QApplication::translate("CliphistWindow", "&Delete marked entry", 0, QApplication::UnicodeUTF8));
        actionDelete_marked_entry->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        action_Find_text->setText(QApplication::translate("CliphistWindow", "&Find text...", 0, QApplication::UnicodeUTF8));
        action_Find_text->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionFind_next->setText(QApplication::translate("CliphistWindow", "Find &next", 0, QApplication::UnicodeUTF8));
        actionFind_next->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        action_Edit_entry->setText(QApplication::translate("CliphistWindow", "&Edit entry...", 0, QApplication::UnicodeUTF8));
        action_Edit_entry->setShortcut(QApplication::translate("CliphistWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        action_Activate_cliphist->setText(QApplication::translate("CliphistWindow", "&Activate cliphist", 0, QApplication::UnicodeUTF8));
        actionErase_clipboard_contents->setText(QApplication::translate("CliphistWindow", "&Erase clipboard contents", 0, QApplication::UnicodeUTF8));
        action_Lines_per_entry->setText(QApplication::translate("CliphistWindow", "&Lines per entry...", 0, QApplication::UnicodeUTF8));
        actionSelect_font->setText(QApplication::translate("CliphistWindow", "Select &font...", 0, QApplication::UnicodeUTF8));
        action_Double_click_to_select->setText(QApplication::translate("CliphistWindow", "&Double click to select", 0, QApplication::UnicodeUTF8));
        actionAutoload_data->setText(QApplication::translate("CliphistWindow", "&Autoload data", 0, QApplication::UnicodeUTF8));
        actionAutoload_window_position_and_size->setText(QApplication::translate("CliphistWindow", "Autoload &window position and size", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("CliphistWindow", "About &Qt...", 0, QApplication::UnicodeUTF8));
        actionAlways_on_top->setText(QApplication::translate("CliphistWindow", "Always on &top", 0, QApplication::UnicodeUTF8));
        actionMaximal_number_of_entries->setText(QApplication::translate("CliphistWindow", "Ma&ximal number of entries...", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("CliphistWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("CliphistWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("CliphistWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menu_Options->setTitle(QApplication::translate("CliphistWindow", "&Options", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CliphistWindow: public Ui_CliphistWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIPHISTWINDOW_H
