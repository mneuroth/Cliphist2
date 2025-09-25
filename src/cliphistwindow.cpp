/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2012 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				   *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				   *
 * Copyright (C) 2009-2012 by Michael Neuroth.								       *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify		   *
 * it under the terms of the GNU General Public License as published by    	   *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/
/*
    Initial Version for OS/2:         1994
    Ported to Windows XP:             2001
    Started porting to Qt:       12.9.2009    (Mac, Linux, Windows)

    Translation:
      *.pro file must contain something like: TRANSLATIONS = cliphist_de.ts
      lupdate cliphist2.pro    *.cpp --> *.ts
      linguist cliphist_de.ts
      lrelease cliphist2.pro   *.ts  --> *.qm
      make (*.qm is contained in executable file via qrc)

      copy /Developer/Applications/Qt/translations/qt_de.qm to working directory

    build on Mac OS X:
      qmake -spec macx-g++ cliphist2.pro [CONFIG+=release]
      make
      #cp *.qm Cliphist2.app/Contents/Resources/
      macdeployqt cliphist2.app -dmg

    for xcode:
      qmake -spec macx-xcode cliphist2.pro

    Used infos about making distributions from texmaker application (http://www.xm1math.net/texmaker/)

    Other resources:

    Qt4 cross platform toolkit

    For the icon:
      http://commons.wikimedia.org/wiki/File:Nuvola_clipboard_unlined.svg
      http://commons.wikimedia.org/wiki/File:Stock_alarm.svg

    For the windows installer:
      http://www.jrsoftware.org/isinfo.php      (Inno Setup)

    Windows installer:
      remove dependencies to other DLLs:
      - mingwm10.dll ==> http://blog.lugru.com/2009/03/qt-static-mingwm10dll-and-deployment-under-windows-environment/
                     ==> http://www.qtforum.org/article/12863/qt4-for-win-and-mingw10-dll-dependency.html
      - libgcc_s_dw2-1.dll ==> -static-libgcc ==> http://sourceforge.net/project/shownotes.php?release_id=691876

      build Qt from source:
        - modify in notepad %qtdir%\mkspecs\win32-g++\qmake.conf --> add -static
            --> QMAKE_LFLAGS = -static -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
        >configure -static -release -no-exceptions (ggf. -webkit)

      reduce size of exe file under Windows:
      - upx -9 cliphist2.exe

    Mac dmg:
      >. qt473static.sh
      >./make_dist_mac.sh

    Debian package:
      http://www.qtwiki.de/wiki/Der_Weg_vom_fertigen_QT_-_Programm_zum_Debian_Paket
      http://meetings-archive.debian.net/pub/debian-meetings/2009/fosdem/slides/The_Common_Debian_Build_System_CDBS/cdbs-presentation.pdf
      http://standards.freedesktop.org/menu-spec/latest/apa.html
      http://www.debian-administration.org/articles/336

      dh_make -s -c gpl -e michael.neuroth@freenet.de -f cliphist2-1.0.0.tar.gz
      cp rules debian/rules
      ggf. cp copyright debian/copyright
      ggf. cp control debian/control
      dpkg-buildpackage -rfakeroot -b
      lintian cliphist2-1.0.0.deb # zur Ueberpruefung
      Anpassungen in cliphist.pro -> target.path +=  /usr/bin/      und     INSTALLS += target
      Anpassungen in debian/rules (siehe erste doku)

    RPM package:
      http://en.opensuse.org/Packaging/SUSE_Package_Conventions/RPM_Style
      http://tldp.org/HOWTO/RPM-HOWTO/build.html
      http://en.opensuse.org/Packaging/SUSE_Package_Conventions/RPM_Style
      http://www.rpm.org/max-rpm/ch-rpm-b-command.html

      http://forums.opensuse.org/applications/420946-howto-install-virtualbox-guest-additions-opensuse-11-1-a.html#post2060879

      cd ..
      tar -cf cliphist2.tar cliphist2/ *
      gzip cliphist2.tar
      create cliphist2.spec

      rpmbuild -bb cliphist2.spec

*/

#include "cliphistwindow.h"
#include "ui_cliphistwindow.h"

#include "edititem.h"
#include "editmemo.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDataStream>
#include <QClipboard>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QScrollBar>
#include <QFont>
#include <QFontMetrics>
#include <QUndoStack>
#include <QUndoCommand>
#include <QTimer>
#include <QCryptographicHash>
#include <QMimeData>
#include <QBuffer>
#include <QCursor>

//#include <QDebug>

// ************************************************************************

#ifndef VERSION
#define VERSION                     "1.1.3"
#endif
#define TITLE                       "<a href=http://www.mneuroth.de/projects/Cliphist2.html>Clipboard History 2</a>"
#define HOMEPAGE                    "<a href=http://www.mneuroth.de/projects/Cliphist2.html>Homepage</a>"
#define LICENSE                     "<a href=http://www.fsf.org/licensing/licenses/gpl.html>GPL</a>"
#define AUTHORS                     "Michael Neuroth"

#define FILE_VERSION                2   // 1 until 9.2.2012
#define EXTENSIONS                  "*.clp"
#define DEFAULT_FILE_NAME           "cliphist2.clp"
#define DEFAULT_IMAGE_FILE_NAME     "cliphist_image.png"
#define DEFAULT_LINES_PER_ENTRY     3
#define DEFAULT_MAX_ENTRIES         100
#define TIMER_DELAY                 300 // ms
#define AUTOSAVE_TIMER_DELAY        60000 // ms
#define BLUE                        "blue"  //"#0000ff"   //"blue"
#define GREEN                       "green" //"#008000"   //"green"
#define RED                         "red"   //"#ff0000"   //"red"

#define IMAGE_TAG                   ":::IMAGE:::_"

#define _DEFAULT_TEXT               "XXX"
#define _UNSAVED_MARKER             " *"

#if defined(Q_OS_MAC)
#define DEFAULT_VALUE_USE_TIMER true
#else
#define DEFAULT_VALUE_USE_TIMER false
#endif

// ************************************************************************
static QByteArray GetHashForImage(const QPixmap & aImg)
{
    QCryptographicHash aHash( QCryptographicHash::Md5 );
    QImage aTempImg = aImg.toImage();
#if QT_VERSION >= 0x060000
    QByteArrayView view(reinterpret_cast<const char*>(aTempImg.bits()), aTempImg.sizeInBytes());
    aHash.addData(view);
    //aHash.addData((const char *)aTempImg.bits(),aTempImg.sizeInBytes());
#else
    aHash.addData((const char *)aTempImg.bits(),aTempImg.byteCount());
#endif
    return aHash.result();
}

// ************************************************************************
class UpdateCommand : public QUndoCommand
{
public:
     UpdateCommand(CliphistWindow * pApp, int iPosition, const QString & sText, const QString & sOldText, QUndoCommand *parent = 0)
         : QUndoCommand(parent),
           m_pApp(pApp),
           m_iPosition(iPosition),
           m_sText(sText),
           m_sOldText(sOldText)
     {}

     void undo()
     {
         m_iPosition = m_pApp->UpdateList(m_iPosition,m_sOldText);
     }
     void redo()
     {
         // verify if operation was successfully
         m_iPosition = m_pApp->UpdateList(m_iPosition,m_sText);
     }

private:
     CliphistWindow *   m_pApp;
     int                m_iPosition;
     QString            m_sText;
     QString            m_sOldText;
};

// ************************************************************************
class InsertCommand : public QUndoCommand
{
public:
     InsertCommand(CliphistWindow * pApp, int iPosition, const QString & sText, bool bUpdateSelection=false, QUndoCommand *parent = 0)
         : QUndoCommand(parent),
           m_pApp(pApp),
           m_iPosition(iPosition),
           m_sText(sText),
           m_bUpdateSelection(bUpdateSelection)
     {}

