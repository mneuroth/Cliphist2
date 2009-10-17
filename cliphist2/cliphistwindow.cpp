/*
    Initial Version for OS/2:         1993
    Ported to Windows XP:             2001
    Started porting to Qt:       12.9.2009    (Mac, Linux, Windows)
    
    Translation:
       lupdate Cliphist2.pro    *.cpp --> *.ts
       linguist cliphist_de.ts
       lrelease Cliphist2.pro   *.ts  --> *.qm
       
    *.pro file must contain something like: TRANSLATIONS = cliphist_de.ts
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

#define FILE_VERSION                1
#define DEFAULT_FILE_NAME           "cliphist2.dat"
#define DEFAULT_LINES_PER_ENTRY     3
#define DEFAULT_MAX_ENTRIES         100

// TODO: ggf. globalen Key definieren und auf diese Anwendung umleiten moeglich ?
// TODO: ggf. nicht auswaehlbare Menueintrage disablen (Find next, Edit Item, Delete act Item)
// TODO: Icon erstellen
// TODO: Installer fuer die einzelnen Platformen erstellen: Mac, Windows, Linux
// (TODO: in der Spalte für Nummern durch ... signalisieren das mehr Zeilen als sichtbar sind als Eintrag vorhanden sind
// (TODO: Beschraenkung auf 100 Eintraege (oder n Eintraege, konfigurierbar)
// (TODO: Internationalisierung durchfuehren
// (TODO: ggf. mehr Einstellungen der Anwendung speichern und restaurieren (z.B. always on top)
// (TODO: Eintrags-Nr anzeigen
// (TODO: aktuellen Eintrag andersfarbig hervorheben (rot)
// (TODO: ggf. Option always on top
// (TODO: selektion des Eintrags (rot) nur durch doppelclick (oder auswählbar machen)

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
    m_bColorToggle      = false;    
    m_iActSelectedIndex = -1;
    m_iMaxEntries       = DEFAULT_MAX_ENTRIES;
    m_iMaxLinesPerEntry = DEFAULT_LINES_PER_ENTRY;
    m_sFileName         = DEFAULT_FILE_NAME;
    
    LoadSettings();
    if( ui->actionAutoload_data->isChecked() )
    {
        Load();
    }
    OnToggleAlwaysOnTop(ui->actionAlways_on_top->isChecked());
    SyncListWithUi();    
        
    connect(m_pClipboard, SIGNAL(dataChanged()), this, SLOT(OnClipboardDataChanged()));
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
    
    OnSelectionChanged();
}

CliphistWindow::~CliphistWindow()
{
    if( ui->actionAutoload_data->isChecked() )
    {
        Save();
    }
    SaveSettings();
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
    QMessageBox::about(this,tr("About Application"),tr("Clipboard History 2\n\n(c) 1993-2009 by Michael Neuroth"));
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
    QList<QListWidgetItem *> aList = ui->listWidget->selectedItems();
    if( aList.size()>0 )
    {
        m_aTxtHistory.removeAt(ui->listWidget->row(aList[0]));
        SetDataChanged(true);
        SyncListWithUi();
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
    QList<QListWidgetItem *> aList = ui->listWidget->selectedItems();
    if( aList.size()==1 )
    {
        EditItem aDlg(this,aList[0]->text());
        if( m_aEditDialogGeometry.count()>0 )
        {
            aDlg.restoreGeometry(m_aEditDialogGeometry);
        }
        aDlg.setFont(ui->listWidget->font());
        if( aDlg.exec()==QDialog::Accepted )
        {
            m_aEditDialogGeometry = aDlg.saveGeometry();
            m_aTxtHistory[ui->listWidget->row(aList[0])] = aDlg.text();
            SetDataChanged(true);
            SyncListWithUi();
        }
    }        
}

void CliphistWindow::OnClipboardDataChanged()
{
    if( !m_bMyCopy && !(m_pClipboard->text().isEmpty() || m_pClipboard->text().isNull()) && ui->action_Activate_cliphist->isChecked() )
    {
        UpdateColorOfLastSelectedItem();
        m_aTxtHistory.insert(0,m_pClipboard->text());
        while( m_aTxtHistory.size()>m_iMaxEntries )
        {
            m_aTxtHistory.removeLast();
        }
        SetDataChanged(true);
        //InsertNewData(m_pClipboard->text(),1);        
        SyncListWithUi();        
        UpdateLastSelectedItemData(ui->listWidget->item(0));        // update of clipboard data always into index 0 !
    }
}

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
    emit SelectionChanged(ui->listWidget->selectedItems().size()>0);
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
        Load();
        SyncListWithUi();
    }
}

void CliphistWindow::OnSaveData()
{
    Save();
}

void CliphistWindow::OnSaveDataAs()
{
    QString sFileName = QFileDialog::getSaveFileName(this,tr("Save Data as"), m_sFileName, tr("Data Files (*.dat)"));    
    if( !sFileName.isEmpty() )
    {
        m_sFileName = sFileName;
        Save();
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

QListWidgetItem * CliphistWindow::CreateNewItem(const QString & s)
{
    QPair<QString,bool> aResult = FilterForDisplay(s);
    QListWidgetItem * pItem = new QListWidgetItem(aResult.first,/*parent*/0,aResult.second ? 1 : 0);
    pItem->setForeground( m_bColorToggle ? QBrush("blue") : QBrush("green") );
    //pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    pItem->setToolTip(s);
    m_bColorToggle = !m_bColorToggle;
    return pItem;
}

