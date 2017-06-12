#ifndef DIALOGCLIENT_H
#define DIALOGCLIENT_H

#include <QDialog>

namespace Ui {
    class DialogClient;
}

class DialogClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClient(QWidget *parent = 0);
    ~DialogClient();

    int getState() const;

    void addManager(int rcd, QString surname, QString name, QString patronymic);
    void setSurname(const QString &value);
    void setName(const QString &value);
    void setPatronymic(const QString &value);
    void setPhone(const QString &value);
    void setEmail(const QString &value);
    void setManagerId(int value);

    QString getSurname() const;
    QString getName() const;
    QString getPatronymic() const;
    QString getPhone() const;
    QString getEmail() const;
    int getManagerId() const;

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::DialogClient *ui;

    int state = 0;

    QString surname;
    QString name;
    QString patronymic;
    QString phone;
    QString email;
    int     managerId;

    QVector<int> manager_rcd;
};

#endif // DIALOGCLIENT_H
