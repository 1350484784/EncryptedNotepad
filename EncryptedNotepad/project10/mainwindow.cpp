#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    find_dialog = new FindDialog;  //不实例化，就报野指针的错
    replace_dialog = new ReplaceDialog;

    ui->textEdit->setFont(QFont("宋体", 15));//设置默认字体：字体，字体大小
    setCentralWidget(ui->textEdit);//设置文本区域为整个窗口的中央部件


    textInfo = new QLabel();
    textInfo->setText("Ready");//设置默认显示内容
    statusBar()->addPermanentWidget(textInfo);//添加到状态栏,在软件右下角



    //设置快捷键
    ui->newAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->saveAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    ui->undoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    ui->redoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    ui->cutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    ui->copyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    ui->pasteAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    ui->findAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));

    //设置槽与信号
    connect(ui->newAct,SIGNAL(triggered()),this,SLOT(slot_newFile()));
    connect(ui->openAct,SIGNAL(triggered()),this,SLOT(slot_open()));
    connect(ui->saveAct,SIGNAL(triggered()),this,SLOT(slot_save()));
    connect(ui->saveasAct,SIGNAL(triggered()),this,SLOT(slot_saveAs()));
    connect(ui->quitAct,SIGNAL(triggered()),this,SLOT(quitApp()));

    connect(ui->undoAct,SIGNAL(triggered()),this,SLOT(slot_undo()));
    connect(ui->redoAct,SIGNAL(triggered()),this,SLOT(slot_redo()));
    connect(ui->cutAct,SIGNAL(triggered()),this,SLOT(slot_cut()));
    connect(ui->copyAct,SIGNAL(triggered()),this,SLOT(slot_copy()));
    connect(ui->pasteAct,SIGNAL(triggered()),this,SLOT(slot_paste()));
    connect(ui->selectAllAct,SIGNAL(triggered()),this,SLOT(slot_selectAll()));

    connect(ui->fontAct,SIGNAL(triggered()),this,SLOT(changeFontFamily()));
    connect(ui->colorAct,SIGNAL(triggered()),this,SLOT(changeFontColor()));

    connect(ui->findAct,SIGNAL(triggered()),this,SLOT(showFindDialog()));
    connect(find_dialog,SIGNAL(findTextDataButtonClickedSignal(QString,bool,bool)),this,SLOT(find(QString,bool,bool)));
    connect(ui->replaceAct,SIGNAL(triggered()),this,SLOT(showReplaceDialog()));
    connect(replace_dialog,SIGNAL(findReplaceStr(QString,bool)),this,SLOT(findReplace(QString,bool)));
    connect(replace_dialog,SIGNAL(replaceCurOneStr(QString,QString,bool)),this,SLOT(replaceCur(QString,QString,bool)));
    connect(replace_dialog,SIGNAL(replaceAllStr(QString,QString,bool)),this,SLOT(replaceAll(QString,QString,bool)));


    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(showTextInfo()));

}


MainWindow::~MainWindow(){
    delete ui;
}

/**
 * @brief MainWindow::slot_newFile 新建文件
 */
void MainWindow::slot_newFile(){
    if(maybeSave()){
        ui->textEdit->clear();
        setFileName("");
    }
}

/**
 * @brief MainWindow::maybeSave 是否保存
 * @return
 */
bool MainWindow::maybeSave(){
    if(ui->textEdit->document()->isModified()){
        QMessageBox::StandardButtons result;
        result = QMessageBox::warning(this,"Waring","Do you want to save the file?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(result == QMessageBox::Cancel){//取消
            return false;
        }
        if(result == QMessageBox::Save){//保存
            return slot_save();
        }
        if(result == QMessageBox::Discard){//忽略，即不保存
            return true;
        }
    }else{
        return true;
    }
}

/**
 * @brief MainWindow::setFileName 设置文本的名称
 * @param fileName
 */
void MainWindow::setFileName(QString fileName){
    currentFile = fileName;
    ui->textEdit->document()->setModified(false);
    this->setWindowModified(false);
    fileName.isEmpty() ?
                this->setWindowTitle("new.txt") :
                this->setWindowTitle(fileName);
}

/**
 * @brief MainWindow::slot_save 保存
 * @return
 */
bool MainWindow::slot_save(){
    if(currentFile.isEmpty()){
        return slot_saveAs();
    } else{
        return save_file(currentFile);
    }
}

/**
 * @brief MainWindow::slot_saveAs 另存为
 * @return
 */
