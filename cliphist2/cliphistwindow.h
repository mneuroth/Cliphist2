#ifndef CLIPHISTWINDOW_H
#define CLIPHISTWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QStringList>
#include <QList>
#include <QByteArray>

class QClipboard;
class QListWidgetItem;
    
namespace Ui
{
    class CliphistWindow;
}

class CliphistWindow : public QMainWindow
{
    Q_OBJECT

public:
    CliphistWindow(QWidget *parent = 0);
    ~CliphistWindow();

private slots:
    void OnLoadData();
    void OnSaveData();
    void OnSaveDataAs();
    void OnDeleteAllItems();
    void OnDeleteItem();
    void OnFindItem();
    void OnFindNextItem();
    void OnEditItem();
    void OnClipboardDataChanged();
    void OnEraseClipboard();
    void OnSelectFont();
    void OnLinesPerEntry();
    void OnHelp();
    void OnAbout();
    void OnAboutQt();

    void OnToggleAllwaysOnTop(bool bChecked);

    void OnItemDoublecClicked(QListWidgetItem * current);
    void OnItemActivated(QListWidgetItem * current);

private:
    bool SaveSettings();
    bool LoadSettings();
    bool Save();
    bool Load();
    bool SyncListWithUi();
    QListWidgetItem * CreateNewItem(const QString & s);
    QString FilterForDisplay(const QString & s) const;
    QString FilterNumber(const QString & s, const QString & sNumber) const;
    void ActivateEntry(int iIndex);   
    void ActivateEntry(QListWidgetItem * current);
    void UpdateColorOfLastSelectedItem();
    void UpdateLastSelectedItemData(QListWidgetItem * current);
    void SetFont(const QFont & aFont);
    void InsertNewData(const QString & sText, int iNumber);

private:   /*data*/
    Ui::CliphistWindow *        ui;

    QClipboard *                m_pClipboard;
    bool    	                m_bMyCopy;              // temp
    bool                        m_bColorToggle;         // temp
    QList<QListWidgetItem *>    m_aFindList;            // temp
    int                         m_iFindIndex;           // temp
    QByteArray                  m_aEditDialogGeometry;  // temp
    int                         m_iActSelectedIndex;    // temp
    QBrush                      m_aLastColor;           // temp
    int                         m_iMaxLinesPerEntry;    
    QString                     m_sFileName;
    QString                     m_sLastSearchText;
    QStringList                 m_aTxtHistory;
};

#endif // CLIPHISTWINDOW_H
