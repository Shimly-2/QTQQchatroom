/**
 *  form class flie                                                           *
 *                                                                            *
 *  This file is part of our work.                                            *
 *                                                                            *
 *  This program is tu make the loginin gui                                   *
 *                                                                            *
 *  @file     form.cpp                                                        *
 *  @brief    loginin                                                         *
 *  Details.                                                                  *
 *                                                                            *
 *  @author   JYY 1950049                                                     *
 *  @email    1950049@tongji.edu.cn                                           *
 *  @version  1.0.0.1                                                         *
 *  @date     2022/5/29                                                       *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *  Remark         : Description                                              *
 *----------------------------------------------------------------------------*
 *  Change History :                                                          *
 *  <Date>     | <Version> | <Author>       | <Description>                   *
 *----------------------------------------------------------------------------*
 *  2022/05/29 | 1.0.0.1   | jyy            | Create file                     *
 *----------------------------------------------------------------------------*
 *                                                                            *
 */

#include "form.h"
#include "ui_form.h"
#include<QNetworkRequest>
#include<QByteArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QNetworkReply>
#include<QJsonArray>
#include"chat.h"
#include"global.h"
#include"register.h"
#include<QMessageBox>

/**
 * @brief   Form::Form
 * @param   QWidget
 * @return  None
 * @note    ????????????
 */
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    i=0;
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
//    QMovie *movie = new QMovie(":/red69-2.gif");
    QMovie *movie = new QMovie(":/blue70-2.gif");
     ui->label->setMovie(movie);
     ui->label_2->installEventFilter(this);
     ui->label_3->installEventFilter(this);
     movie->start();

     ui->label_4->setStyleSheet("border-radius:15px;background-color:green;border-image:url(':/10.JPG')");
     ui->label_5->setOpenExternalLinks(true);

     ui->label_5->installEventFilter(this);
     animation=new QPropertyAnimation(ui->label_4, "geometry");
     animation->setDuration(400);
     animation->setStartValue(QRect(30, 170, 85, 85));
     animation->setEndValue(QRect(150, 170, 85, 85));

     QString qss;
       QFile qssFile(":/myqss/login.qss");
       qssFile.open(QFile::ReadOnly);
       qDebug()<<"sdfasd";
       if(qssFile.isOpen())
       {

         qss = QLatin1String(qssFile.readAll());
         this->setStyleSheet(qss);
         qssFile.close();
         qDebug()<<"????????????qss";
       }

       /* ?????????socket */
       mysocket->init(ip,port);
       connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton2_clicked()));
       connect(mysocket,SIGNAL(isDone(QString)),this,SLOT(logincheck(QString)));


       //debug
//       QString content2="user1|user2|user3$";
//       char* ptr;
//       QByteArray ba;
//       ba = content2.toLatin1();
//       ptr = ba.data();
//       strcpy_s(namelist, ptr);//strcpy_s(a,b)???b?????????a?????????????????????
//       namelength = content2.length()-1;

}

/**
 * @brief   Form::~Form
 * @param   None
 * @return  None
 * @note    ????????????
 */
Form::~Form()
{
    delete ui;
}
void Form::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
         this->setMouseTracking(true);
          mouse_press = true;
          //?????????????????????????????????????????????event->globalPos() - this->pos()???
          move_point = event->pos();
          qDebug()<<"??????1";
          qDebug()<<event->source();
     }

}

/**
 * @brief   Form::mouseReleaseEvent
 * @param   QMouseEvent
 * @return  None
 * @note    ??????????????????
 */
void Form::mouseReleaseEvent(QMouseEvent *event)
{
    //???????????????????????????
        mouse_press = false;

}

/**
 * @brief   Form::mouseMoveEvent
 * @param   QMouseEvent
 * @return  None
 * @note    ??????????????????
 */
void Form::mouseMoveEvent(QMouseEvent *event)
{
    //????????????????????????
  //  qDebug()<<"mouse_press="<<event->globalPos();
        if(mouse_press)
        {
            i++;
            //??????????????????????????????
            QPoint move_pos = event->globalPos();
             qDebug()<<"??????"<<i;
           //?????????????????????
           this->move(move_pos - move_point);
        }
}

