#include "emotion.h"
#include "ui_emotion.h"
#include<QLabel>
#include<QDebug>
Emotion::Emotion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Emotion)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint);
    ui->label->installEventFilter(this);
    ui->label_2->installEventFilter(this);
    ui->label_3->installEventFilter(this);
    //ui->label_15->installEventFilter(this);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    ui->label_6->installEventFilter(this);
    ui->label_7->installEventFilter(this);
    ui->label_8->installEventFilter(this);
    ui->label_9->installEventFilter(this);
    ui->label_10->installEventFilter(this);
    ui->label_11->installEventFilter(this);
    ui->label_12->installEventFilter(this);
    //ui->label_13->installEventFilter(this);
}

Emotion::~Emotion()
{
    delete ui;
}
bool Emotion::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label)
    {
        emit imgurl(QString(":/emotion/emo/em1.jpg"));
         hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_2){
     emit imgurl(QString(":/emotion/emo/em2.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_3){
     emit imgurl(QString(":/emotion/emo/em3.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_4){
     emit imgurl(QString(":/emotion/emo/em4.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_5){
      emit imgurl(QString(":/emotion/emo/em5.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_6){
     emit imgurl(QString(":/emotion/emo/em6.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_7){
     emit imgurl(QString(":/emotion/emo/em7.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_8){
     emit imgurl(QString(":/emotion/emo/em8.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_9){
     emit imgurl(QString(":/emotion/emo/em9.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_10){
     emit imgurl(QString(":/emotion/emo/em10.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_11){
     emit imgurl(QString(":/emotion/emo/em11.jpg"));
     hide();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_12){
     emit imgurl(QString(":/emotion/emo/em12.jpg"));
     hide();
    }
    return false;
}

void Emotion::on_pushButton_clicked()
{
    qDebug()<<"点击了";
}
