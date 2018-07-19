#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    this->setMaximumSize(434,270); //固定大小
    ui->next_radio->setChecked(true);
    ui->find_button->setEnabled(false);

    //setWindowFlags(Qt::WindowStaysOnTopHint);//置顶显示

    connect(ui->find_edit,SIGNAL(textChanged(QString)),this,SLOT(findButtonState()));
    connect(ui->find_button,SIGNAL(clicked(bool)),this,SLOT(findDataButtonClickedState()));
}

FindDialog::~FindDialog()
{
    delete ui;
}


void FindDialog::findButtonState(){
    if(ui->find_edit->text().isEmpty()){
        ui->find_button->setEnabled(false);
    }else{
        ui->find_button->setEnabled(true);
    }
}

void FindDialog::findDataButtonClickedState(){
    if(ui->find_edit->text().isEmpty()){
        return;
    }
    QString str = ui->find_edit->text();
    if(ui->back_radio->isChecked()){
        if(ui->ignore_flag->isChecked()){
            emit findTextDataButtonClickedSignal(str,true,true);
        }else{
            emit findTextDataButtonClickedSignal(str,false,true);
        }
    }
    if(ui->next_radio->isChecked()){
        if(ui->ignore_flag->isChecked()){
            emit findTextDataButtonClickedSignal(str,true,false);
        }else{
            emit findTextDataButtonClickedSignal(str,false,false);
        }
    }
}
