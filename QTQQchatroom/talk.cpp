/**
 *  talk class flie                                                           *
 *                                                                            *
 *  This file is part of our work.                                            *
 *                                                                            *
 *  This program is tu make the talk gui                                      *
 *                                                                            *
 *  @file     talk.cpp                                                        *
 *  @brief    talk gui                                                        *
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

#include "talk.h"
#include "ui_talk.h"
#include "winsocket.h"
#include "two.h"
#include "ui_two.h"
#include"mylistwidget.h"
#include "QListWidgetItem"
#include<QListWidget>
#include<QVBoxLayout>
#include<mychatwidget.h>
#include "form.h"
#include"global.h"

/**
 * @brief   Talk::Talk
 * @param   QWidget
 * @return  None
 * @note    ????????????
 */
Talk::Talk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
    i=0;
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    setAutoFillBackground(true);
    ui->label->installEventFilter(this);
    ui->label_2->installEventFilter(this);
    ui->label_9->installEventFilter(this);
    ui->label_10->installEventFilter(this);
    ui->label_8->installEventFilter(this);

    QImage image;
    image.load(":/myicon_3.png");
    QImage result = image.scaled(40, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_4->setPixmap(QPixmap::fromImage(result));

    QImage image2;
    image2.load(":/sun.jpg");
    QImage result2 = image2.scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_3->setPixmap(QPixmap::fromImage(result2));
    ui->label_3->setStyleSheet("QLabel{padding:2px;border-radius:4px;}QLabel:hover{border:2px solid rgb(12, 255, 235);}");

    QImage image3;
    image3.load(":/myicon_1.png");

    QImage image4;
    image2.load(":/sun2.png");
    QImage result3 = image2.scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_11->setPixmap(QPixmap::fromImage(result3));
    ui->label_11->setStyleSheet("QLabel{padding:2px;border-radius:4px;}QLabel:hover{border:2px solid rgb(12, 255, 235);}");

    /* ?????????????????? */
    char user[12]="\0";
    qstacklayout=new QStackedLayout;
    ui->widget_2->setLayout(qstacklayout);
    drwer=new Drawer(this);

    /* ?????????friendlist */
    qDebug()<<namelength;
    qDebug()<<namelist;
    int userlen=0;
    for(int i=0;i<namelength;i++)
    {
        if(namelist[i]!='|')
        {
            user[userlen]=namelist[i];
            userlen++;
        }
        if(namelist[i+1] == '|' || i == namelength-1)
        {
            i++;
            QString temp(user);
            qDebug()<<"talk"<<user<<temp;
            drwer->addfriend(temp,"welcome back!");
            qstacklayout->addWidget(drwer);
            for(int j=0;j<12;j++)
            {
                user[j]='\0';
            }
            userlen=0;
        }
    }

//    QVBoxLayout* layout=new QVBoxLayout(ui->listWidget);
//    QListWidgetItem* item1=new QListWidgetItem();
//    item1->setSizeHint(QSize(0,35));
//    Mychatwidget* m1=new Mychatwidget();
//    ui->listWidget->addItem(item1);
//    ui->listWidget->setItemWidget(item1,m1);
//    ui->listWidget->setStyleSheet("QListWidget::item:hover {background-color:#FCF0C1;}");
//    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),m1,SLOT(changehight()));
//    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(changeheight(QListWidgetItem*)));

    QIcon icon = QIcon(":/kobe_1.jpg"); //png,ico,svg,bmp???????????????????????????????????????60x60?????????????????????????????????????????????????????????
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    m_pShowTimer = new QTimer(this);
    m_pShowTimer2 = new QTimer(this);
    connect(m_pShowTimer, SIGNAL(timeout()), this, SLOT(onMove()));
    connect(m_pShowTimer2, SIGNAL(timeout()), this, SLOT(onMove2()));
    //setMouseTracking(true);
    mouse_press=false;
    ui->label_8->installEventFilter(this);
    ui->label_9->installEventFilter(this);
    ui->label_10->installEventFilter(this);

    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(showResearch()));

    /* ????????????????????? */
    connect(mysocket,SIGNAL(isDone(QString)),this,SLOT(updatalist(QString)));
}

/**
 * @brief   Talk::~Talk
 * @param   None
 * @return  None
 * @note    ????????????
 */
Talk::~Talk()
{
    delete ui;
}
void Talk::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event->screenPos();
    if(event->button() == Qt::LeftButton)
     {
        qDebug()<<"????????????";
         this->setMouseTracking(true);
          mouse_press = true;
          //?????????????????????????????????????????????event->globalPos() - this->pos()???
          move_point = event->pos();
          qDebug()<<event->source();
     }

}

