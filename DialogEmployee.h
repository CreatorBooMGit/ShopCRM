#ifndef DIALOGEMPLOYEE_H
#define DIALOGEMPLOYEE_H

#include <QDate>
#include <QDialog>

namespace Ui {
    class DialogEmployee;
}

class DialogEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEmployee(QWidget *parent = 0);
    ~DialogEmployee();

    int getState() const;

    void setSurname(const QString &value);
    void setName(const QString &value);
    void setPatronymic(const QString &value);
    void setBirthday(const QDate &value);
    void setLogin(const QString &value);

    QString getSurname() const;
    QString getName() const;
    QString getPatronymic() const;
    QDate getBirthday() const;
    QString getLogin() const;
    QString getPassword() const;


private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::DialogEmployee *ui;

    int state = 0;

    QString surname;
    QString name;
    QString patronymic;
    QDate   birthday;
    QString login;
    QString password;
};

#endif // DIALOGEMPLOYEE_H