     void undo()
     {
         /*m_sText =*/ m_pApp->RemoveGiven(m_iPosition);
     }
     void redo()
     {
         // verify if operation was successfully
         m_iPosition = m_pApp->InsertInList(m_iPosition,m_sText,m_bUpdateSelection);
     }

private:
     CliphistWindow *   m_pApp;
     int                m_iPosition;
     QString            m_sText;
     bool               m_bUpdateSelection;
};

// ************************************************************************
class DeleteItemCommand : public QUndoCommand
 {
 public:
     DeleteItemCommand(CliphistWindow * pApp, const QList<int> & aActSelected, QUndoCommand *parent = 0)
         : QUndoCommand(parent),
           m_pApp(pApp),
           m_aActSelected(aActSelected)
     {}

     void undo()
     {
         for( int i=0; i<m_aActSelected.size(); i++ )
         {
             m_pApp->InsertInList(m_aActSelected[i],m_aText[m_aActSelected.size()-i-1]);
         }
     }
     void redo()
     {
         for( int i=m_aActSelected.size()-1; i>=0; i-- )
         {
             m_aText.append(m_pApp->RemoveGiven(m_aActSelected[i]));
         }
     }

private:
     CliphistWindow *   m_pApp;
     QList<int>         m_aActSelected;
     QStringList        m_aText;
};

// ************************************************************************
class ActivateItemCommand : public QUndoCommand
 {
 public:
     ActivateItemCommand(CliphistWindow * pApp, int iActivateNo, int iOldActivateNo, QUndoCommand *parent = 0)
         : QUndoCommand(parent),
           m_pApp(pApp),
           m_iActivateNo(iActivateNo),
           m_iOldActivateNo(iOldActivateNo)
     {}

     void undo()
     {
         m_pApp->ActivateItemIdx(m_iOldActivateNo);
     }
     void redo()
     {
         m_pApp->ActivateItemIdx(m_iActivateNo);
     }

private:
     CliphistWindow *   m_pApp;
     int                m_iActivateNo;
     int                m_iOldActivateNo;
};

// ************************************************************************
class DeleteAllCommand : public QUndoCommand
{
public:
     DeleteAllCommand(CliphistWindow * pApp, const QStringList & aHistory, const QList<int> & aActSelected, QUndoCommand *parent = 0)
         : QUndoCommand(parent),
           m_pApp(pApp),
           m_aHistory(aHistory),
           m_aActSelected(aActSelected)
     {}

     void undo()
     {
         m_pApp->UndoDeleteAllItems(m_aHistory,m_aActSelected);
     }
     void redo()
     {
         m_pApp->DoDeleteAllItems();
     }

private:
     CliphistWindow *   m_pApp;
     QStringList        m_aHistory;
     QList<int>         m_aActSelected;
};

// ************************************************************************

CliphistWindow::CliphistWindow(bool bIsSelfTest, const QString sFileName, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::CliphistWindow)
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    
    QPixmap aIcon(":/icons/cliphist2_512x512.png");
    setWindowIcon(aIcon);

    //QCoreApplication::setOrganizationName("mneuroth");
    QCoreApplication::setOrganizationDomain("mneuroth.de");
    QCoreApplication::setApplicationName("cliphist2");
    
    m_pUndoStack        = new QUndoStack();

    m_bSelfTest         = bIsSelfTest;
    m_pClipboard        = QApplication::clipboard();    
    m_iFindIndex        = -1;
    m_bChangedData      = false;
    m_bMyClipboardCopy  = false;
    m_bIfFoundMoveToFirstPos   = false;
    m_iActivatedIndex   = -1;
    m_iMaxEntries       = DEFAULT_MAX_ENTRIES;
    m_iMaxLinesPerEntry = DEFAULT_LINES_PER_ENTRY;
    //m_sLastImageSavePath= QCoreApplication::applicationDirPath()+QDir::separator()+DEFAULT_IMAGE_FILE_NAME;
    m_sLastImageSavePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+QDir::separator()+DEFAULT_IMAGE_FILE_NAME;
#if defined( Q_WS_MACX )
    m_sFileName         = QCoreApplication::applicationDirPath()+QDir::separator()+".."+QDir::separator()+"Resources"+QDir::separator()+QString(DEFAULT_FILE_NAME);
#else
    m_sFileName         = QCoreApplication::applicationDirPath()+QDir::separator()+QString(DEFAULT_FILE_NAME);
    //m_sFileName         = QDir::homePath()+QDir::separator()+QString(DEFAULT_FILE_NAME);
#endif
    m_sOriginalWindowTitle = windowTitle();

    LoadSettings();
    if( !(sFileName.isNull() || sFileName.isEmpty()) )
    {
        m_sFileName = sFileName;
    }

    OnToggleAlwaysOnTop(ui->actionAlways_on_top->isChecked());

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnItemClicked(QListWidgetItem *)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnItemDoubleClicked(QListWidgetItem *)));
    connect(ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(OnItemActivated(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
    
    // synchronize the two vertical scrollbars of the two list widgets:
    connect(ui->listWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->listWidgetLineNumbers->verticalScrollBar(), SLOT(setValue(int)) );
    connect(ui->listWidgetLineNumbers->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->listWidget->verticalScrollBar(), SLOT(setValue(int)) );

    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OnLoadData()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(OnSaveData()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(OnSaveDataAs()));

    // handle undo/redo
    QAction * redoAction = m_pUndoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui->menu_Edit->insertAction(ui->actionDelete_all_entries,redoAction);;
    QAction * undoAction = m_pUndoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    ui->menu_Edit->insertAction(redoAction,undoAction);
    ui->menu_Edit->insertSeparator(ui->actionDelete_all_entries);

    connect(ui->actionDelete_all_entries, SIGNAL(triggered()), this, SLOT(OnDeleteAllItems()));
    connect(ui->actionDelete_marked_entry, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));
    connect(this, SIGNAL(SelectionChanged(bool)), ui->actionDelete_marked_entry, SLOT(setEnabled(bool)));
    connect(ui->actionA_dd_marked_to_new_entry, SIGNAL(triggered()), this, SLOT(OnAddSelectedToNewEntry()));
    connect(this, SIGNAL(MoreThanOneSelected(bool)), ui->actionA_dd_marked_to_new_entry, SLOT(setEnabled(bool)));
    connect(ui->action_Find_text, SIGNAL(triggered()), this, SLOT(OnFindItem()));
    connect(ui->actionFind_next, SIGNAL(triggered()), this, SLOT(OnFindNextItem()));
    connect(ui->action_Edit_entry, SIGNAL(triggered()), this, SLOT(OnEditItem()));
    connect(ui->actionEdit_memo, SIGNAL(triggered()), this, SLOT(OnEditMemo()));
    connect(ui->actionMove_selected_entry_to_top, SIGNAL(triggered()), this, SLOT(OnMoveSelectedEntryToTop()));
    connect(this, SIGNAL(JustOneSelected(bool)), ui->action_Edit_entry, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(JustOneSelected(bool)), ui->actionMove_selected_entry_to_top, SLOT(setEnabled(bool)));
    connect(ui->actionErase_clipboard_contents, SIGNAL(triggered()), this, SLOT(OnEraseClipboard()));    
    connect(ui->actionSelect_font, SIGNAL(triggered()), this, SLOT(OnSelectFont()));    
    connect(ui->actionMaximal_number_of_entries, SIGNAL(triggered()), this, SLOT(OnMaxItems()));
    connect(ui->action_Lines_per_entry, SIGNAL(triggered()), this, SLOT(OnLinesPerItem()));
    connect(ui->actionSet_to_default_position_and_size, SIGNAL(triggered()), this, SLOT(OnSetDefaultPosSize()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(OnHelp()));    
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));    
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(OnAboutQt()));
    connect(ui->actionAlways_on_top, SIGNAL(triggered(bool)), this, SLOT(OnToggleAlwaysOnTop(bool)));
    connect(ui->actionUse_timer_to_detect_clipboard_changes, SIGNAL(triggered(bool)), this, SLOT(OnToggleUseTimer(bool)));
    connect(ui->actionAlways_move_to_first_position_if_found, SIGNAL(triggered(bool)), this, SLOT(OnAlwaysMoveToTopIfFound(bool)) );
    connect(ui->actionEnable_global_hot_keys, SIGNAL(triggered(bool)), this, SLOT(OnEnableGlobalHotkeys(bool)) );
    connect(ui->actionMove_to_current_mouse_position, SIGNAL(triggered()), this, SLOT(OnMoveToCurrentMousePosition()) );

    connect(this, SIGNAL(SavedDataChanged(bool)), this, SLOT(OnSavedDataChanged(bool)));
    connect(m_pUndoStack, SIGNAL(cleanChanged(bool)), this, SLOT(OnCleanChanged(bool)));

    //connect(dynamic_cast<QApplication *>(qApp), SIGNAL(screenAdded(QScreen *)), this, SLOT(OnScreenAdded(QScreen *)));
    //connect(dynamic_cast<QApplication *>(qApp), SIGNAL(screenRemoved(QScreen *)), this, SLOT(OnScreenRemoved(QScreen *)));
    //connect(dynamic_cast<QApplication *>(qApp), SIGNAL(primaryScreenChanged(QScreen *)), this, SLOT(OnPrimaryScreenChanged(QScreen *)));

    if( ui->actionAutoload_data->isChecked() )
    {
        LoadAndCheck();
    }
    QList<int> aFirstRow;
    aFirstRow.append(0);
    SyncListWithUi(aFirstRow);

