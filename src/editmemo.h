#ifndef EDITMEMO_H
#define EDITMEMO_H

#include <QDialog>

namespace Ui {
class EditMemo;
}

class EditMemo : public QDialog
{
    Q_OBJECT

public:
    explicit EditMemo(QWidget *parent = nullptr, const QFont & aFont = QFont(), const QString & sText = "");
    ~EditMemo();

    QString getMemo() const;

public slots:
    void OnLoad();
    void OnSave();

private:
    Ui::EditMemo *ui;
};

#endif // EDITMEMO_H
