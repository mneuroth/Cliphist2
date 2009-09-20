/*
    Started porting to Qt: 12.9.2009    
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

#define FILE_VERSION                1
#define DEFAULT_FILE_NAME           "cliphist2.dat"
#define DEFAULT_LINES_PER_ENTRY     3

// TODO --> globalen Key definieren und auf diese Anwendung umleiten moeglich ?
// TODO: Beschraenkung auf 100 Eintraege (oder n Eintraege, konfigurierbar)
// TODO: Eintrags-Nr anzeigen
// TODO: aktuellen Eintrag andersfarbig hervorheben (rot)
// TODO: ggf. Option allways on top
// TODO: selektion des Eintrags (rot) nur durch doppelclick (oder auswählbar machen)

CliphistWindow::CliphistWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CliphistWindow)
{
    ui->setupUi(this);
//    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    
    QCoreApplication::setOrganizationName("MNeuroth");
    QCoreApplication::setOrganizationDomain("mneuroth.de");
    QCoreApplication::setApplicationName("ClipHist2");    
    
    m_pClipboard        = QApplication::clipboard();    
    m_iFindIndex        = -1;
    m_bMyCopy           = false;
    m_bColorToggle      = false;    
    m_iMaxLinesPerEntry = DEFAULT_LINES_PER_ENTRY;
    m_sFileName         = DEFAULT_FILE_NAME;
    
    LoadSettings();
    Load();
    SyncListWithUi();
   
    connect(m_pClipboard, SIGNAL(dataChanged()), this, SLOT(OnClipboardDataChanged()));
    connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *)), this, SLOT(OnCurrentItemChanged(QListWidgetItem *,QListWidgetItem *)));
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OnLoadData()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(OnSaveData()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(OnSaveDataAs()));
    connect(ui->actionDelete_all_entries, SIGNAL(triggered()), this, SLOT(OnDeleteAllItems()));
    connect(ui->actionDelete_marked_entry, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));
    connect(ui->action_Find_text, SIGNAL(triggered()), this, SLOT(OnFindItem()));
    connect(ui->actionFind_next, SIGNAL(triggered()), this, SLOT(OnFindNextItem()));
    connect(ui->action_Edit_entry, SIGNAL(triggered()), this, SLOT(OnEditItem()));
    connect(ui->actionErase_clipboard_contents, SIGNAL(triggered()), this, SLOT(OnEraseClipboard()));    
    connect(ui->actionSelect_font, SIGNAL(triggered()), this, SLOT(OnSelectFont()));    
    connect(ui->action_Lines_per_entry, SIGNAL(triggered()), this, SLOT(OnLinesPerEntry()));    
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(OnHelp()));    
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));    
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(OnAboutQt()));
    connect(ui->actionAllways_on_top, SIGNAL(triggered(bool)), this, SLOT(OnToggleAllwaysOnTop(bool)));
}

CliphistWindow::~CliphistWindow()
{
    Save();
    SaveSettings();
    delete ui;
}

void CliphistWindow::OnHelp()
{
    QMessageBox::about(this,tr("Clipboard history"),tr("Sorry, no help available yet !"));
}

void CliphistWindow::OnToggleAllwaysOnTop(bool bChecked)
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
    QMessageBox::about(this,tr("Clipboard history"),tr("Clipboard history 2, (c) by Michael Neuroth"));
}

void CliphistWindow::OnAboutQt()
{
    QMessageBox::aboutQt(this,tr("Clipboard history"));
}

void CliphistWindow::OnDeleteAllItems()
{
    QMessageBox::StandardButtons aButton = QMessageBox::question(this,tr("Question"),tr("Really delete all items in the list ?"),QMessageBox::Yes|QMessageBox::No);
    if( aButton == QMessageBox::Yes )
    {
        m_aTxtHistory.clear();
        SyncListWithUi();
    }
}

void CliphistWindow::OnDeleteItem()
{
    QList<QListWidgetItem *> aList = ui->listWidget->selectedItems();
    if( aList.size()>0 )
    {
        m_aTxtHistory.removeAt(ui->listWidget->row(aList[0]));
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
            SyncListWithUi();
        }
    }        
}

void CliphistWindow::OnClipboardDataChanged()
{
    if( !m_bMyCopy && !(m_pClipboard->text().isEmpty() || m_pClipboard->text().isNull()) )
    {
        m_aTxtHistory.insert(0,m_pClipboard->text());
        ui->listWidget->insertItem(0,CreateNewItem(m_pClipboard->text()));
    }
}

void CliphistWindow::OnCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
    m_bMyCopy = true;
    if( current )
    {
        m_pClipboard->setText(current->text());
    }
    m_bMyCopy = false;
}

void CliphistWindow::OnLoadData()
{
// TODO abfrage ob daten geaendert wurden...    
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
        ui->listWidget->setFont(aFont);
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
    QListWidgetItem * pItem = new QListWidgetItem(FilterForDisplay(s));
    pItem->setForeground( m_bColorToggle ? QBrush("blue") : QBrush("green") );
    //pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    pItem->setToolTip(s);
    m_bColorToggle = !m_bColorToggle;
    return pItem;
}

QString CliphistWindow::FilterForDisplay(const QString & s) const
{
    QString sRet;
    QStringList aList = s.split("\n");
    int iMax = qMin(m_iMaxLinesPerEntry,aList.size());
    for( int i=0; i<iMax; i++ )
    {
        sRet += aList[i];
        if( i<iMax-1 )
        {
            sRet += '\n';
        }
    }
    return sRet;
}

bool CliphistWindow::SyncListWithUi()
{
    ui->listWidget->clear();
    for( int i=m_aTxtHistory.size()-1; i>=0; i-- )
    {
        ui->listWidget->insertItem(0,CreateNewItem(m_aTxtHistory[i]));
    }
    return true;
}

bool CliphistWindow::SaveSettings()
{
    QSettings aSettings;
    
    aSettings.setValue("App/DataFileName",m_sFileName);
    aSettings.setValue("App/LastSearchText",m_sLastSearchText);
    aSettings.setValue("App/MaxLinesPerEntry",m_iMaxLinesPerEntry);    
    aSettings.setValue("App/WindowState",saveState());
    aSettings.setValue("App/WindowGeometry",saveGeometry());
    aSettings.setValue("App/Font",ui->listWidget->font().toString());
    
    return true;
}

bool CliphistWindow::LoadSettings()
{
    QSettings aSettings;
    QFont aFont;
    
    m_sFileName = aSettings.value("App/DataFileName",DEFAULT_FILE_NAME).toString();
    m_sLastSearchText = aSettings.value("App/LastSearchText").toString();
    m_iMaxLinesPerEntry = aSettings.value("App/MaxLinesPerEntry",DEFAULT_LINES_PER_ENTRY).toInt();    
    restoreState(aSettings.value("App/WindowState").toByteArray());
    restoreGeometry(aSettings.value("App/WindowGeometry").toByteArray());
    aFont.fromString(aSettings.value("App/Font",QString("Courier")).toString());
    ui->listWidget->setFont(aFont);

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
    return in.status()==QDataStream::Ok;
}

void CliphistWindow::ActivateEntry(int iIndex)
{
    ui->listWidget->setCurrentItem(m_aFindList[iIndex]);
}