/**
 * @brief   Talk::mouseReleaseEvent
 * @param   QMouseEvent
 * @return  None
 * @note    ??????????????????
 */
void Talk::mouseReleaseEvent(QMouseEvent *event)
{
    //???????????????????????????
        mouse_press = false;
    qDebug()<<"???????????????????????????";
    if(y()<=0)
    {
        qDebug()<<"y????????????0";
        m_nDesktopHeight=0;
        m_pShowTimer->start(0.5);
    }
}

/**
 * @brief   Talk::mouseMoveEvent
 * @param   QMouseEvent
 * @return  None
 * @note    ??????????????????
 */
void Talk::mouseMoveEvent(QMouseEvent *event)
{
    //????????????????????????
  //  qDebug()<<"mouse_press="<<event->globalPos();
      qDebug()<<"???????????????????????????";
        if(mouse_press)
        {
            i++;
            //??????????????????????????????
            QPoint move_pos = event->globalPos();
             qDebug()<<"??????"<<i;
           //?????????????????????
           this->move(move_pos - move_point);
        }
        qDebug()<<"???????????????";
}

/**
 * @brief   Talk::eventFilter
 * @param   QObject
 * @param   QEvent
 * @return  bool
 * @note    ????????????
 */
bool Talk::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label){
        hide();
        qDebug()<<"?????????";
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_2){
         close();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_9){
        qDebug()<<"??????????????????";
        qstacklayout->setCurrentIndex(0);
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_10){
        qDebug()<<"??????????????????";
        qstacklayout->setCurrentIndex(1);
    }
    else if(object==ui->label_8&&e->type()==QEvent::Enter&&y()<0){
        qDebug()<<"????????????????????????y??????0";
         qDebug()<<e->type();
        //move(x(),5);
         m_nDesktopHeight2=y();
         m_pShowTimer2->start(0.5);

    }
    else if(object==ui->label_9&&e->type()==QEvent::Enter&&y()<0){
        qDebug()<<"????????????????????????y??????0";
        qDebug()<<e->type();
       //move(x(),5);
        m_nDesktopHeight2=y();
        m_pShowTimer2->start(0.5);
    }
    else if(object==ui->label_10&&e->type()==QEvent::Enter&&y()<0){
        qDebug()<<"????????????????????????y??????0";
         qDebug()<<e->type();
        //move(x(),5);
         m_nDesktopHeight2=y();
         m_pShowTimer2->start(0.5);
    }
    return false;
}

/**
 * @brief   Talk::updatalist
 * @param   QPaintEvent
 * @return  None
 * @note    ??????????????????
 */
void Talk::updatalist(QString data)
{
    /* ???????????????????????? */
    char user[12]="\0";
    int userlen=0;
    int len=0;
    char namelistnew[512]="\0";
    QString stringname(namelist);
    qDebug()<<"[Talk.cpp line275 get talklist]:"<<data;
    /* ???????????????????????? */
    if(data[0] == "t")
    {
        for(int i = 16;i<data.length();i++)
        {
            namelistnew[len] = data[i].toLatin1();
            len++;
            if(data[i+1]=="$")break;
        }
        qDebug()<<"[Talk.cpp line288 get namelistnew]:"<<namelistnew;
        for(int i=0;i<len;i++)
        {
            if(namelistnew[i]!='|')
            {
                user[userlen]=namelistnew[i];
                userlen++;
            }
            if(namelistnew[i+1] == '|' || i == len-1)
            {
                i++;
                QString temp(user);
                qDebug()<<"[Talk.cpp line290 campale talklist]:"<<temp<<stringname<<stringname.contains(temp,Qt::CaseSensitive);
                if(!stringname.contains(temp,Qt::CaseSensitive))
                {
//                    namelist[namelength++]='|';
//                    for(int i = 0;i<userlen;i++)
//                    {
//                        namelist[namelength++] = user[i];
//                    }
                    drwer->addfriend(temp,"welcome back!");
                    qstacklayout->addWidget(drwer);
                }
                for(int j=0;j<12;j++)
                {
                    user[j]='\0';
                }
                userlen=0;
            }
        }
        qDebug()<<"[Talk.cpp line310 updata talklist]:"<<namelist;
    }
}

/**
 * @brief   Talk::paintEvent
 * @param   QPaintEvent
 * @return  None
 * @note    ????????????
 */
