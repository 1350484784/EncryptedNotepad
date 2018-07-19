#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = 0);
    ~ReplaceDialog();

signals:
    void replaceAllStr(QString , QString,bool);
    void replaceCurOneStr(QString,QString,bool);
    void findReplaceStr(QString,bool);

private slots:
    void chooseStrSlot();
    void findButtonStateSlot();
    void replaceOneButtonStateSlot();
    void replaceAllButtonStateSlot();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