/**
 * @brief   Form::eventFilter
 * @param   QObject
 * @param   QEvent
 * @return  bool
 * @note    ????????????
 */
bool Form::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label_3)
    {
         close();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_2){
        showMinimized();
        qDebug()<<"?????????";
     
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_5){
//        QDesktopServices::openUrl(QUrl("http://www.baidu.com"));
        mouse_press=false;

    }
    return false;
}

/**
 * @brief   Form::on_pushButton_clicked
 * @param   None
 * @return  None
 * @note    ????????????
 */
void Form::on_pushButton_clicked()
{
    qDebug()<<ui->lineEdit->text();
    if(ui->lineEdit->text()==""||ui->lineEdit_2->text()==""){
        qDebug()<<"????????????";
        ui->label_6->setText("????????????????????????");
    }
    else{
        ui->label_6->hide();
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->label_5->hide();
        animation->start();

        qDebug()<<(ui->lineEdit->text()=="123");
        qDebug()<<(ui->lineEdit_2->text()=="123");

        QString username = ui->lineEdit->text();
        QString userpass = ui->lineEdit_2->text();
        username = username + "$|$" + userpass;
        QString send = "\0";
        QString len=QString::number(username.length(), 10);
        qDebug()<<len;
        send = "lserver!!!!!!";
        if(username.length()<10) send = send + "00" + len;
        else send = send + "0" + len;
        send = send + username;
        char buff[512]="\0";
        char* ptr;
        QByteArray ba;
        ba = send.toLatin1();
        ptr = ba.data();
        strcpy_s(buff, ptr);//strcpy_s(a,b)???b?????????a?????????????????????
        qDebug()<<buff;
        mysocket->sendData(buff);

        if(ui->lineEdit->text()=="123" && ui->lineEdit_2->text()=="123"){
            QString temp(ui->lineEdit->text());
            name=temp;
            Talk* t=new Talk();
            setVisible(false);
            t->init(byte_array);
            t->show();
            close();
        }

        if(ui->lineEdit->text()=="111" && ui->lineEdit_2->text()=="111"){
            Chat *c2 = new Chat();
            c2->show();
            close();
        }
    }
}

/**
 * @brief   Form::on_lineEdit_2_returnPressed
 * @param   None
 * @return  None
 * @note    ??????
 */
void Form::on_lineEdit_2_returnPressed()
{
    qDebug()<<"????????????";
}

/**
 * @brief   Form::showEvent
 * @param   QShowEvent
 * @return  None
 * @note    ????????????
 */
void Form::showEvent(QShowEvent *event)
{
    mouse_press=false;
}

/**
 * @brief   Form::on_lineEdit_textEdited
 * @param   QString
 * @return  None
 * @note    editline
 */
void Form::on_lineEdit_textEdited(const QString &arg1)
{
    qDebug()<<"????????????"+arg1;
    if(ui->label_6->text()!=""){
        qDebug()<<"?????????????????????";
        ui->label_6->setText("");
    }

}

/**
 * @brief   Form::logincheck
 * @param   QNetworkReply
 * @return  None
 * @note    ???????????????????????????
 */
void Form::logincheck(QString data)
{
    /* ?????????????????? */
    if(data[0] == "l")
    {
        if(data[16] == "s")
        {
            ui->label_6->hide();
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            ui->lineEdit->hide();
            ui->lineEdit_2->hide();
            ui->label_5->hide();
            animation->start();

            QString temp(ui->lineEdit->text());
            name=temp;
            Talk* t=new Talk();
            setVisible(false);
            t->init(byte_array);
            t->show();
            close();
        }
        else {
            QMessageBox::critical(this, tr("????????????"),  tr("??????????????????????????????"),
                                               QMessageBox::Save | QMessageBox::Discard,  QMessageBox::Discard);
        }
    }

    /* ?????????????????? */
    int len = 0;
    if(data[0]=="t")
    {
        for(int i = 16;i<512;i++)
        {
            namelist[len] = data[i].toLatin1();
            len++;
            if(data[i+1]=="$")break;
        }
    }
    qDebug()<<len;
    namelength=len;
    qDebug()<<namelist;

}

void Form::on_pushButton2_clicked()
{
    qDebug()<<"in register";
    Register *R = new Register;
    R->show();
}
