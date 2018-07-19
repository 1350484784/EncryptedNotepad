#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    this->setMaximumSize(500,300); //固定大小

    ui->find_next_button->setEnabled(false);
    ui->replace_one_button->setEnabled(false);
    ui->replace_all_button->setEnabled(false);

    connect(ui->find_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(chooseStrSlot()));
    connect(ui->cancel_button,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->find_next_button,SIGNAL(clicked()),this,SLOT(findButtonStateSlot()));
    connect(ui->replace_one_button,SIGNAL(clicked()),this,SLOT(replaceOneButtonStateSlot()));
    connect(ui->replace_all_button,SIGNAL(clicked()),this,SLOT(replaceAllButtonStateSlot()));

}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

/**
 * @brief ReplaceDialog::chooseStrSlot
 */
void ReplaceDialog::chooseStrSlot(){
    if(!ui->find_lineEdit->text().isEmpty()){
        ui->find_next_button->setEnabled(true);
        ui->replace_one_button->setEnabled(true);
        ui->replace_all_button->setEnabled(true);
    }else{
        ui->find_next_button->setEnabled(false);
        ui->replace_one_button->setEnabled(false);
        ui->replace_all_button->setEnabled(false);
    }
}

/**
 * @brief ReplaceDialog::findButtonStateSlot
 */
void ReplaceDialog::findButtonStateSlot(){
    if(ui->ignore_flag->isChecked()){
        emit findReplaceStr(ui->find_lineEdit->text(),true);
    }else{
        emit findReplaceStr(ui->find_lineEdit->text(),false);
    }
}

/**
 * @brief ReplaceDialog::replaceOneButtonStateSlot
 */
void ReplaceDialog::replaceOneButtonStateSlot(){
    emit replaceCurOneStr(ui->find_lineEdit->text(),ui->replace_lineEdit->text(),true);
}

/**
 * @brief ReplaceDialog::replaceAllButtonStateSlot
 */
void ReplaceDialog::replaceAllButtonStateSlot(){
    if(ui->ignore_flag->isChecked()){
        emit replaceAllStr(ui->find_lineEdit->text() , ui->replace_lineEdit->text(),true);
    }else{
        emit replaceAllStr(ui->find_lineEdit->text() , ui->replace_lineEdit->text(),false);
    }
}

