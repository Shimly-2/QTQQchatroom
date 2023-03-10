/**
 *  drawer class flie                                                         *
 *                                                                            *
 *  This file is part of our work.                                            *
 *                                                                            *
 *  This program is tu make the friend list gui                               *
 *                                                                            *
 *  @file     drawer.cpp                                                      *
 *  @brief    friend list                                                     *
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

#include "drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include<QDebug>
#include<mylistwidget.h>
#include<QFile>
#include "two.h"
#include "chat.h"

/**
 * @brief   Drawer::Drawer
 * @param   QWidget
 * @return  None
 * @note    ????????????
 */
Drawer::Drawer(QWidget *parent,Qt::WindowFlags f)
    :QToolBox(parent,0)
{

   //******************????????????????????????*****************
    listWidget = new QListWidget;
    listWidget2 = new QListWidget;
    QVBoxLayout *layout4=new QVBoxLayout(listWidget);
    //?????????demo?????????itemclick??????.
    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showitemclick(QListWidgetItem*)));


   //***************??????????????????*****************
    this->addItem(listWidget,tr("????????????"));
    QVBoxLayout *layout1=new QVBoxLayout(listWidget2);
    this->addItem(listWidget2,tr("?????????"));
    this->setStyleSheet("QToolBox::tab {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);border-radius: 2px;color: black;}QToolBox::tab:selected { /* italicize selected tabs */font: italic;color: white; }");

}

/**
 * @brief   Drawer::showitemclick
 * @param   QListWidgetItem
 * @return  None
 * @note    ?????????????????????
 */
void Drawer::showitemclick(QListWidgetItem *item)
{
     qDebug()<<item->text();
     qDebug()<<listWidget->row(item);
     Chat *c = new Chat();
     c->show();
}

/**
 * @brief   Drawer::addfriend
 * @param   QString    name
 * @param   QString    sign
 * @return  None
 * @note    ??????????????????
 */
void Drawer::addfriend(QString name,QString sign)
{
    QListWidgetItem *listItem = new QListWidgetItem();
    listWidget->addItem(listItem);
    Mylistwidget* mlistwidget=new Mylistwidget();
    listItem->setSizeHint(QSize(0, 62));
    mlistwidget->init(name,sign);
    listWidget->setItemWidget(listItem, mlistwidget);
}