bool MainWindow::slot_saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save As"),
                                                    "/home/new",tr("Text Files (*.txt)"));
    if(fileName.isEmpty()){
        return false;
    }else{
        return save_file(fileName);
    }
}

/**
 * @brief MainWindow::save_file 保存文件
 * @param fileName
 * @return
 */
bool MainWindow::save_file(QString fileName){
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::critical(this,
                              "critical",
                              "cannot write file"
                              );
        return false;
    }else{
        QTextStream out(&file);
        //out.setCodec("UTF-8");
        //out<<ui->textEdit->toPlainText();



        QString str = ui->textEdit->toPlainText().toUtf8().toBase64();
        QByteArray ba = str.toLatin1();//toLatin1().toBase64();
        //ba = QByteArray::toBase64(QByteArray::Base64Encoding);
        out<<ba;

        out<<tr("\n这是我们之间的秘密哦");     //设置加密的标志
        //out<<123;
        setFileName(fileName);

        return true;
    }
}


/**
 * @brief MainWindow::slot_open 打开文件
 */
void MainWindow::slot_open(){
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()){
            loadFile(fileName);
        }
    }
}

/**
 * @brief MainWindow::loadFile 加载文件的函数
 * @param fileName
 */
void MainWindow::loadFile(QString fileName){
    QFile file(fileName);

    QString str = tr("这是我们之间的秘密哦");
    if(!file.open(QFile::ReadWrite | QFile::Text)){
        QMessageBox::critical(this,
                              "critical",
                              "cannot read file"
                              );
    }else{
        QTextStream in(&file);
        //in.setCodec("UTF-8");
        //ui->textEdit->setText(in.readAll());

        //定位到最后一行，判断是不是加密文件
        QString line;
        QStringList qst;
        while(!in.atEnd()){
            line = in.readLine();
            qst.append(line);
        }
        //qDebug()<<line;

        if(line == str){
            QString temp;
            for(int i=0;i<qst.size()-1;i++){

                temp.append(qst.at(i));
            }
            QByteArray ba = temp.toLatin1();
            ba = QByteArray::fromBase64(ba);
            ui->textEdit->setText(ba);
        }else{
            in.seek(0);
            ui->textEdit->setText(in.readAll());
        }

        //        QByteArray ba = QByteArray::fromBase64(file.readAll());
        //        ui->textEdit->setText(ba);
        setFileName(fileName);
    }
}

/**
 * @brief MainWindow::quitApp 退出
 */
void MainWindow::quitApp(){
    if(maybeSave()){
        qApp->closeAllWindows();//关闭所有窗口
    }
}

void MainWindow::closeEvent(QCloseEvent *e){
    if(maybeSave()){
        e->accept();
        qApp->closeAllWindows();//关闭所有窗口
    }else{
        e->ignore();//忽略，直接关闭
        qApp->closeAllWindows();//关闭所有窗口
    }
}
/********************************************************/
/**
 * @brief MainWindow::slot_cut 剪切
 */
void MainWindow::slot_cut(){
    ui->textEdit->cut();
}

/**
 * @brief MainWindow::slot_copy 复制
 */
void MainWindow::slot_copy(){
    ui->textEdit->copy();
}

/**
 * @brief MainWindow::slot_paste 粘贴
 */
void MainWindow::slot_paste(){
    ui->textEdit->paste();
}

/**
 * @brief MainWindow::slot_redo 重做
 */
void MainWindow::slot_redo(){
    ui->textEdit->redo();
}

/**
 * @brief MainWindow::slot_undo 撤销
 */
void MainWindow::slot_undo(){
    ui->textEdit->undo();
}

/**
 * @brief MainWindow::slot_selectAll 全选
 */
void MainWindow::slot_selectAll(){
    ui->textEdit->selectAll();
}
/*********************************************************/

/**
 * @brief MainWindow::changeFontFamily 字体
 */
void MainWindow::changeFontFamily(){
    bool isChanged;

    font = fontDlg.getFont(&isChanged);
    if(isChanged){
        ui->textEdit->setFont(font);
    }
}

/**
 * @brief MainWindow::changeFontColor 颜色
 */
void MainWindow::changeFontColor(){
    QPalette palette = ui->textEdit->palette();//获得文本区域的调色板

    color = colorDlg.getColor(Qt::black);//默认是黑色
    if (color.isValid()){
        palette.setColor(QPalette::Text, color);//设置字体颜色
        ui->textEdit->setPalette(palette);
    }
}

/*********************************************************/

