#include "register.h"
#include "ui_register.h"
#include<QMessageBox>
#include<QPushButton>
#include<QDebug>
#include"global.h"
#include<QString>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    QPushButton *registerok=ui->registerok;
    connect(ui->registerok,SIGNAL(clicked()),this,SLOT(registok_clicked()));
    connect(ui->registerno,SIGNAL(clicked()),this,SLOT(registno_clicked()));
    connect(mysocket,SIGNAL(isDone(QString)),this,SLOT(getconform(QString)));
}

Register::~Register()
{
delete ui;
}


void Register::registok_clicked()
{
    if(ui->rename->text()==""||ui->repass->text()==""||ui->repassag->text()==""){
        qDebug()<<"内容为空";
        QMessageBox::critical(this, tr("提示信息"),  tr("账号密码不能为空"),
                                           QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);
//        ui->label_6->setText("账号密码不能为空");
    }
    else if(ui->repass->text() != ui->repassag->text())
    {
        qDebug()<<"密码不一致";
        QMessageBox::critical(this, tr("提示信息"),  tr("两次输入密码不一致"),
                                           QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);
    }
    else {
        QString username = ui->rename->text();
        QString userpass = ui->repass->text();
        if(username.length()>12)
            QMessageBox::critical(this, tr("提示信息"),  tr("用户名过长"),
                                               QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);
        username = username + "$|$" + userpass;
        QString send = "\0";
        QString len=QString::number(username.length(), 10);
        qDebug()<<len;
        send = "rserver!!!!!!";
        if(username.length()<10) send = send + "00" + len;
        else send = send + "0" + len;
        send = send + username;
        char buff[512]="\0";
        char* ptr;
        QByteArray ba;
        ba = send.toLatin1();
        ptr = ba.data();
        strcpy_s(buff, ptr);//strcpy_s(a,b)将b拷贝给a保存到文件夹中
        qDebug()<<buff;
        mysocket->sendData(buff);

    }
}

void Register::getconform(QString data)
{
    if(data[16] == "s")
        {
        QMessageBox::critical(this, tr("提示信息"),  tr("注册成功"),
                                           QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);
        this->hide();
    }
    else
        QMessageBox::critical(this, tr("提示信息"),  tr("注册失败，改用户名已被注册"),
                                           QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);

}

void Register::registno_clicked()
{
   this->hide();
}
