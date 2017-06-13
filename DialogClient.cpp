#include "DialogClient.h"
#include "ui_DialogClient.h"

DialogClient::DialogClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClient)
{
    ui->setupUi(this);
    setFixedSize(350, 250);
}

DialogClient::~DialogClient()
{
    delete ui;
}

void DialogClient::on_pushButtonOk_clicked()
{
    surname     = ui->lineEditSurname->text();
    name        = ui->lineEditName->text();
    patronymic  = ui->lineEditPatronymic->text();
    phone       = ui->lineEditPhone->text();
    email       = ui->lineEditEmail->text();
    managerId   = manager_rcd[ui->comboBoxManager->currentIndex()];

    state = 1;

    reject();
}

void DialogClient::on_pushButtonCancel_clicked()
{
    state = 0;

    reject();
}

int DialogClient::getManagerId() const
{
    return managerId;
}

void DialogClient::setManagerId(int value)
{
    ui->comboBoxManager->setCurrentIndex(manager_rcd.indexOf(value));
}

QString DialogClient::getEmail() const
{
    return email;
}

void DialogClient::setEmail(const QString &value)
{
    ui->lineEditEmail->setText(value);
}

QString DialogClient::getPhone() const
{
    return phone;
}

void DialogClient::setPhone(const QString &value)
{
    ui->lineEditPhone->setText(value);
}

QString DialogClient::getPatronymic() const
{
    return patronymic;
}

void DialogClient::setPatronymic(const QString &value)
{
    ui->lineEditPatronymic->setText(value);
}

QString DialogClient::getName() const
{
    return name;
}

void DialogClient::setName(const QString &value)
{
    ui->lineEditName->setText(value);
}

QString DialogClient::getSurname() const
{
    return surname;
}

void DialogClient::setSurname(const QString &value)
{
    ui->lineEditSurname->setText(value);
}

int DialogClient::getState() const
{
    return state;
}

void DialogClient::addManager(int rcd, QString surname, QString name, QString patronymic)
{
    manager_rcd.push_back(rcd);
    ui->comboBoxManager->addItem(QString("%1 %2 %3").arg(surname).arg(name).arg(patronymic));
}
