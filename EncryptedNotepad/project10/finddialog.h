#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

signals:
    void findTextDataButtonClickedSignal(QString,bool,bool);

private slots:
    void findButtonState();
    void findDataButtonClickedState();

private:
    Ui::FindDialog *ui;


};

#endif // FINDDIALOG_H