QPair<QString,bool> CliphistWindow::FilterForDisplay(const QString & s) const
{
    QString sRet;
    QStringList aList = s.split("\n");
    int iMax = qMin(m_iMaxLinesPerEntry,aList.size());
    bool bMoreLines = m_iMaxLinesPerEntry<aList.size();
    for( int i=0; i<iMax; i++ )
    {
        sRet += aList[i]+'\n';
    }
    return QPair<QString,bool>(sRet.mid(0,sRet.size()-1),bMoreLines);   // remove last \n from return string;
}

QString CliphistWindow::FilterNumber(const QString & s, const QString & sNumber, bool bMoreLines ) const
{
    QString sRet;
    QStringList aList = s.split("\n");
    for( int i=0; i<aList.size(); i++ )
    {
        if( bMoreLines && i==aList.size()-1 )
        {
            sRet += "...\n";
        }
        else
        {
            sRet += i==0 ? sNumber+"\n" : "\n";
        }
    }
    return sRet.mid(0,sRet.size()-1);   // remove last \n from return string
}

bool CliphistWindow::SyncListWithUi()
{
    ui->listWidget->clear();
    ui->listWidgetLineNumbers->clear();
    for( int i=m_aTxtHistory.size()-1; i>=0; i-- )
    {
        InsertNewData(m_aTxtHistory[i],i+1);
    }
    return true;
}

void CliphistWindow::InsertNewData(const QString & sText, int iNumber)
{
    QListWidgetItem * pNewItem = CreateNewItem(sText);
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
    ui->action_Double_click_to_select->setChecked(aSettings.value("App/DblClickToSelect").toBool());
    ui->actionAutoload_data->setChecked(aSettings.value("App/AutoLoadData").toBool());
    ui->actionAutoload_window_position_and_size->setChecked(aSettings.value("App/AutoWindowData").toBool());
    ui->actionAlways_on_top->setChecked(aSettings.value("App/AlwaysOnTop").toBool());
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
    aFile.open(QIODevice::WriteOnly);
    QDataStream out(&aFile);
    out << "CLIPHIST2";
    out << "VERSION" << FILE_VERSION;
    out << m_aTxtHistory;
    SetDataChanged(false);
    return out.status()==QDataStream::Ok;
}

bool CliphistWindow::Load()
{
    QFile aFile(m_sFileName);
    aFile.open(QIODevice::ReadOnly);
    QDataStream in(&aFile);
    qint32 iVersion;
    QString s1,s2;
    in >> s1 >> s2 >> iVersion >> m_aTxtHistory;
    SetDataChanged(false);
    return in.status()==QDataStream::Ok;
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
        m_pClipboard->setText(current->text());
        m_bMyCopy = false;
    }
}

void CliphistWindow::UpdateColorOfLastSelectedItem()
{
    if( m_iActSelectedIndex>=0 )
    {
        ui->listWidget->item(m_iActSelectedIndex)->setForeground(m_aLastColor);
        ui->listWidgetLineNumbers->item(m_iActSelectedIndex)->setForeground(m_aLastColor);
    }
}

void CliphistWindow::UpdateLastSelectedItemData(QListWidgetItem * current)
{
    m_iActSelectedIndex = ui->listWidget->row(current);
    m_aLastColor = current->foreground();
    current->setForeground(QBrush("red"));
    // create also a number entry
    QListWidgetItem * pNumber = ui->listWidgetLineNumbers->item(m_iActSelectedIndex);
    pNumber->setForeground(QBrush("red"));
}

void CliphistWindow::SetFont(const QFont & aFont)
{
    ui->listWidget->setFont(aFont);
    ui->listWidgetLineNumbers->setFont(aFont);
    // update width of the number list widget
    QFontMetrics aMetrics(aFont);
    ui->listWidgetLineNumbers->setMaximumWidth(aMetrics.boundingRect(QString("XXXX")).width());
}

void CliphistWindow::SetDataChanged(bool bValue)
{
    m_bChangedData = bValue;
}