#if defined(Q_OS_MAC)
    // Mac only supports all detection of changes via timer
    ui->actionUse_timer_to_detect_clipboard_changes->setEnabled(false);
    ui->actionUse_timer_to_detect_clipboard_changes->setChecked(true);
#endif

    // because the semantic of the dataChanged() signal of the QClipboard class is different for Mac
    // we use a timer to get the changes of the clipboard contents...
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerUpdate()));
    if( ui->actionUse_timer_to_detect_clipboard_changes->isChecked() )
    {
        m_pTimer->start(TIMER_DELAY);
    }
#if !defined(Q_OS_MAC)
    connect(m_pClipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(OnClipboardChanged(QClipboard::Mode)));
    connect(m_pClipboard, SIGNAL(dataChanged()), this, SLOT(OnClipboardDataChanged()));
#endif
// TODO findBufferChanged signal ?

    m_pAutoSaveTimer = new QTimer(this);
    connect(m_pAutoSaveTimer, SIGNAL(timeout()), this, SLOT(OnAutoSaveTimeout()));
    if( ui->actionAutosave->isChecked() )
    {
        m_pAutoSaveTimer->start(AUTOSAVE_TIMER_DELAY);     // save every 60 seconds
    }

    OnToggleUseTimer(ui->actionUse_timer_to_detect_clipboard_changes->isChecked());
    OnToggleAutoSaveTimer(ui->actionAutosave->isChecked());
    connect(ui->actionUse_timer_to_detect_clipboard_changes, SIGNAL(toggled(bool)), this, SLOT(OnToggleUseTimer(bool)));
    connect(ui->actionAutosave, SIGNAL(toggled(bool)), this, SLOT(OnToggleAutoSaveTimer(bool)));

    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    QPalette aPalette;
//    aPalette.setColor(QPalette::HighlightedText,QColor("#00FFFF"));
//    aPalette.setColor(QPalette::Highlight,QColor("#FF00FF"));
//    ui->listWidget->setPalette(aPalette);

    m_aHotkeyManager = new UGlobalHotkeys(this);

    OnEnableGlobalHotkeys(ui->actionEnable_global_hot_keys->isChecked());

    OnSelectionChanged();
}

CliphistWindow::~CliphistWindow()
{
    delete m_aHotkeyManager;

    if( ui->actionAutoload_data->isChecked() )
    {
        SaveAndCheck();
    }
    SaveSettings();
    delete m_pUndoStack;
    delete m_pTimer;
    delete m_pAutoSaveTimer;
    delete ui;
}

void CliphistWindow::OnHelp()
{
    QMessageBox::information(this,tr("Help"),QString(tr("<p>Sorry, no help available yet !</p><p>See %1 for more information.</p>")).arg(HOMEPAGE));
}

void CliphistWindow::OnSetDefaultPosSize()
{
#if defined( Q_WS_MACX )
    resize(380,300);
    move(1,20);
#else
    resize(380,320);
    move(1,1);
#endif
}

void CliphistWindow::OnAlwaysMoveToTopIfFound(bool bChecked)
{
    m_bIfFoundMoveToFirstPos = bChecked;
}

