#include "DialogAddGoodInOrder.h"
#include "ui_DialogAddGoodInOrder.h"

#include <QInputDialog>
#include <QDebug>

DialogAddGoodInOrder::DialogAddGoodInOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddGoodInOrder)
{
    ui->setupUi(this);
}

DialogAddGoodInOrder::~DialogAddGoodInOrder()
{
    delete ui;
}

void DialogAddGoodInOrder::on_pushButtonOk_clicked()
{
    good.rcd    = ui->lineEditRcd->text();
    good.name   = ui->lineEditName->text();
    good.price  = ui->doubleSpinBoxPrice->value();
    good.count  = ui->doubleSpinBoxCount->value();
    good.sum    = ui->doubleSpinBoxSum->value();

    state = 1;

    reject();
}

void DialogAddGoodInOrder::on_pushButtonCancel_clicked()
{
    state = 0;

    reject();
}

Good DialogAddGoodInOrder::getGood() const
{
    return good;
}

void DialogAddGoodInOrder::setGoods(const QVector<Good> &value)
{
    goods = value;
}

int DialogAddGoodInOrder::getState() const
{
    return state;
}

void DialogAddGoodInOrder::on_doubleSpinBoxCount_valueChanged(double arg1)
{
    if(arg1 > good.countHave)
        ui->doubleSpinBoxCount->setValue(good.countHave);
    ui->doubleSpinBoxSum->setValue(ui->doubleSpinBoxPrice->value() * arg1);
}

void DialogAddGoodInOrder::on_doubleSpinBoxSum_valueChanged(double arg1)
{
    ui->doubleSpinBoxSumPDV->setValue(arg1 * 1.2);
}

void DialogAddGoodInOrder::on_doubleSpinBoxPrice_valueChanged(double arg1)
{
    ui->doubleSpinBoxPricePDV->setValue(arg1 * 1.2);
    ui->doubleSpinBoxSumPDV->setValue(ui->doubleSpinBoxCount->value() * arg1);
}

void DialogAddGoodInOrder::on_toolButtonRcd_clicked()
{
    QStringList t_rcds;

    for(Good t_g : goods)
        t_rcds.push_back(t_g.rcd);

    qDebug() << t_rcds;
    QString t_rcd = QInputDialog::getItem(this, tr("Выбор товара"), tr("Выберите артикул товара"), t_rcds, current);

    current = t_rcds.indexOf(t_rcd);

    good = goods[current];

    ui->lineEditRcd->setText(good.rcd);
    ui->lineEditName->setText(good.name);
    ui->doubleSpinBoxPrice->setValue(good.price);
    ui->labelCountHave->setText(tr("В наличии: %1").arg(good.countHave));
}

void DialogAddGoodInOrder::on_toolButtonName_clicked()
{
    QStringList t_names;

    for(Good t_n : goods)
        t_names.push_back(t_n.rcd);

    QString t_name = QInputDialog::getItem(this, tr("Выбор товара"), tr("Выберите артикул товара"), t_names, current);

    current = t_names.indexOf(t_name);

    good = goods[current];

    ui->lineEditRcd->setText(good.rcd);
    ui->lineEditName->setText(good.name);
    ui->doubleSpinBoxPrice->setValue(good.price);
    ui->labelCountHave->setText(tr("В наличии: %1").arg(good.countHave));
}
