#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QLabel>
#include <QTextCursor>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QPalette>
#include <QTextDocument>
#include <QByteArray>


#include "finddialog.h"
#include "replacedialog.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    //File 菜单项 槽
    void slot_newFile();
    bool maybeSave();
    void setFileName(QString fileName);
    bool slot_save();
    bool slot_saveAs();
    bool save_file(QString fileName);
    void slot_open();
    void loadFile(QString fileName);
    void quitApp();
    void closeEvent(QCloseEvent *e);

    //Edit 编辑项 槽
    void slot_cut();
    void slot_copy();
    void slot_paste();
    void slot_redo();
    void slot_undo();
    void slot_selectAll();

    //Format 字体 槽
    void changeFontFamily();
    void changeFontColor();

    //显示坐标
    void showTextInfo();

    //Tool 槽
    void find(QString str,bool ignore,bool choose);
    void showFindDialog();
    void showReplaceDialog();
    void findReplace(QString find_str, bool flag);
    void replaceCur(QString,QString,bool);
    void replaceAll(QString,QString,bool);



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString currentFile;

    QLabel *textInfo; //位于右下角的显示坐标

    QFontDialog fontDlg; //字体选择对话框，可以选择字体，字体样式，大小
    QFont font;

    QColorDialog colorDlg; //颜色选择对话框
    QColor color;

    bool findFlag;
    FindDialog *find_dialog;
    ReplaceDialog *replace_dialog;



};

#endif // MAINWINDOW_H
