/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2010 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				 *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				 *
 * Copyright (C) 2009-2010 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/
/*
    Initial Version for OS/2:         1994
    Ported to Windows XP:             2001
    Started porting to Qt:       12.9.2009    (Mac, Linux, Windows)

    Translation:
       lupdate Cliphist2.pro    *.cpp --> *.ts
       linguist cliphist_de.ts
       lrelease Cliphist2.pro   *.ts  --> *.qm

    *.pro file must contain something like: TRANSLATIONS = cliphist_de.ts

    qmake -spec macx-g++ Cliphist2.pro [CONFIG+=release]
    make
    cp *.qm Cliphist2.app/Contents/Resources/
    macdeployqt Cliphist2.app -dmg
*/

#include "cliphistwindow.h"
#include "ui_cliphistwindow.h"

#include "edititem.h"

#include <QFile>
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

#if defined(Q_OS_MAC)
#include <QTimer>
#endif

// ************************************************************************

#define VERSION                     "1.0.0"

#define FILE_VERSION                1
#define DEFAULT_FILE_NAME           "cliphist2.dat"
#define DEFAULT_LINES_PER_ENTRY     3
#define DEFAULT_MAX_ENTRIES         100
#define BLUE                        "#0000ff"   //"blue"
#define GREEN                       "#008000"   //"green"
#define RED                         "#ff0000"   //"red"

// ************************************************************************

CliphistWindow::CliphistWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CliphistWindow)
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    
    QCoreApplication::setOrganizationName("MNeuroth");
    QCoreApplication::setOrganizationDomain("mneuroth.de");
    QCoreApplication::setApplicationName("ClipHist2");    
    
    m_pClipboard        = QApplication::clipboard();    
    m_iFindIndex        = -1;
    m_bChangedData      = false;
    m_bMyCopy           = false;
    m_iActSelectedIndex = -1;
    m_iMaxEntries       = DEFAULT_MAX_ENTRIES;
    m_iMaxLinesPerEntry = DEFAULT_LINES_PER_ENTRY;
    m_sFileName         = DEFAULT_FILE_NAME;
    
    LoadSettings();
    if( ui->actionAutoload_data->isChecked() )
    {
        LoadAndCheck();
    }
    SyncListWithUi();

    OnToggleAlwaysOnTop(ui->actionAlways_on_top->isChecked());

    connect(m_pClipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(OnClipboardChanged(QClipboard::Mode)));
    connect(m_pClipboard, SIGNAL(dataChanged()), this, SLOT(OnClipboardDataChanged()));
// TODO findBufferChanged signal ?
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnItemClicked(QListWidgetItem *)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnItemDoubleClicked(QListWidgetItem *)));
    connect(ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(OnItemActivated(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
    
    // synchronize the two vertical scrollbars of the two list widgets:
    connect(ui->listWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->listWidgetLineNumbers->verticalScrollBar(), SLOT(setValue(int)) );

    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OnLoadData()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(OnSaveData()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(OnSaveDataAs()));
    connect(ui->actionDelete_all_entries, SIGNAL(triggered()), this, SLOT(OnDeleteAllItems()));
    connect(ui->actionDelete_marked_entry, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));
    connect(this, SIGNAL(SelectionChanged(bool)), ui->actionDelete_marked_entry, SLOT(setEnabled(bool)));
    connect(ui->action_Find_text, SIGNAL(triggered()), this, SLOT(OnFindItem()));
    connect(ui->actionFind_next, SIGNAL(triggered()), this, SLOT(OnFindNextItem()));
    connect(ui->action_Edit_entry, SIGNAL(triggered()), this, SLOT(OnEditItem()));
    connect(this, SIGNAL(SelectionChanged(bool)), ui->action_Edit_entry, SLOT(setEnabled(bool)));
    connect(ui->actionErase_clipboard_contents, SIGNAL(triggered()), this, SLOT(OnEraseClipboard()));    
    connect(ui->actionSelect_font, SIGNAL(triggered()), this, SLOT(OnSelectFont()));    
    connect(ui->actionMaximal_number_of_entries, SIGNAL(triggered()), this, SLOT(OnMaxEntries()));    
    connect(ui->action_Lines_per_entry, SIGNAL(triggered()), this, SLOT(OnLinesPerEntry()));    
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(OnHelp()));    
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));    
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(OnAboutQt()));
    connect(ui->actionAlways_on_top, SIGNAL(triggered(bool)), this, SLOT(OnToggleAlwaysOnTop(bool)));

