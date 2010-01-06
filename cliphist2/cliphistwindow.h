/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2010 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: E:/home/cvsroot/visiscript/visiscript.cpp,v $
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

class QClipboard;
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
    void OnClipboardDataChanged();
    void OnClipboardChanged(QClipboard::Mode);
#if defined(Q_OS_MAC)
    void OnTimerUpdate();
#endif
    void OnEraseClipboard();
    void OnSelectFont();
    void OnMaxEntries();
    void OnLinesPerEntry();
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
    bool SyncListWithUi();
    void ActivateEntry(int iIndex);   
    void ActivateEntry(QListWidgetItem * current);
    void UpdateColorOfLastSelectedItem();
    void UpdateLastSelectedItemData(QListWidgetItem * current);
    void SetFont(const QFont & aFont);
    void InsertNewData(const QString & sText, int iNumber);
    void SetDataChanged(bool bValue);
    QString GetNewLine() const;
    QListWidgetItem * CreateNewItem(const QString & s);
    QPair<QString,bool> FilterForDisplay(const QString & s) const;
    QString FilterNumber(const QString & s, const QString & sNumber, bool bMoreLines) const;

private:   /*data*/
    Ui::CliphistWindow *        ui;

    QClipboard *                m_pClipboard;
    QTimer *                    m_pTimer;
    bool                        m_bChangedData;         // temp
    bool    	                m_bMyCopy;              // temp
    bool                        m_bColorToggle;         // temp
    QList<QListWidgetItem *>    m_aFindList;            // temp
    int                         m_iFindIndex;           // temp
    QByteArray                  m_aEditDialogGeometry;  // temp
    int                         m_iActSelectedIndex;    // temp
    QBrush                      m_aLastColor;           // temp
    int                         m_iMaxEntries;
    int                         m_iMaxLinesPerEntry;    
    QString                     m_sFileName;
    QString                     m_sLastSearchText;
    QStringList                 m_aTxtHistory;
};

#endif // CLIPHISTWINDOW_H
