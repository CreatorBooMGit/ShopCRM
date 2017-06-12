#include "DialogEmployee.h"
#include "ui_DialogEmployee.h"

#include <QCryptographicHash>

DialogEmployee::DialogEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEmployee)
{
    ui->setupUi(this);
    setFixedSize(350, 250);
}

DialogEmployee::~DialogEmployee()
{
    delete ui;
}

void DialogEmployee::on_pushButtonOK_clicked()
{
    surname     = ui->lineEditSurname->text();
    name        = ui->lineEditName->text();
    patronymic  = ui->lineEditPatronymic->text();
    birthday    = ui->dateEditBirthday->date();
    login       = ui->lineEditLogin->text();
    password    = QCryptographicHash::hash(ui->lineEditPassword->text().append("x7sgas352da").toLatin1(), QCryptographicHash::Sha3_512).toHex();

    state = 1;

    reject();
}

void DialogEmployee::on_pushButtonCancel_clicked()
{
    state = 0;

    reject();
}

QString DialogEmployee::getPassword() const
{
    return password;
}

QString DialogEmployee::getLogin() const
{
    return login;
}

void DialogEmployee::setLogin(const QString &value)
{
    ui->lineEditLogin->setText(value);
}

QDate DialogEmployee::getBirthday() const
{
    return birthday;
}

void DialogEmployee::setBirthday(const QDate &value)
{
    ui->dateEditBirthday->setDate(value);
}

QString DialogEmployee::getPatronymic() const
{
    return patronymic;
}

void DialogEmployee::setPatronymic(const QString &value)
{
    ui->lineEditPatronymic->setText(value);
}

QString DialogEmployee::getName() const
{
    return name;
}

void DialogEmployee::setName(const QString &value)
{
    ui->lineEditName->setText(value);
}

QString DialogEmployee::getSurname() const
{
    return surname;
}

void DialogEmployee::setSurname(const QString &value)
{
    ui->lineEditSurname->setText(value);
}

int DialogEmployee::getState() const
{
    return state;
}