#if defined(Q_OS_MAC)
    // because the semantic of the dataChanged() signal of the QClipboard class is different for Mac
    // we use a timer to get the changes of the clipboard contents...
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerUpdate()));
    m_pTimer->start(200);
#else
    m_pTimer = 0;
#endif

    OnSelectionChanged();
}

CliphistWindow::~CliphistWindow()
{
    if( ui->actionAutoload_data->isChecked() )
    {
        SaveAndCheck();
    }
    SaveSettings();
    delete m_pTimer;
    delete ui;
}

void CliphistWindow::OnHelp()
{
    QMessageBox::about(this,tr("Help"),tr("Sorry, no help available yet !"));
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

void CliphistWindow::OnAbout()
{
    QMessageBox::about(this,tr("About Application"),QString(tr("Clipboard History 2\n\nVersion %1\n\n(c) 2010 by Michael Neuroth")).arg(VERSION));
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
        m_aTxtHistory.clear();
        SetDataChanged(false);
        SyncListWithUi();
    }
}

void CliphistWindow::OnDeleteItem()
{
    if( ui->listWidget->currentItem() )
    {
        int iIndexOfSelectedItem = ui->listWidget->currentRow();
        m_aTxtHistory.removeAt(iIndexOfSelectedItem);
        // WARNING:
        // side effect --> if actual selected item is the contents of the clipboard,
        // than also clear the contents of the clipboard !
        if( iIndexOfSelectedItem==m_iActSelectedIndex )
        {
            m_pClipboard->setText(QString());
        }
        SetDataChanged(true);
        SyncListWithUi();
        UpdateSelection(iIndexOfSelectedItem);
    }
}

