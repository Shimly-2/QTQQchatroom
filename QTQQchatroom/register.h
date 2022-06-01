#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
     void registok_clicked();
     void registno_clicked();
     void getconform(QString data);

//     void on_lineEdit_2_returnPressed();
//     void on_lineEdit_textEdited(const QString &arg1);
//     void parseresult(QNetworkReply* reply);


private:
    Ui::Register *ui;
};

#endif // REGISTER_H