/**
 * @brief MainWindow::find 查找
 * @param str
 * @param ignore
 * @param choose
 */
void MainWindow::find(QString str,bool ignore,bool choose){
    QString text = str;
    bool find_flag;
    if(!text.isEmpty()){
        if(choose){
            if(ignore){
                find_flag=ui->textEdit->find(text,QTextDocument::FindCaseSensitively);
            }else{
                find_flag=ui->textEdit->find(text);
            }
        }else{
            if(ignore){
                find_flag=ui->textEdit->find(text,QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
            }else{
                find_flag=ui->textEdit->find(text,QTextDocument::FindBackward);
            }
        }

        if(!find_flag){
            QMessageBox::information(this,tr("结果"),tr("没有找到查找内容"),QMessageBox::Yes);
        }
    }
}


void MainWindow::showFindDialog(){
    find_dialog->show();
}

/**
 * @brief MainWindow::findReplace
 * @param find_str
 * @param flag
 */
void MainWindow::findReplace(QString find_str, bool flag){
    bool find_flag;
    if(flag){
        find_flag = ui->textEdit->find(find_str,QTextDocument::FindCaseSensitively);
    }else{
        find_flag = ui->textEdit->find(find_str);
    }
    if(!find_flag){
        QMessageBox::information(this,tr("结果"),tr("没有找到查找内容"),QMessageBox::Yes);
    }else{
        findFlag = true;
    }
}

/**
 * @brief MainWindow::replaceCur
 * @param find_str
 * @param replace_str
 */
void MainWindow::replaceCur(QString find_str, QString replace_str,bool flag){
    if(findFlag){
        QTextCursor text_cursor = ui->textEdit->textCursor();
        text_cursor.insertText(replace_str);
        findFlag = false;
    }else{
        bool find_flag;
        if(flag){
            find_flag = ui->textEdit->find(find_str,QTextDocument::FindCaseSensitively);
        }else{
            find_flag = ui->textEdit->find(find_str);
        }
        if(!find_flag){
            findFlag = false;
            QMessageBox::information(this,tr("结果"),tr("没有内容，不能替换")+find_str,QMessageBox::Yes);
        }else{
            findFlag = true;
        }
    }

}

/**
 * @brief MainWindow::replaceAll
 * @param find_str
 * @param replace_str
 * @param flag
 */
void MainWindow::replaceAll(QString find_str, QString replace_str,bool flag){
    if(!flag){
        bool find_flag = ui->textEdit->find(find_str);
        if(find_flag){
            QTextCursor text_cursor = ui->textEdit->textCursor();
            text_cursor.insertText(replace_str);
            while(ui->textEdit->find(find_str)){
                text_cursor = ui->textEdit->textCursor();
                text_cursor.insertText(replace_str);
            }
            while(ui->textEdit->find(find_str,QTextDocument::FindBackward)){
                text_cursor=ui->textEdit->textCursor();
                text_cursor.insertText(replace_str);
            }
        }else{
            QMessageBox::information(this,tr("结果"),tr("没有内容,不能替换 ")+find_str,QMessageBox::Yes);
        }
    }else{
        bool find_flag = ui->textEdit->find(find_str,QTextDocument::FindCaseSensitively);
        if(find_flag){
            QTextCursor text_cursor = ui->textEdit->textCursor();
            text_cursor.insertText(replace_str);
            while(ui->textEdit->find(find_str,QTextDocument::FindCaseSensitively)){
                text_cursor = ui->textEdit->textCursor();
                text_cursor.insertText(replace_str);
            }
            while(ui->textEdit->find(find_str,QTextDocument::FindBackward|QTextDocument::FindCaseSensitively)){
                text_cursor=ui->textEdit->textCursor();
                text_cursor.insertText(replace_str);
            }
        }else{
            QMessageBox::information(this,tr("结果"),tr("没有内容,不能替换 ")+find_str,QMessageBox::Yes);
        }
    }
}

void MainWindow::showReplaceDialog(){
    replace_dialog->show();
}

/********************************************************/

/**
 * @brief MainWindow::showTextInfo 显示鼠标坐标
 */
void MainWindow::showTextInfo(){
    QTextCursor textCursor = ui->textEdit->textCursor();//获得鼠标光标
    int lineNum = textCursor.blockNumber();//行数
    int colNum = textCursor.columnNumber();//列数
    textInfo->setText(tr("Li:%1,Col:%2").arg(lineNum+1).arg(colNum));//显示在Label上，注意：行数是从0开始的
}