void CliphistWindow::OnToggleAlwaysOnTop(bool bChecked)
{
    if( bChecked )
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void CliphistWindow::OnToggleUseTimer(bool bChecked)
{
    if( bChecked )
    {
        m_pTimer->start(TIMER_DELAY);
    }
    else
    {
        m_pTimer->stop();
    }
}

void CliphistWindow::OnToggleAutoSaveTimer(bool bChecked)
{
    if( bChecked )
    {
        m_pAutoSaveTimer->start(AUTOSAVE_TIMER_DELAY);
    }
    else
    {
        m_pAutoSaveTimer->stop();
    }
}

void CliphistWindow::OnAbout()
{
    QMessageBox::about(this,tr("About Application"),QString(tr("<b>%1</b><small><p>Version %2 from %3</p><p>(c) 2025 by %4</p><p>License: %5</p><small>")).arg(TITLE,VERSION,__DATE__,AUTHORS,LICENSE));
}

void CliphistWindow::OnAboutQt()
{
    QMessageBox::aboutQt(this,tr("Clipboard History 2"));
}

void CliphistWindow::OnDeleteAllItems()
{
    QMessageBox::StandardButtons aButton = QMessageBox::question(this,tr("Question"),tr("Really delete all items in the list ?"),QMessageBox::Yes|QMessageBox::No);
    if( aButton == QMessageBox::Yes )
    {
        m_pUndoStack->push(new DeleteAllCommand(this,m_aTxtHistory,GetAllIndicesOfActSelected()));
    }
}

void CliphistWindow::DoDeleteAllItems()
{
    m_aTxtHistory.clear();
    SetDataChanged(false);
    SyncListWithUi(QList<int>());
}

void CliphistWindow::UndoDeleteAllItems(const QStringList & aHistory, const QList<int> & aActSelected)
{
    m_aTxtHistory = aHistory;
    SetDataChanged(true);
    SyncListWithUi(aActSelected);
}

void CliphistWindow::OnDeleteItem()
{
    m_pUndoStack->push(new DeleteItemCommand(this,GetAllIndicesOfActSelected()));
}

QList<int> CliphistWindow::FindTextInHistory(const QString & sFindText) const
{
    QList<int> aRet;

    for( int i=0; i<m_aTxtHistory.size(); i++ )
    {
        if( m_aTxtHistory[i].contains(sFindText,Qt::CaseInsensitive) )
        {
            aRet.push_back(i);
        }
    }

    return aRet;
}

void CliphistWindow::OnFindItem()
{
    bool ok;
    QString sFind = QInputDialog::getText(this,tr("Input"),tr("Find text:"),/*QLineEdit::EchoMode mode=*/QLineEdit::Normal,/*text=*/m_sLastSearchText,&ok); //,/*Qt::WindowFlags flags =*/0);
    if( ok && !sFind.isEmpty() )
    {
        m_sLastSearchText = sFind;
        
        m_aFindList = FindTextInHistory(sFind);
        if( m_aFindList.size()>0 )
        {
            m_iFindIndex = 0;
            ActivateItemIdx(m_aFindList[m_iFindIndex++]);
        }
        else
        {
            m_iFindIndex = -1;
            QMessageBox::warning(this,tr("Warning"),tr("Search text not found !"));
        }
    }
}

void CliphistWindow::OnFindNextItem()
{
    if( m_aFindList.size()==0 )
    {
        QMessageBox::warning(this,tr("Warning"),tr("Search text not found !"));
    }
    else if( m_iFindIndex<m_aFindList.size() )
    {
        ActivateItemIdx(m_aFindList[m_iFindIndex++]);
    }
    else
    {
        if( QMessageBox::warning(this,tr("Warning"),tr("No more occurrences of the search text found !\nGoto first item again ?"),QMessageBox::Yes|QMessageBox::No)
            == QMessageBox::Yes )
        {
            m_iFindIndex = 0;
            OnFindNextItem();
        }
    }
}

void CliphistWindow::OnMoveSelectedEntryToTop()
{
    // this is a combined undo/redo command (macro)
    m_pUndoStack->beginMacro("Move to top");
    // save actual value
    int iSelectedIndex = GetAllIndicesOfActSelected()[0];
    QString sTemp = m_aTxtHistory[iSelectedIndex];     // we know that only one item is selected !
    // delete actual vaule
    m_pUndoStack->push(new DeleteItemCommand(this,GetAllIndicesOfActSelected()));
    // restore saved value as entry on top of the saved entries
    m_pUndoStack->push(new InsertCommand(this,0,sTemp,/*bUpdateSelection*/true));
    // activate entry if it was activated before
// TODO --> bug, aktivierung des eintrags passt nicht mehr da danach delete und insert durchgefuehrt wird !!!
    if( iSelectedIndex==m_iActivatedIndex )
    {
        m_pUndoStack->push(new ActivateItemCommand(this,0,m_iActivatedIndex));
    }
    // finish macro
    m_pUndoStack->endMacro();
}

void CliphistWindow::OnAddSelectedToNewEntry()
{
    QString sNewItem;
    QList<int> aSelected = GetAllIndicesOfActSelected(false);
    for( int i=0; i<aSelected.size(); i++ )
    {
        if( sNewItem.length()>0 )
        {
            sNewItem += GetNewLine();
        }
        sNewItem += m_aTxtHistory[aSelected[i]];
    }
    m_pUndoStack->push(new InsertCommand(this,0,sNewItem,/*bUpdateSelection*/true));
}

void CliphistWindow::OnEditItem()
{
    if( ui->listWidget->currentItem() )
    {
        int iCurrentRow = GetAllIndicesOfActSelected()[0];      // we know that only one item is selected !
        // set the color of the selected item as color for the text control
        QBrush aBrush = GetColorForIndex(iCurrentRow+1);
        if( m_iActivatedIndex==iCurrentRow )
        {
            aBrush.setColor(QColor(RED));
        }
        QPalette aPalette;
        aPalette.setBrush(QPalette::Text,aBrush);
        EditItem aDlg(this,ui->listWidget->font(),aPalette,m_aTxtHistory[iCurrentRow]);
        if( m_aEditDialogGeometry.size()>0 )
        {
            aDlg.restoreGeometry(m_aEditDialogGeometry);
        }
        if( IsTextImageItem(iCurrentRow) )
        {
            int iIndex;
            if( FindPixmap(GetHashFromTextImageItem(iCurrentRow),&iIndex) )
            {
                aDlg.setImage(m_aPixmapList[iIndex].second);
            }
            aDlg.showOnlyText(false);
            if( aDlg.exec()==QDialog::Accepted )
            {
                if( aDlg.exportImage() )
                {
                    QString sFileName = QFileDialog::getSaveFileName(this, tr("Export image as"), m_sLastImageSavePath, QString(tr("Image Files (%1)")).arg("*.png"));

                    if( !sFileName.isEmpty() && m_aPixmapList.size()>0 )
                    {
                        m_sLastImageSavePath = sFileName;

                        if( aDlg.GetSketch().hasSceneItems() )
                        {
                            aDlg.GetSketch().exportSceneAsImage().save(sFileName,"PNG");
                        }
                        else
                        {
                            m_aPixmapList[iIndex].second.save(sFileName,"PNG");
                        }
                    }
                }
            }
        }
        else
        {
            if( aDlg.exec()==QDialog::Accepted )
            {
                // we know that only one item is selected !
                if( aDlg.asNewEntry() )
                {
                    m_pUndoStack->push(new InsertCommand(this,iCurrentRow,aDlg.text(),/*bUpdateSelection*/true));
                }
                else
                {
                    m_pUndoStack->push(new UpdateCommand(this,iCurrentRow,aDlg.text(),m_aTxtHistory[iCurrentRow]));
                }
            }
        }
        // in any case --> save the new size of the window
        m_aEditDialogGeometry = aDlg.saveGeometry();
    }
}

void CliphistWindow::OnEditMemo()
{
    EditMemo aDlg(this, m_sMemo);
    if( m_aMemoDialogGeometry.size()>0 )
    {
        aDlg.restoreGeometry(m_aMemoDialogGeometry);
    }
    if( aDlg.exec()==QDialog::Accepted )
    {
        m_sMemo = aDlg.getMemo();
    }
    m_aMemoDialogGeometry = aDlg.saveGeometry();
}

void CliphistWindow::UpdateSelection(const QList<int> & aCurrentRows)
{
    foreach(int iRow,aCurrentRows)
        ui->listWidget->setCurrentRow(iRow,QItemSelectionModel::Select);
}

bool CliphistWindow::IsActClipboardEntrySameAsActivatedItem(QString & sHash) const
{
    if( m_iActivatedIndex>=0 && m_iActivatedIndex<m_aTxtHistory.size() )
    {
        if( ui->actionEnable_support_for_images->isChecked() && m_pClipboard->mimeData()->hasImage() )
        {
            if( IsTextImageItem(m_iActivatedIndex) )
            {
                QPixmap aActPixmap = m_pClipboard->pixmap();
                sHash = GetHashForImage(aActPixmap).toHex();
                return sHash==GetHashFromTextImageItem(m_iActivatedIndex);
            }
            else
            {
                return false;
            }
        }
        return m_pClipboard->text()==m_aTxtHistory[m_iActivatedIndex];
    }
    else
    {
        return false;
    }
}

int CliphistWindow::FindItemIndex(const QString & sItemText) const
{
    for( int i=0; i<m_aTxtHistory.size(); i++ )
    {
        if( sItemText==m_aTxtHistory[i] )
        {
            return i;
        }
    }
    return -1;
}

bool CliphistWindow::IsActClipboardEntryPixmap() const
{
    return m_pClipboard->mimeData()->hasImage(); //!m_pClipboard->pixmap().isNull();
}

bool CliphistWindow::IsActClipboardEntryEmpty() const
{
    QString sActClipboardEntry = m_pClipboard->text();
    return sActClipboardEntry.isEmpty() || sActClipboardEntry.isNull();
}

bool CliphistWindow::IsAnyItemActivated() const
{
    return m_iActivatedIndex>=0;
}

void CliphistWindow::OnTimerUpdate()
{
    if( ui->actionUse_timer_to_detect_clipboard_changes->isChecked() )
    {
        OnClipboardDataChanged();
    }
}

void CliphistWindow::OnClipboardChanged(QClipboard::Mode /*aMode*/)
{
    // nothing to do yet...
}

bool CliphistWindow::FindPixmap(const QPixmap & aPixmap, int * pIndex) const
{
    QString sHash = GetHashForImage(aPixmap).toHex();
    return FindPixmap(sHash,pIndex);
}

bool CliphistWindow::FindPixmap(const QString & sHash, int * pIndex) const
{
    for(int i=0; i<m_aPixmapList.size(); i++)
    {
        if( m_aPixmapList[i].first==sHash )
        {
            if( pIndex )
            {
                *pIndex = i;
            }
            return true;
        }
    }
    return false;
}

bool CliphistWindow::FindPixmapInTextHistory(const QString & sHash, int * pIndex)
{
    for( int i=0; i<m_aTxtHistory.size(); i++ )
    {
        if( IsTextImageItem(i) && GetHashFromTextImageItem(i)==sHash )
        {
            if( pIndex )
            {
                *pIndex = i;
            }
            return true;
        }
    }
    return false;
}

void CliphistWindow::RemovePixmapWithHash(const QString & sHash)
{
    int iIndex;
    if( FindPixmap(sHash,&iIndex) )
    {
        m_aPixmapList.removeAt(iIndex);
    }
}

void CliphistWindow::CleanUpUnusedPixmaps()
{
    int i;
    QList<QString> aLstFound;
    // search for all images which are referenced in the actual text list
    for( i=0; i<m_aTxtHistory.size(); i++ )
    {
        if( IsTextImageItem(i) )
        {
            aLstFound.push_back(GetHashFromTextImageItem(i));
        }
    }
    // remove all images not referenced from the actual text list
    for( i=m_aPixmapList.size()-1; i>=0; i-- )
    {
        if( !aLstFound.contains(m_aPixmapList[i].first) )
        {
            m_aPixmapList.removeAt(i);
        }
    }
}

QString CliphistWindow::GetHashFromTextImageItem(int iIndex) const
{
    return m_aTxtHistory[iIndex].mid(QString(IMAGE_TAG).length());
}

bool CliphistWindow::IsTextImageItem(int iIndex) const
{
    return m_aTxtHistory[iIndex].startsWith(IMAGE_TAG);
}

// update text/image list if actual clipboard content is not already active item
void CliphistWindow::OnClipboardDataChanged()
{
    if( !m_bMyClipboardCopy && ui->action_Activate_cliphist->isChecked() )
    {
        QString sHash;
        bool bIsActTheSameAsActivated = IsActClipboardEntrySameAsActivatedItem(sHash);
        if( !IsActClipboardEntryPixmap() &&
            !IsActClipboardEntryEmpty() &&
            !bIsActTheSameAsActivated )
        {
            int iFoundIndex = FindItemIndex(m_pClipboard->text());
            if( iFoundIndex>=0 )
            {
                ActivateItemIdx(iFoundIndex);
                if( m_bIfFoundMoveToFirstPos )
                {
                    OnMoveSelectedEntryToTop();
                    ActivateItemIdx(0);
                }
            }
            else
            {
                m_pUndoStack->push(new InsertCommand(this,0,m_pClipboard->text(),ui->actionNew_clipboard_content_updates_selection->isChecked()));
            }
        }
        else
        {
// TODO gulp working --> dies ist wohl ein performance problem !!! --> IsActClipboardEntrySameAsActivatedItem() kopiert pixmap, hash wird zu häufig berechnet !!!
// --> problem mit timer aktualisierung und interferenz mit Selektion (wegen performance)
            if( ui->actionEnable_support_for_images->isChecked() )
            {
                if( m_pClipboard->mimeData()->hasImage() && !bIsActTheSameAsActivated )
                {
                    int iIndex;
                    if( FindPixmapInTextHistory(sHash,&iIndex) )
                    {
                        ActivateItemIdx(iIndex);
                        if( m_bIfFoundMoveToFirstPos )
                        {
                            OnMoveSelectedEntryToTop();
                            ActivateItemIdx(0);
                        }
                    }
                    else
                    {
                        QPixmap aClipboardImg = m_pClipboard->pixmap();
                        QByteArray aHash = GetHashForImage(aClipboardImg);
                        QString sHash = aHash.toHex();
                        QString sImgName = IMAGE_TAG+sHash;
                        QPair<QString,QPixmap> aItem(sHash,aClipboardImg);
                        m_aPixmapList.push_front(aItem);
                        m_pUndoStack->push(new InsertCommand(this,0,sImgName,ui->actionNew_clipboard_content_updates_selection->isChecked()));
                    }
                }
                else
                {
                }
             }
        }
    }
    // update the view if the clipboard has no text or image and the application has a text selected
    if( IsActClipboardEntryEmpty() &&
        (!m_pClipboard->mimeData()->hasImage() || !ui->actionEnable_support_for_images->isChecked()) &&
        IsAnyItemActivated() )
    {
        m_iActivatedIndex = -1;
        SyncListWithUi(GetAllIndicesOfActSelected());
    }
}

void CliphistWindow::OnItemClicked(QListWidgetItem * current)
{
    if( !ui->action_Double_click_to_select->isChecked() )
    {    
        ActivateItem(current);
    }
}

void CliphistWindow::OnItemDoubleClicked(QListWidgetItem * current)
{
// TODO --> bugfix? original ! operator !!!
    if( ui->action_Double_click_to_select->isChecked() )
    {    
        ActivateItem(current);
    }
}

void CliphistWindow::OnItemActivated(QListWidgetItem* current)
{
    ActivateItem(current);
}

void CliphistWindow::OnSelectionChanged()
{
    int iCount = ui->listWidget->selectedItems().size();
    emit SelectionChanged(iCount>0);
    emit JustOneSelected(iCount==1);
    emit MoreThanOneSelected(iCount>1);
}

void CliphistWindow::OnEnableGlobalHotkeys(bool bChecked)
{
    if( bChecked )
    {
        m_aHotkeyManager->registerHotkey("Ctrl+Shift+F12", 1);
        m_aHotkeyManager->registerHotkey("Ctrl+Shift+F11", 2);
        m_aHotkeyManager->registerHotkey("Shift+Ctrl+V", 3);
        m_aHotkeyManager->registerHotkey("Alt+Y", 4);
        connect(m_aHotkeyManager, &UGlobalHotkeys::activated, [this](size_t id)
        {
            if(id==1)
            {
                // show cliphist2 application
                this->show();
                this->showNormal();
                this->raise();
                this->activateWindow();
            }
            if(id==2)
            {
                // minimize cliphist2 application
                this->showMinimized();
            }
            if(id==3)
            {
                int iIndex = GetActivateItemIdx();
                int iShift = ui->actionMove_position_down_Shift_Ctrl_V->isChecked() ? 1 : -1;
                iIndex = iIndex + iShift;
                if( iIndex >= 0 && iIndex < ui->listWidget->count() )
                {
                    ActivateItemIdx(iIndex);
                }
            }
            if(id==4)
            {
                OnMoveToCurrentMousePosition();
            }
        });
    }
    else
    {
        m_aHotkeyManager->unregisterAllHotkeys();
    }
}

void CliphistWindow::OnMoveToCurrentMousePosition()
{
    // move application window to the current mouse position
    QPoint aPos = QCursor::pos();
    move(aPos+QPoint(0,-size().height()));
}

void CliphistWindow::OnSavedDataChanged(bool bIsSaved)
{
    QString sFileSize;
    sFileSize = QString(tr(" (Size: %1 Bytes)")).arg(m_iCurrentFileSize);
    if( !bIsSaved )
    {
        setWindowTitle(m_sOriginalWindowTitle+sFileSize+_UNSAVED_MARKER);
    }
    else
    {
        setWindowTitle(m_sOriginalWindowTitle+sFileSize);
    }
}

void CliphistWindow::OnCleanChanged(bool isClean)
{
    emit SavedDataChanged(isClean);
}

void CliphistWindow::OnAutoSaveTimeout()
{
    if(windowTitle().contains(_UNSAVED_MARKER))
    {
        Save();
    }
}

/*
#include <QDebug>

void CliphistWindow::OnScreenAdded(QScreen * pScreen)
{
    qDebug() << "SLOT screenAdded() " << pScreen << Qt::endl;
}

void CliphistWindow::OnScreenRemoved(QScreen * pScreen)
{
    qDebug() << "SLOT screenRemoved() " << pScreen << Qt::endl;
}

void CliphistWindow::OnPrimaryScreenChanged(QScreen * pScreen)
{
    qDebug() << "SLOT primaryScreenChanged() " << pScreen << Qt::endl;
}
*/

void CliphistWindow::LoadAndCheck()
{
    if( !Load(m_sFileName) && !m_bSelfTest )
    {
        QMessageBox::warning(this,tr("Warning"),QString(tr("Can not read data file %1")).arg(m_sFileName),QMessageBox::Ok);
    }
}

void CliphistWindow::SaveAndCheck()
{
    if( !Save() && !m_bSelfTest )
    {
        if( QMessageBox::Retry == QMessageBox::warning(this,tr("Warning"),QString(tr("Can not save data file %1\nDiscard changes or retry with other name?")).arg(m_sFileName),QMessageBox::Discard|QMessageBox::Retry) )
        {
            OnSaveDataAs();
        }
    }
}

void CliphistWindow::OnLoadData()
{
    if( m_bChangedData )
    {
        QMessageBox::StandardButtons aButton = QMessageBox::question(this,tr("Question"),tr("Actual data not saved, really load new data ?"),QMessageBox::Yes|QMessageBox::No);
        if( aButton == QMessageBox::No )
        {
            return;
        }
    }
    QString sFileName = QFileDialog::getOpenFileName(this,tr("Open Data"), ".", QString(tr("Data Files (%1)")).arg(EXTENSIONS));
    LoadFileAndSync(sFileName);
}

void CliphistWindow::OnSaveData()
{
    SaveAndCheck();
}

void CliphistWindow::OnSaveDataAs()
{
    QString sFileName = QFileDialog::getSaveFileName(this,tr("Save Data as"), m_sFileName, QString(tr("Data Files (%1)")).arg(EXTENSIONS));
    if( !sFileName.isEmpty() )
    {
        m_sFileName = sFileName;
        SaveAndCheck();
    }
}

void CliphistWindow::OnEraseClipboard()
{
    m_bMyClipboardCopy = true;
    m_pClipboard->setText(QString());
    m_bMyClipboardCopy = false;
}

void CliphistWindow::OnSelectFont()
{
    bool ok;
    QFont aFont = QFontDialog::getFont(&ok, ui->listWidget->font(), this);
    if( ok )
    {
        SetFont(aFont);
    }
}

void CliphistWindow::OnMaxItems()
{
    bool ok;
    // for Qt >= 4.4.x use QInputDialog::getInt(...)
    int i = QInputDialog::getInt(this,tr("Input"),tr("Maximal number of entries:"),m_iMaxEntries,1,2147483647,1,&ok );
    if( ok )
    {
        m_iMaxEntries = i;
    }
}

void CliphistWindow::OnLinesPerItem()
{
    bool ok;
    // for Qt >= 4.4.x use QInputDialog::getInt(...)
    int i = QInputDialog::getInt(this,tr("Input"),tr("Lines per entry:"),m_iMaxLinesPerEntry,1,2147483647,1,&ok );
    if( ok )
    {
        m_iMaxLinesPerEntry = i;
        SyncListWithUi(GetAllIndicesOfActSelected());
    }
}    

QListWidgetItem * CliphistWindow::CreateNewItem(const QString & s, const QBrush & aBrush, QPixmap * pPixmap)
{
    int noOfLines;
    QPair<QString,bool> aResult = FilterForDisplay(s, noOfLines);
    QListWidgetItem * pItem = pPixmap==0 ?
                            new QListWidgetItem(aResult.first,/*parent*/0,aResult.second ? 1 : 0) :
                            new QListWidgetItem(*pPixmap, aResult.first,/*parent*/0,aResult.second ? 1 : 0);

	if (pPixmap != 0)
	{
		// https://stackoverflow.com/questions/21211909/how-to-add-bold-text-to-rich-text-box
		QImage aIcon(pPixmap->toImage().scaledToWidth(100));
		QByteArray aData;
		QBuffer aBuffer(&aData);
		aIcon.save(&aBuffer, "PNG", 100);
		QString html = QString("<img src='data:image/png;base64, %0'>").arg(QString(aData.toBase64()));
		pItem->setToolTip(html);
	}
	else
	{
        pItem->setToolTip(s);
	}

    // Bugfix 22.9.2025 -> set height for list items for text and icon entries to the same value
    pItem->setSizeHint(QSize(m_iFontWidth, m_iFontHeight*noOfLines));

    pItem->setForeground(aBrush);
    //pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    return pItem;
}

QPair<QString,bool> CliphistWindow::FilterForDisplay(const QString & s, int & noOfLines) const
{
    QString sRet;
    QStringList aList = s.split(GetNewLine());
    int iMax = qMin(m_iMaxLinesPerEntry,aList.size());
    bool bMoreLines = m_iMaxLinesPerEntry<aList.size();
    for( int i=0; i<iMax; i++ )
    {
        sRet += aList[i]+GetNewLine();
    }
    noOfLines = iMax;
    return QPair<QString,bool>(sRet.mid(0,sRet.size()-GetNewLine().size()),bMoreLines);   // remove last \n from return string;
}

QString CliphistWindow::FilterNumber(const QString & s, const QString & sNumber, bool bMoreLines ) const
{
    QString sRet;
    QStringList aList = s.split(GetNewLine());
    for( int i=0; i<aList.size(); i++ )
    {
        if( bMoreLines && i==aList.size()-1 )
        {
            sRet += QString("...")+GetNewLine();
        }
        else
        {
            sRet += i==0 ? sNumber+GetNewLine() : GetNewLine();
        }
    }
    return sRet.mid(0,sRet.size()-GetNewLine().size());   // remove last \n from return string
}

bool CliphistWindow::SyncListWithUi(const QList<int> & aSelectIdx)
{
    QString sActClipBoardText = m_pClipboard->text();
    bool bIsImage = ui->actionEnable_support_for_images->isChecked() && m_pClipboard->mimeData()->hasImage();
    QString sHash = bIsImage ? QString(GetHashForImage(m_pClipboard->pixmap()).toHex()) : "";
    bool bActivatedNeedUpdate = true;
    int iFirstOccurance = -1;
    ui->listWidget->clear();
    ui->listWidgetLineNumbers->clear();
    for( int i=m_aTxtHistory.size()-1; i>=0; i-- )
    {
        if( (bIsImage && IsTextImageItem(i) && GetHashFromTextImageItem(i)==sHash) ||
            (sActClipBoardText==m_aTxtHistory[i] && bActivatedNeedUpdate) )
        {
            iFirstOccurance = i;        // always update variable because we loop from high indizes
            // actual index is still valid --> no update needed
            if( m_iActivatedIndex==i )
            {
                bActivatedNeedUpdate = false;
            }
        }
        // handle icons for images in list item
        QPixmap * pPixmap = 0;
        if( IsTextImageItem(i) )
        {
            int iIndex;
            if( FindPixmap(GetHashFromTextImageItem(i),&iIndex) )
            {
                pPixmap = &m_aPixmapList[iIndex].second;
            }
        }
// TODO --> QIcons veraendern die Hoehe der Zeilen --> anpassen + Behandlung von Icons konfigurierbar machen (via Menu Eintrag)
        InsertNewUiData(m_aTxtHistory[i],i+1,pPixmap);
    }
    if( bActivatedNeedUpdate && iFirstOccurance!=-1 )
    {
        m_iActivatedIndex = iFirstOccurance;
    }
    if( IsAnyItemActivated() )
    {
        UpdateLastActivatedItemData(ui->listWidget->item(m_iActivatedIndex));
    }
    UpdateSelection(aSelectIdx);
    return true;
}

void CliphistWindow::InsertNewUiData(const QString & sText, int iNumber, QPixmap * pPixmap)
{
    QListWidgetItem * pNewItem = CreateNewItem(sText,GetColorForIndex(iNumber),pPixmap);
    ui->listWidget->insertItem(0,pNewItem);
    QListWidgetItem * pNewNumberItem = CreateNewItem(sText,GetColorForIndex(iNumber),0); //pNewItem->clone();
    QString s = pNewNumberItem->text();
    pNewNumberItem->setText(FilterNumber(s,QString::number(iNumber),(bool)pNewItem->type()));
    pNewNumberItem->setToolTip("");
    pNewNumberItem->setTextAlignment(Qt::AlignRight);
    ui->listWidgetLineNumbers->insertItem(0,pNewNumberItem);
}

void CliphistWindow::CheckHistoryMemory()
{
    while( m_aTxtHistory.size()>m_iMaxEntries )
    {
        if( IsTextImageItem(m_aTxtHistory.size()-1) )
        {
            RemovePixmapWithHash(GetHashFromTextImageItem(m_aTxtHistory.size()-1));
        }
        m_aTxtHistory.removeLast();
    }
}

bool CliphistWindow::SaveSettings()
{
    QSettings aSettings;
    
    aSettings.setValue("App/DataFileName",m_sFileName);
    aSettings.setValue("App/LastImageFileName",m_sLastImageSavePath);
    aSettings.setValue("App/LastSearchText",m_sLastSearchText);
    aSettings.setValue("App/MaxLinesPerEntry",m_iMaxLinesPerEntry);    
    aSettings.setValue("App/MaxEntries",m_iMaxEntries);
    aSettings.setValue("App/MoveToFirstPos",m_bIfFoundMoveToFirstPos);
    aSettings.setValue("App/WindowState",saveState());
    aSettings.setValue("App/WindowGeometry",saveGeometry());
    aSettings.setValue("App/Font",ui->listWidget->font().toString());   
    aSettings.setValue("App/DblClickToSelect",ui->action_Double_click_to_select->isChecked()); 
    aSettings.setValue("App/AutoLoadData",ui->actionAutoload_data->isChecked());
    aSettings.setValue("App/AutoWindowData",ui->actionAutoload_window_position_and_size->isChecked()); 
    aSettings.setValue("App/AlwaysOnTop",ui->actionAlways_on_top->isChecked()); 
    aSettings.setValue("App/UseTimer",ui->actionUse_timer_to_detect_clipboard_changes->isChecked());
    aSettings.setValue("App/UseAutosave",ui->actionAutosave->isChecked());
    aSettings.setValue("App/GeometryEditDlg",m_aEditDialogGeometry);
    aSettings.setValue("App/GeometryMemoDlg",m_aMemoDialogGeometry);
    aSettings.setValue("App/UseGlobalKeys",ui->actionEnable_global_hot_keys->isChecked());
    aSettings.setValue("App/MovePositionDown",ui->actionMove_position_down_Shift_Ctrl_V->isChecked());
    aSettings.setValue("App/MemoText",m_sMemo);

    return true;
}

bool CliphistWindow::LoadSettings()
{
    QSettings aSettings;
    QFont aFont;
    
    m_sFileName = aSettings.value("App/DataFileName",m_sFileName).toString();
    m_sLastImageSavePath = aSettings.value("App/LastImageFileName",m_sLastImageSavePath).toString();
    m_sLastSearchText = aSettings.value("App/LastSearchText").toString();
    m_iMaxLinesPerEntry = aSettings.value("App/MaxLinesPerEntry",DEFAULT_LINES_PER_ENTRY).toInt();    
    m_iMaxEntries = aSettings.value("App/MaxEntries",DEFAULT_MAX_ENTRIES).toInt();
    m_bIfFoundMoveToFirstPos = aSettings.value("App/MoveToFirstPos",false).toBool();
    ui->action_Double_click_to_select->setChecked(aSettings.value("App/DblClickToSelect",true).toBool());
    ui->actionAutoload_data->setChecked(aSettings.value("App/AutoLoadData",true).toBool());
    ui->actionAutoload_window_position_and_size->setChecked(aSettings.value("App/AutoWindowData",true).toBool());
    ui->actionAlways_on_top->setChecked(aSettings.value("App/AlwaysOnTop",false).toBool());
    ui->actionUse_timer_to_detect_clipboard_changes->setChecked(aSettings.value("App/UseTimer",DEFAULT_VALUE_USE_TIMER).toBool());
    ui->actionAutosave->setChecked(aSettings.value("App/UseAutosave",true).toBool());
    if( ui->actionAutoload_window_position_and_size->isChecked() )
    {
        restoreState(aSettings.value("App/WindowState").toByteArray());
        restoreGeometry(aSettings.value("App/WindowGeometry").toByteArray());
    }
    aFont.fromString(aSettings.value("App/Font",QString("Courier")).toString());
    SetFont(aFont);
    m_aEditDialogGeometry = aSettings.value("App/GeometryEditDlg",QVariant(QByteArray())).toByteArray();
    m_aMemoDialogGeometry = aSettings.value("App/GeometryMemoDlg",QVariant(QByteArray())).toByteArray();
    ui->actionEnable_global_hot_keys->setChecked(aSettings.value("App/UseGlobalKeys").toBool());
    ui->actionMove_position_down_Shift_Ctrl_V->setChecked(aSettings.value("App/MovePositionDown",true).toBool());
    m_sMemo = aSettings.value("App/MemoText").toString();
    return true;
}

bool CliphistWindow::Save()
{
    CleanUpUnusedPixmaps();                 // destroys the possibility to undo the deletion of an image entry --> after save the image will not be found anymore
    QFile aFile(m_sFileName);
    if( aFile.open(QIODevice::WriteOnly) )
    {
        QDataStream out(&aFile);
        out << "CLIPHIST2";
        out << "VERSION" << FILE_VERSION;
        out << m_aTxtHistory;
        out << m_aPixmapList;
        m_iCurrentFileSize = aFile.size();
        SetDataChanged(false);
        return out.status()==QDataStream::Ok;
    }
    return false;
}

bool CliphistWindow::Load(const QString sFileName)
{
    if( QFile::exists(sFileName) )
    {
        QFile aFile(sFileName);
        if( aFile.open(QIODevice::ReadOnly) )
        {
            QDataStream in(&aFile);
            qint32 iVersion;
            QString s1,s2;
            in >> s1 >> s2 >> iVersion >> m_aTxtHistory;
            if( iVersion>1 )
            {
                in >> m_aPixmapList;
            }
            m_iCurrentFileSize = aFile.size();
            SetDataChanged(false);
            return in.status()==QDataStream::Ok;
        }
    }
    return false;
}

int CliphistWindow::GetActivateItemIdx() const
{
    return ui->listWidget->currentRow();
}

void CliphistWindow::ActivateItemIdx(int iIndex)
{
    ui->listWidget->setCurrentRow(iIndex);
    ActivateItem(ui->listWidget->currentItem());
}

void CliphistWindow::ActivateItem(QListWidgetItem * current)
{
    if( current )
    {
        ui->listWidget->setCurrentItem(current);
        UpdateColorOfLastActivatedItem();
        m_bMyClipboardCopy = true;
        UpdateLastActivatedItemData(current);
        if( IsTextImageItem(m_iActivatedIndex) )
        {
            int iIndex;
            if( FindPixmap(GetHashFromTextImageItem(m_iActivatedIndex),&iIndex) )
            {
                m_pClipboard->setPixmap(m_aPixmapList[iIndex].second);
            }
        }
        else
        {
			if (m_pClipboard->text() != m_aTxtHistory[m_iActivatedIndex])
			{
				m_pClipboard->setText(m_aTxtHistory[m_iActivatedIndex]);
			}
        }
        m_bMyClipboardCopy = false;
    }
}

QBrush CliphistWindow::GetNextColor(const QBrush & aActColor) const
{
    return aActColor.color().name()==GREEN ? QBrush(QColor(BLUE)) : QBrush(QColor(GREEN));
}

QBrush CliphistWindow::GetColorForIndex(int iIndex) const
{
    return (iIndex % 2)==1 ? QBrush(QColor(BLUE)) : QBrush(QColor(GREEN));
}

QBrush CliphistWindow::GetColorOfNeighbour(int iMyIndex) const
{
    int iNeighbourIndex = -1;
    if( iMyIndex>0 )
    {
        iNeighbourIndex = iMyIndex-1;
    }
    else if( iMyIndex+1<ui->listWidget->count() )
    {
        iNeighbourIndex = iMyIndex+1;
    }
    if( iNeighbourIndex!=-1 )
    {
        return GetNextColor(ui->listWidget->item( iNeighbourIndex )->foreground());
    }
    return QBrush(QColor(BLUE));
}

void CliphistWindow::UpdateColorOfLastActivatedItem()
{
    if( m_iActivatedIndex>=0 )
    {
        QBrush aColor = GetColorOfNeighbour(m_iActivatedIndex);
        ui->listWidget->item(m_iActivatedIndex)->setForeground(aColor);
        ui->listWidgetLineNumbers->item(m_iActivatedIndex)->setForeground(aColor);
    }
}

void CliphistWindow::UpdateLastActivatedItemData(QListWidgetItem * current)
{
    m_iActivatedIndex = ui->listWidget->row(current);
    if( current )
    {
        current->setForeground(QBrush(QColor(RED)));
        // create also a number entry
        QListWidgetItem * pNumber = ui->listWidgetLineNumbers->item(m_iActivatedIndex);
        pNumber->setForeground(QBrush(QColor(RED)));
    }
}

void CliphistWindow::SetFont(const QFont & aFont)
{
    ui->listWidget->setFont(aFont);
    ui->listWidgetLineNumbers->setFont(aFont);
    // update width of the number list widget
    QFontMetrics aMetrics(aFont);
    m_iFontWidth = aMetrics.boundingRect(QString(_DEFAULT_TEXT)).width();
    m_iFontHeight = aMetrics.boundingRect(QString(_DEFAULT_TEXT)).height();
    ui->listWidgetLineNumbers->setMaximumWidth(m_iFontWidth);
    ui->listWidgetLineNumbers->setMaximumWidth(m_iFontHeight*m_iMaxLinesPerEntry);
}

void CliphistWindow::SetDataChanged(bool bValue)
{
    m_bChangedData = bValue;
    emit SavedDataChanged(!bValue);
    if(!bValue)
    {
        m_pUndoStack->setClean();
    }
}

QString CliphistWindow::GetNewLine() const
{
    return "\n";
}

//int CliphistWindow::GetIndexOfActSelected() const
//{
//    int i = ui->listWidget->currentRow();
//    QList<QListWidgetItem *> aLst = ui->listWidget->selectedItems();
//    QList<int> aIndexLst;
//    foreach(QListWidgetItem * pItem,aLst)
//        aIndexLst.append(ui->listWidget->row(pItem));
//    qSort(aIndexLst);
//    return i;
//}
#include <algorithm>

QList<int> CliphistWindow::GetAllIndicesOfActSelected(bool bSort) const
{
    QList<int> aIndexLst;
    foreach(QListWidgetItem * pItem,ui->listWidget->selectedItems())
        aIndexLst.append(ui->listWidget->row(pItem));
    if( bSort )
    {
        //qSort(aIndexLst);
        std::sort(aIndexLst.begin(), aIndexLst.end());
    }
    return aIndexLst;
}

QString CliphistWindow::RemoveGiven(int iIndexOfDeletedItem)
{
    QString sItem = QString();
    if( iIndexOfDeletedItem>=0 && iIndexOfDeletedItem<m_aTxtHistory.size() )
    {
        QList<int> aSelectedItems = GetAllIndicesOfActSelected();
        sItem = m_aTxtHistory[iIndexOfDeletedItem];
        m_aTxtHistory.removeAt(iIndexOfDeletedItem);
        aSelectedItems.removeOne(iIndexOfDeletedItem);
        // WARNING:
        // side effect --> if actual selected item is the contents of the clipboard,
        // than also clear the contents of the clipboard !
        if( iIndexOfDeletedItem==m_iActivatedIndex )
        {
            m_pClipboard->setText(QString());
        }
        SetDataChanged(true);
        //SyncListWithUi( GetIndexOfActSelected()>iIndexOfDeletedItem ? GetIndexOfActSelected()-1 : GetIndexOfActSelected() );
        SyncListWithUi( aSelectedItems );
    }
    return sItem;
}

int CliphistWindow::UpdateList(int iPosition, const QString & sText)
{
    if( (iPosition>=0 && iPosition<m_aTxtHistory.size() && sText.length()>0) )
    {
        m_aTxtHistory[iPosition] = sText;
        if( iPosition==m_iActivatedIndex )
        {
            m_pClipboard->setText(m_aTxtHistory[m_iActivatedIndex]);
        }
        CheckHistoryMemory();
        SetDataChanged(true);
        SyncListWithUi( GetAllIndicesOfActSelected() );
        return iPosition;
    }
    return -1;
}

int CliphistWindow::InsertInList(int iPosition, const QString & sText, bool bUpdateSelection)
{
    if( iPosition>=0 /*&& iPosition<m_aTxtHistory.size()*/ && !sText.isEmpty() && !sText.isNull() )
    {
        QList<int> aSelectedItems = GetAllIndicesOfActSelected();
        m_aTxtHistory.insert(iPosition,sText);
        CheckHistoryMemory();
        SetDataChanged(true);
        if( bUpdateSelection )
        {
            QList<int> aSelection;
            aSelection.append(iPosition);
            SyncListWithUi( aSelection );
        }
        else
        {
            //SyncListWithUi( GetIndexOfActSelected()>iPosition ? GetIndexOfActSelected()+1 : GetIndexOfActSelected() );
            // update the positions of the actual selected items,
            // must be updated because the new insert item maybe shifts the positions
            for( int i=0; i<aSelectedItems.size(); i++ )
            {
                // increment all the positions after the inserted item
                if( aSelectedItems[i]>=iPosition )
                {
                    aSelectedItems[i]++;
                }
            }
            SyncListWithUi(aSelectedItems);
        }
        return iPosition;
    }
    return -1;
}

void CliphistWindow::LoadFileAndSync(const QString sFileName)
{
    if( !sFileName.isEmpty() )
    {
        m_sFileName = sFileName;
        LoadAndCheck();
        QList<int> aFirstRow;
        aFirstRow.append(0);
        SyncListWithUi(aFirstRow);
    }
}
