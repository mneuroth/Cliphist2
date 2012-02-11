/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2012 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				 *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				 *
 * Copyright (C) 2009-2012 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef CLIPHISTWINDOW_H
#define CLIPHISTWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QStringList>
#include <QList>
#include <QByteArray>
#include <QPair>
#include <QClipboard>

class QUndoStack;
class QTimer;
class QListWidgetItem;
    
namespace Ui
{
    class CliphistWindow;
}

class CliphistWindow : public QMainWindow
{
    Q_OBJECT

public:
    CliphistWindow(const QString sFileName = QString::null, QWidget *parent = 0);
    ~CliphistWindow();

signals:
    void SelectionChanged(bool bAnySelected);
    void JustOneSelected(bool bJustOneSelected);
    void MoreThanOneSelected(bool bMoreThanOneSelected);

private slots:
    void OnLoadData();
    void OnSaveData();
    void OnSaveDataAs();
    void OnDeleteAllItems();
    void OnDeleteItem();
    void OnFindItem();
    void OnFindNextItem();
    void OnMoveSelectedEntryToTop();
    void OnAlwaysMoveToTopIfFound(bool bChecked);
    void OnAddSelectedToNewEntry();
    void OnEditItem();
    void OnTimerUpdate();
    void OnClipboardChanged(QClipboard::Mode);
    void OnClipboardDataChanged();
    void OnEraseClipboard();
    void OnSelectFont();
    void OnMaxItems();
    void OnLinesPerItem();
    void OnHelp();
    void OnAbout();
    void OnAboutQt();
    void OnSetDefaultPosSize();
    void OnToggleAlwaysOnTop(bool bChecked);
    void OnToggleUseTimer(bool bChecked);
    void OnItemClicked(QListWidgetItem * current);
    void OnItemDoubleClicked(QListWidgetItem * current);
    void OnItemActivated(QListWidgetItem * current);    
    void OnSelectionChanged();

private:
    bool SaveSettings();
    bool LoadSettings();
    bool Save();
    bool Load(const QString sFileName);
    bool SyncListWithUi(const QList<int> & aSelectIdx);
    bool IsActClipboardEntryPixmap() const;
    bool IsActClipboardEntrySameAsActivatedItem() const;
    bool IsActClipboardEntryEmpty() const;
    bool IsAnyItemActivated() const;
    void LoadAndCheck();
    void SaveAndCheck();
    void ActivateItemIdx(int iIndex);
    void ActivateItem(QListWidgetItem * current);
    void UpdateColorOfLastActivatedItem();
    void UpdateLastActivatedItemData(QListWidgetItem * current);
    void SetFont(const QFont & aFont);
    void InsertNewUiData(const QString & sText, int iNumber);
    void CheckHistoryMemory();
    void SetDataChanged(bool bValue);
    QBrush GetColorOfNeighbour(int iMyIndex) const;
    QBrush GetNextColor(const QBrush & aActColor) const;
    QBrush GetColorForIndex(int iIndex) const;
    QListWidgetItem * CreateNewItem(const QString & s, const QBrush & aBrush);
    QPair<QString,bool> FilterForDisplay(const QString & s) const;
    QString FilterNumber(const QString & s, const QString & sNumber, bool bMoreLines) const;
    QString GetNewLine() const;
    int FindItemIndex(const QString & sItemText) const;
    QList<int> FindTextInHistory(const QString & sFindText) const;
    bool FindPixmap(const QPixmap & aPixmap, int * pIndex = 0) const;
    bool FindPixmap(const QString & sHash, int * pIndex) const;
    void RemovePixmapWithHash(const QString & sHash);
    bool FindPixmapInTextHistory(const QString & sHash, int * pIndex);
    void CleanUpUnusedPixmaps();

public:
    void LoadFileAndSync(const QString sFileName);
//    int GetIndexOfActSelected() const;
    QList<int> GetAllIndicesOfActSelected(bool bSort=true) const;
    QString RemoveGiven(int iIndexOfSelectedItem = -1);
    //int UpdateOrInsertList(int iPosition, const QString & sText, bool bUpdate);
    int UpdateList(int iPosition, const QString & sText);
    int InsertInList(int iPosition, const QString & sText, bool bUpdateSelection=false);
    void UpdateSelection(const QList<int> & aCurrentRows);
    void DoDeleteAllItems();
    void UndoDeleteAllItems(const QStringList & aHistory, const QList<int> & aActSelected);

private:   /*data*/
    Ui::CliphistWindow *        ui;

    QUndoStack *                m_pUndoStack;
    QClipboard *                m_pClipboard;           // temp
    QTimer *                    m_pTimer;               // temp
    QList<QPair<QString,QPixmap> > m_aPixmapList;
    QList<int>                  m_aFindList;            // temp
    QByteArray                  m_aEditDialogGeometry;  // temp
    bool                        m_bIfFoundMoveToFirstPos; // temp
    bool                        m_bChangedData;         // temp
    bool    	                m_bMyClipboardCopy;     // temp, flag to show that the current clipboard operation comes from this application
    int                         m_iFindIndex;           // temp
    int                         m_iActivatedIndex;      // temp --> TODO: maybe use ui->listWidget->currentItem() ? ==> No, currentItem()!=activatedItem
    int                         m_iMaxEntries;
    int                         m_iMaxLinesPerEntry;    
    QString                     m_sFileName;
    QString                     m_sLastSearchText;
    QStringList                 m_aTxtHistory;
};

#endif // CLIPHISTWINDOW_H
