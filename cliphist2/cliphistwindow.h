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

#ifndef CLIPHISTWINDOW_H
#define CLIPHISTWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QStringList>
#include <QList>
#include <QByteArray>
#include <QPair>
#include <QClipboard>

#if defined(Q_OS_MAC)
#define _WITH_TIMER
#else
#undef _WITH_TIMER
#endif

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
    CliphistWindow(QWidget *parent = 0);
    ~CliphistWindow();

signals:
    void SelectionChanged(bool bAnySelected);

private slots:
    void OnLoadData();
    void OnSaveData();
    void OnSaveDataAs();
    void OnDeleteAllItems();
    void OnDeleteItem();
    void OnFindItem();
    void OnFindNextItem();
    void OnEditItem();
#if defined(_WITH_TIMER)
    void OnTimerUpdate();
#else
    void OnClipboardChanged(QClipboard::Mode);
#endif
    void OnClipboardDataChanged();
    void OnEraseClipboard();
    void OnSelectFont();
    void OnMaxItems();
    void OnLinesPerItem();
    void OnHelp();
    void OnAbout();
    void OnAboutQt();
    void OnToggleAlwaysOnTop(bool bChecked);
    void OnItemClicked(QListWidgetItem * current);
    void OnItemDoubleClicked(QListWidgetItem * current);
    void OnItemActivated(QListWidgetItem * current);    
    void OnSelectionChanged();

private:
    bool SaveSettings();
    bool LoadSettings();
    bool Save();
    bool Load();
    bool SyncListWithUi(int iSelectIdx=-1);
    bool IsActClipboardEntrySameAsActivatedItem() const;
    bool IsActClipboardEntryEmpty() const;
    bool IsAnyItemActivated() const;
    void LoadAndCheck();
    void SaveAndCheck();
    void ActivateItem(int iIndex);
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
public:
    int GetIndexOfActSelected() const;
    QString RemoveGiven(int iIndexOfSelectedItem = -1);
    //int UpdateOrInsertList(int iPosition, const QString & sText, bool bUpdate);
    int UpdateList(int iPosition, const QString & sText);
    int InsertInList(int iPosition, const QString & sText);
    void UpdateSelection(int iCurrentRow);
    void DoDeleteAllItems();
    void UndoDeleteAllItems(const QStringList & aHistory, int iActSelected);


private:   /*data*/
    Ui::CliphistWindow *        ui;

    QUndoStack *                m_pUndoStack;
    QClipboard *                m_pClipboard;           // temp
    QTimer *                    m_pTimer;               // temp
    QList<QListWidgetItem *>    m_aFindList;            // temp
    QByteArray                  m_aEditDialogGeometry;  // temp
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