void Talk::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(3, 3, this->width()-6, this->height()-6);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(path, QBrush(Qt::white));

        QColor color(0, 0, 0, 75);
        for(int i=0; i<3; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRect(3-i, 3-i, this->width()-(3-i)*2, this->height()-(3-i)*2);
            color.setAlpha(50 - qSqrt(i)*25);
            painter.setPen(color);
            painter.drawPath(path);
        }
}

/**
 * @brief   Talk::iconActivated
 * @param   QSystemTrayIcon
 * @return  None
 * @note    ????????????
 */
void Talk::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {

    case QSystemTrayIcon::DoubleClick:


        break;
    case QSystemTrayIcon::Trigger:

        //????????????????????????
        mouse_press=false;
        showNormal();
        raise();
        activateWindow();
        move(x(),5);
        break;
    case QSystemTrayIcon::MiddleClick:

        break;
    default:
        break;
    }
}

/**
 * @brief   Talk::hideEvent
 * @param   QHideEvent
 * @return  None
 * @note    ????????????
 */
void Talk::hideEvent(QHideEvent *event)
{
    qDebug()<<"123";
    if(trayIcon->isVisible())
    {
        hide(); //????????????
        event->ignore(); //????????????
    }
}

/**
 * @brief   Talk::onMove
 * @param   None
 * @return  None
 * @note    ????????????
 */
void Talk::onMove()
{
    qDebug()<<"?????????"<<m_nDesktopHeight;
    m_nDesktopHeight--;
    move(x(), m_nDesktopHeight);
    if (m_nDesktopHeight <= -610)
    {
        m_pShowTimer->stop();
        //hide();
    }
}

/**
 * @brief   Talk::onMove2
 * @param   None
 * @return  None
 * @note    ????????????
 */
void Talk::onMove2()
{
    qDebug()<<"?????????"<<m_nDesktopHeight2;
    m_nDesktopHeight2++;
    move(x(), m_nDesktopHeight2);
    if (y() >=5)
    {
        m_pShowTimer2->stop();
        //hide();
    }
}

/**
 * @brief   Talk::showResearch
 * @param   None
 * @return  None
 * @note    ????????????
 */
void Talk::showResearch()
{
    Search* s=new Search();
    s->show();
    s->doSearch(ui->lineEdit->text());
}

/**
 * @brief   Talk::resolveMessage
 * @param   None
 * @return  None
 * @note    ????????????
 */
void Talk::resolveMessage()
{
    QByteArray qba= socket->readAll(); //??????

        QString ss=QVariant(qba).toString();
        qDebug()<<ss;
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(ss.toUtf8(), &error);
        //QString ss=QVariant(qba).toString();
        //qDebug("%s", qba);
        //qDebug()<<ss;
        if (error.error == QJsonParseError::NoError) {
            if (jsonDocument.isObject()) {
                /*QVariantMap result = jsonDocument.toVariant().toMap();
                qDebug()<<result;
                qDebug()<<result["userinfo"].toJsonArray();
                foreach (QVariant plugin, result["userinfo"].toList()) {
                            qDebug()<<plugin.toString();
                   }*/
                QJsonObject obj=jsonDocument.object();
                //QJsonArray array=obj["userinfo"].toArray();
                QJsonObject obj1=obj["usersInfoVo"].toObject();
                qDebug()<<obj1["name"].toString();
                ui->label_6->setText(obj1["name"].toString());
                ui->label_7->setText(obj1["sign"].toString());
                QJsonArray friends=obj1["friends"].toArray();
                qDebug()<<friends;
                for(int j=0;j<friends.size();j++){
                    QJsonObject myfriend=friends[i].toObject();
                    qDebug()<<myfriend["name"];
                    QString name;
                    if(myfriend["onLine"]==false)
                        name=myfriend["name"].toString()+"[??????]";
                    else
                        name=myfriend["name"].toString()+"[??????]";
                    drwer->addfriend(name,myfriend["sign"].toString());
                }


            }
        } else {
           qDebug()<<"????????????";
        }
       // socket->write(qba);
}

/**
 * @brief   Talk:init
 * @param   None
 * @return  None
 * @note    ?????????
 */
void Talk::init(QByteArray byte_array)
{
    qDebug()<<byte_array.append('\n');
    qDebug()<<byte_array;
//    socket->write(byte_array);
}

/**
 * @brief   Talk::solvedisconnect
 * @param   None
 * @return  None
 * @note    ??????debug
 */
void Talk::solvedisconnect()
{
    qDebug()<<"socket????????????";
}

