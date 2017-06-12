#include "DialogGood.h"
#include "ui_DialogGood.h"

DialogGood::DialogGood(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGood)
{
    ui->setupUi(this);
}

DialogGood::~DialogGood()
{
    delete ui;
}

void DialogGood::addCategory(int rcd, QString name)
{
    category_rcd.push_back(rcd);
    ui->comboBoxCategory->addItem(name);
}

void DialogGood::on_pushButtonOK_clicked()
{
    rcd             = ui->lineEditRcd->text();
    category        = category_rcd[ui->comboBoxCategory->currentIndex()];
    name            = ui->lineEditName->text();
    price           = ui->doubleSpinBoxPrice->value();
    count           = ui->doubleSpinBoxCount->value();

    state = 1;

    reject();
}

void DialogGood::on_pushButtonCancel_clicked()
{
    state = 0;

    reject();
}

int DialogGood::getCount() const
{
    return count;
}

void DialogGood::setCount(int value)
{
    ui->doubleSpinBoxCount->setValue(value);
}

double DialogGood::getPrice() const
{
    return price;
}

void DialogGood::setPrice(const double &value)
{
    ui->doubleSpinBoxPrice->setValue(value);
}

QString DialogGood::getName() const
{
    return name;
}

void DialogGood::setName(const QString &value)
{
    ui->lineEditName->setText(value);
}

int DialogGood::getCategory() const
{
    return category;
}

void DialogGood::setCategory(int value)
{
    ui->comboBoxCategory->setCurrentIndex(category_rcd.indexOf(value));
}

QString DialogGood::getRcd() const
{
    return rcd;
}

void DialogGood::setRcd(const QString &value)
{
    ui->lineEditRcd->setText(value);
}

void DialogGood::on_doubleSpinBoxPrice_valueChanged(double arg1)
{
    ui->doubleSpinBoxPricePDV->setValue(arg1 + arg1 * 0.2);
}

int DialogGood::getState() const
{
    return state;
}