void CliphistWindow::OnFindItem()
{
    bool ok;
    QString sFind = QInputDialog::getText(this,tr("Input"),tr("Find text:"),/*QLineEdit::EchoMode mode=*/QLineEdit::Normal,/*text=*/m_sLastSearchText,&ok,/*Qt::WindowFlags flags =*/0);
    if( ok && !sFind.isEmpty() )
    {
        m_sLastSearchText = sFind;
        
        m_aFindList = ui->listWidget->findItems(sFind,/*Qt::MatchFlags*/Qt::MatchContains);
        if( m_aFindList.size()>0 )
        {
            m_iFindIndex = 0;
            ActivateEntry(m_iFindIndex++);
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
    if( m_aFindList.size()>0 && m_iFindIndex<m_aFindList.size() )
    {
        ActivateEntry(m_iFindIndex++);
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("No more occurrences of the search text found !"));
    }
}

void CliphistWindow::OnEditItem()
{
    if( ui->listWidget->currentItem() )
    {
        int iCurrentRow = ui->listWidget->currentRow();
        EditItem aDlg(this,ui->listWidget->font(),m_aTxtHistory[iCurrentRow]);
        if( m_aEditDialogGeometry.count()>0 )
        {
            aDlg.restoreGeometry(m_aEditDialogGeometry);
        }
        if( aDlg.exec()==QDialog::Accepted )
        {
            m_aEditDialogGeometry = aDlg.saveGeometry();
            m_aTxtHistory[ui->listWidget->currentRow()] = aDlg.text();
            SetDataChanged(true);
            SyncListWithUi();
            UpdateSelection(iCurrentRow);
        }
    }        
}

void CliphistWindow::UpdateSelection(int iCurrentRow)
{
    // update selection after modifying list
    if( iCurrentRow<ui->listWidget->count() )
    {
        ui->listWidget->setCurrentRow(iCurrentRow);
    }
    else
    {
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
}

bool CliphistWindow::IsActClipboardEntrySameAsActSelectedItem() const
{
    if( m_iActSelectedIndex>=0 && m_iActSelectedIndex<m_aTxtHistory.size() )
    {
        return m_pClipboard->text()==m_aTxtHistory[m_iActSelectedIndex];
    }
    else
    {
        return false;
    }
}

bool CliphistWindow::IsActClipboardEntryEmpty() const
{
    QString sActClipboardEntry = m_pClipboard->text();
    return sActClipboardEntry.isEmpty() || sActClipboardEntry.isNull();
}

bool CliphistWindow::IsAnyItemSelected() const
{
    return m_iActSelectedIndex!=-1;
}

void CliphistWindow::OnClipboardDataChanged()
{
    if( !m_bMyCopy &&
        !IsActClipboardEntryEmpty() &&
        !IsActClipboardEntrySameAsActSelectedItem() &&
        ui->action_Activate_cliphist->isChecked() )
    {
        UpdateColorOfLastSelectedItem();
        m_aTxtHistory.insert(0,m_pClipboard->text());
        while( m_aTxtHistory.size()>m_iMaxEntries )
        {
            m_aTxtHistory.removeLast();
        }
        SetDataChanged(true);
        SyncListWithUi();        
        UpdateLastSelectedItemData(ui->listWidget->item(0));        // update of clipboard data always into index 0 !
    }
    else
    {
        // update the view if the clipboard has no text and the application has a text selected
        if( IsActClipboardEntryEmpty() && IsAnyItemSelected() )
        {
            m_iActSelectedIndex = -1;
            SyncListWithUi();
        }
    }
}

void CliphistWindow::OnClipboardChanged(QClipboard::Mode /*aMode*/)
{
}

#if defined(Q_OS_MAC)
void CliphistWindow::OnTimerUpdate()
{
    OnClipboardDataChanged();
}
#endif

void CliphistWindow::OnItemClicked(QListWidgetItem * current)
{
    if( !ui->action_Double_click_to_select->isChecked() )
    {    
        ActivateEntry(current);
    }
}

void CliphistWindow::OnItemDoubleClicked(QListWidgetItem * current)
{
    if( !ui->action_Double_click_to_select->isChecked() )
    {    
        ActivateEntry(current);
    }
}

void CliphistWindow::OnItemActivated(QListWidgetItem* current)
{
    ActivateEntry(current);
}

void CliphistWindow::OnSelectionChanged()
{
    emit SelectionChanged(ui->listWidget->currentItem()!=0);
}

void CliphistWindow::LoadAndCheck()
{
    if( !Load() )
    {
        QMessageBox::warning(this,tr("Warning"),QString(tr("Can not read data file %1")).arg(m_sFileName),QMessageBox::Ok);
    }
}

void CliphistWindow::SaveAndCheck()
{
    if( !Save() )
    {
        QMessageBox::warning(this,tr("Warning"),QString(tr("Can not save data file %1")).arg(m_sFileName),QMessageBox::Ok);
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
    QString sFileName = QFileDialog::getOpenFileName(this,tr("Open Data"), ".", tr("Data Files (*.dat)"));    
    if( !sFileName.isEmpty() )
    {
        m_sFileName = sFileName;
        LoadAndCheck();
        SyncListWithUi();
    }
}

void CliphistWindow::OnSaveData()
{
    SaveAndCheck();
}

void CliphistWindow::OnSaveDataAs()
{
    QString sFileName = QFileDialog::getSaveFileName(this,tr("Save Data as"), m_sFileName, tr("Data Files (*.dat)"));    
    if( !sFileName.isEmpty() )
    {
        m_sFileName = sFileName;
        SaveAndCheck();
    }
}

void CliphistWindow::OnEraseClipboard()
{
    m_bMyCopy = true;
    m_pClipboard->setText(QString());
    m_bMyCopy = false;
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

void CliphistWindow::OnMaxEntries()
{
    bool ok;
    int i = QInputDialog::getInt(this,tr("Input"),tr("Maximal number of entries:"),m_iMaxEntries,1,2147483647,1,&ok );
    if( ok )
    {
        m_iMaxEntries = i;
    }
}

void CliphistWindow::OnLinesPerEntry()
{
    bool ok;
    int i = QInputDialog::getInt(this,tr("Input"),tr("Lines per entry:"),m_iMaxLinesPerEntry,1,2147483647,1,&ok );
    if( ok )
    {
        m_iMaxLinesPerEntry = i;
        SyncListWithUi();
    }
}    

QListWidgetItem * CliphistWindow::CreateNewItem(const QString & s, const QBrush & aBrush)
{
    QPair<QString,bool> aResult = FilterForDisplay(s);
    QListWidgetItem * pItem = new QListWidgetItem(aResult.first,/*parent*/0,aResult.second ? 1 : 0);
    pItem->setForeground(aBrush);
    //pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    pItem->setToolTip(s);
    return pItem;
}

QPair<QString,bool> CliphistWindow::FilterForDisplay(const QString & s) const
{
    QString sRet;
    QStringList aList = s.split(GetNewLine());
    int iMax = qMin(m_iMaxLinesPerEntry,aList.size());
    bool bMoreLines = m_iMaxLinesPerEntry<aList.size();
    for( int i=0; i<iMax; i++ )
    {
        sRet += aList[i]+GetNewLine();
    }
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

bool CliphistWindow::SyncListWithUi()
{
    QString sActClipBoardText = m_pClipboard->text();
    bool bNeedUpdate = true;
    int iFirstOccurance = -1;
    ui->listWidget->clear();
    ui->listWidgetLineNumbers->clear();
    for( int i=m_aTxtHistory.size()-1; i>=0; i-- )
    {
        if( sActClipBoardText==m_aTxtHistory[i] && bNeedUpdate )
        {
            if( iFirstOccurance==-1 )
            {
                iFirstOccurance = i;
            }
            // actual index is still valid --> no update needed
            if( m_iActSelectedIndex==i )
            {
                bNeedUpdate = false;
            }
        }
        InsertNewData(m_aTxtHistory[i],i+1);
    }
    if( bNeedUpdate && iFirstOccurance!=-1 )
    {
        m_iActSelectedIndex = iFirstOccurance;
    }
    if( IsAnyItemSelected() )
    {
        UpdateLastSelectedItemData(ui->listWidget->item(m_iActSelectedIndex));
    }
    return true;
}

void CliphistWindow::InsertNewData(const QString & sText, int iNumber)
{
    QListWidgetItem * pNewItem = CreateNewItem(sText,GetColorForIndex(iNumber));
    ui->listWidget->insertItem(0,pNewItem);
    QListWidgetItem * pNewNumberItem = pNewItem->clone();
    QString s = pNewNumberItem->text();
    pNewNumberItem->setText(FilterNumber(s,QString::number(iNumber),(bool)pNewItem->type()));
    pNewNumberItem->setToolTip("");
    pNewNumberItem->setTextAlignment(Qt::AlignRight);
    ui->listWidgetLineNumbers->insertItem(0,pNewNumberItem);
}

bool CliphistWindow::SaveSettings()
{
    QSettings aSettings;
    
    aSettings.setValue("App/DataFileName",m_sFileName);
    aSettings.setValue("App/LastSearchText",m_sLastSearchText);
    aSettings.setValue("App/MaxLinesPerEntry",m_iMaxLinesPerEntry);    
    aSettings.setValue("App/MaxEntries",m_iMaxEntries);    
    aSettings.setValue("App/WindowState",saveState());
    aSettings.setValue("App/WindowGeometry",saveGeometry());
    aSettings.setValue("App/Font",ui->listWidget->font().toString());   
    aSettings.setValue("App/DblClickToSelect",ui->action_Double_click_to_select->isChecked()); 
    aSettings.setValue("App/AutoLoadData",ui->actionAutoload_data->isChecked());
    aSettings.setValue("App/AutoWindowData",ui->actionAutoload_window_position_and_size->isChecked()); 
    aSettings.setValue("App/AlwaysOnTop",ui->actionAlways_on_top->isChecked()); 
    return true;
}

bool CliphistWindow::LoadSettings()
{
    QSettings aSettings;
    QFont aFont;
    
    m_sFileName = aSettings.value("App/DataFileName",DEFAULT_FILE_NAME).toString();
    m_sLastSearchText = aSettings.value("App/LastSearchText").toString();
    m_iMaxLinesPerEntry = aSettings.value("App/MaxLinesPerEntry",DEFAULT_LINES_PER_ENTRY).toInt();    
    m_iMaxEntries = aSettings.value("App/MaxEntries",DEFAULT_MAX_ENTRIES).toInt();    
    ui->action_Double_click_to_select->setChecked(aSettings.value("App/DblClickToSelect",true).toBool());
    ui->actionAutoload_data->setChecked(aSettings.value("App/AutoLoadData",true).toBool());
    ui->actionAutoload_window_position_and_size->setChecked(aSettings.value("App/AutoWindowData",true).toBool());
    ui->actionAlways_on_top->setChecked(aSettings.value("App/AlwaysOnTop",false).toBool());
    if( ui->actionAutoload_window_position_and_size->isChecked() )
    {
        restoreState(aSettings.value("App/WindowState").toByteArray());
        restoreGeometry(aSettings.value("App/WindowGeometry").toByteArray());
    }
    aFont.fromString(aSettings.value("App/Font",QString("Courier")).toString());
    SetFont(aFont);
    return true;
}

bool CliphistWindow::Save()
{
    QFile aFile(m_sFileName);
    if( aFile.open(QIODevice::WriteOnly) )
    {
        QDataStream out(&aFile);
        out << "CLIPHIST2";
        out << "VERSION" << FILE_VERSION;
        out << m_aTxtHistory;
        SetDataChanged(false);
        return out.status()==QDataStream::Ok;
    }
    return false;
}

bool CliphistWindow::Load()
{
    if( QFile::exists(m_sFileName) )
    {
        QFile aFile(m_sFileName);
        if( aFile.open(QIODevice::ReadOnly) )
        {
            QDataStream in(&aFile);
            qint32 iVersion;
            QString s1,s2;
            in >> s1 >> s2 >> iVersion >> m_aTxtHistory;
            SetDataChanged(false);
            return in.status()==QDataStream::Ok;
        }
    }
    return false;
}

void CliphistWindow::ActivateEntry(int iIndex)
{
    ui->listWidget->setCurrentItem(m_aFindList[iIndex]);
    ActivateEntry(ui->listWidget->currentItem());
}

void CliphistWindow::ActivateEntry(QListWidgetItem * current)
{
    if( current )
    {
        UpdateColorOfLastSelectedItem();
        m_bMyCopy = true;
        UpdateLastSelectedItemData(current);
        m_pClipboard->setText(m_aTxtHistory[m_iActSelectedIndex]);
        m_bMyCopy = false;
    }
}

QBrush CliphistWindow::GetNextColor(const QBrush & aActColor) const
{
    return aActColor.color().name()==GREEN ? QBrush(BLUE) : QBrush(GREEN);
}

QBrush CliphistWindow::GetColorForIndex(int iIndex) const
{
    return (iIndex % 2)==1 ? QBrush(BLUE) : QBrush(GREEN);
}

QBrush CliphistWindow::GetColorOfNeighbour(int iMyIndex) const
{
    int iNeighbourIndex = -1;
    if( iMyIndex>0 )
    {
        iNeighbourIndex = iMyIndex-1;
    }
    else if( iMyIndex-1<ui->listWidget->count() )
    {
        iNeighbourIndex = iMyIndex+1;
    }
    if( iNeighbourIndex!=-1 )
    {
        return GetNextColor(ui->listWidget->item( iNeighbourIndex )->foreground());
    }
    return QBrush(BLUE);
}

void CliphistWindow::UpdateColorOfLastSelectedItem()
{
    if( m_iActSelectedIndex>=0 )
    {
        QBrush aColor = GetColorOfNeighbour(m_iActSelectedIndex);
        ui->listWidget->item(m_iActSelectedIndex)->setForeground(aColor);
        ui->listWidgetLineNumbers->item(m_iActSelectedIndex)->setForeground(aColor);
    }
}

void CliphistWindow::UpdateLastSelectedItemData(QListWidgetItem * current)
{
    m_iActSelectedIndex = ui->listWidget->row(current);
    current->setForeground(QBrush(RED));
    // create also a number entry
    QListWidgetItem * pNumber = ui->listWidgetLineNumbers->item(m_iActSelectedIndex);
    pNumber->setForeground(QBrush(RED));
}

void CliphistWindow::SetFont(const QFont & aFont)
{
    ui->listWidget->setFont(aFont);
    ui->listWidgetLineNumbers->setFont(aFont);
    // update width of the number list widget
    QFontMetrics aMetrics(aFont);
    ui->listWidgetLineNumbers->setMaximumWidth(aMetrics.boundingRect(QString("XXXXX")).width());
}

void CliphistWindow::SetDataChanged(bool bValue)
{
    m_bChangedData = bValue;
}

QString CliphistWindow::GetNewLine() const
{
    return "\n";
}
