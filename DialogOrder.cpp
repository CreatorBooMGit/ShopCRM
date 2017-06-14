#include "DialogOrder.h"
#include "ui_DialogOrder.h"

#include <QInputDialog>
#include <QMenu>

DialogOrder::DialogOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOrder)
{
    ui->setupUi(this);
    setFixedSize(450, 500);
}

DialogOrder::~DialogOrder()
{
    delete ui;
}

void DialogOrder::on_pushButtonOK_clicked()
{
    rcd     = ui->lineEditRcd->text();
    date    = ui->dateEditOrder->date();
    time    = ui->timeEditOrder->time();
    client  = clients_rcd[ui->comboBoxClient->currentIndex()].id;
    manager = managers_rcd[ui->comboBoxManager->currentIndex()].id;
    sum     = ui->doubleSpinBoxSum->value();
    status  = status_rcd[ui->comboBoxStatus->currentIndex()].id;

    state = 1;

    reject();
}

void DialogOrder::on_pushButtonCancel_clicked()
{
    state = 0;

    reject();
}

int DialogOrder::getStatus() const
{
    return status;
}

void DialogOrder::addGoodOrder(int _id, QString _rcd, QString _name, double _price, double _count)
{
    ui->tableWidgetGoods->setRowCount(ui->tableWidgetGoods->rowCount() + 1);

    Good t_good;
    t_good.id       = _id;
    t_good.rcd      = _rcd;
    t_good.name     = _name;
    t_good.price    = _price;
    t_good.count    = _count;
    t_good.sum      = _price * _count;
    t_good.flag     = GoodAdded;
    goodsOrder.push_back(t_good);

    QTableWidgetItem *itemRcd       = new QTableWidgetItem();
    QTableWidgetItem *itemName      = new QTableWidgetItem();
    QTableWidgetItem *itemSum      = new QTableWidgetItem();
    QTableWidgetItem *itemSumPDV      = new QTableWidgetItem();
    QTableWidgetItem *itemCount      = new QTableWidgetItem();

    itemRcd->setText(_rcd);
    itemName->setText(_name);
    itemSum->setText(QString::number(t_good.sum));
    itemSumPDV->setText(QString::number(t_good.sum * 1.2));
    itemCount->setText(QString::number(_count));

    itemRcd->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemSum->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemSumPDV->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    itemCount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 0, itemRcd);
    ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 1, itemName);
    ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 2, itemSum);
    ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 3, itemSumPDV);
    ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 4, itemCount);
}

void DialogOrder::setStatus(int value)
{
    int t_itemIndex = 0;
    for(int i = 0; i < status_rcd.count(); i++)
    {
        if(status_rcd[i].id == value)
        {
            t_itemIndex = i;
            break;
        }
    }
    ui->comboBoxStatus->setCurrentIndex(t_itemIndex);
}

double DialogOrder::getSum() const
{
    return sum;
}

void DialogOrder::setSum(double value)
{
    ui->doubleSpinBoxSum->setValue(value);
}

int DialogOrder::getManager() const
{
    return manager;
}

void DialogOrder::setManager(int value)
{
    int t_itemIndex = 0;
    for(int i = 0; i < managers_rcd.count(); i++)
    {
        if(managers_rcd[i].id == value)
        {
            t_itemIndex = i;
            break;
        }
    }
    ui->comboBoxManager->setCurrentIndex(t_itemIndex);
}

int DialogOrder::getClient() const
{
    return client;
}

void DialogOrder::setClient(int value)
{
    int t_itemIndex = 0;
    for(int i = 0; i < clients_rcd.count(); i++)
    {
        if(clients_rcd[i].id == value)
        {
            t_itemIndex = i;
            break;
        }
    }
    ui->comboBoxClient->setCurrentIndex(t_itemIndex);
}

QTime DialogOrder::getTime() const
{
    return time;
}

void DialogOrder::setTime(const QTime &value)
{
    ui->timeEditOrder->setTime(value);
}

QDate DialogOrder::getDate() const
{
    return date;
}

void DialogOrder::setDate(const QDate &value)
{
    ui->dateEditOrder->setDate(value);
}

QString DialogOrder::getRcd() const
{
    return rcd;
}

void DialogOrder::setRcd(const QString &value)
{
    ui->lineEditRcd->setText(value);
}

void DialogOrder::on_doubleSpinBoxSum_valueChanged(double arg1)
{
    ui->doubleSpinBoxSumPDV->setValue(arg1 * 1.2);
}

QVector<Good> DialogOrder::getGoodsOrder() const
{
    return goodsOrder;
}

void DialogOrder::addClient(int _id, QString _FIO)
{
    Person t_client;

    t_client.id = _id;
    t_client.FIO = _FIO;

    clients_rcd.push_back(t_client);

    ui->comboBoxClient->addItem(_FIO);
}

void DialogOrder::addManager(int _id, QString _FIO)
{
    Person t_manager;

    t_manager.id = _id;
    t_manager.FIO = _FIO;

    managers_rcd.push_back(t_manager);

    ui->comboBoxManager->addItem(_FIO);
}

void DialogOrder::addStatus(int _id, QString _name)
{
    Status t_status;

    t_status.id = _id;
    t_status.name = _name;

    status_rcd.push_back(t_status);

    ui->comboBoxStatus->addItem(_name);
}

void DialogOrder::addGood(int _id, QString _rcd, QString _name, double _price, double _countHave)
{
    Good t_good;

    t_good.id = _id;
    t_good.rcd = _rcd;
    t_good.name = _name;
    t_good.price = _price;
    t_good.countHave = _countHave;

    goods_rcd.push_back(t_good);
}

void DialogOrder::on_tableWidgetGoods_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddGood);
    contextTableMenu.addAction(ui->actionEditGoodCount);
    contextTableMenu.addAction(ui->actionRemoveGood);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void DialogOrder::on_actionAddGood_triggered()
{
    DialogAddGoodInOrder *dialogAddGood = new DialogAddGoodInOrder();
    dialogAddGood->setGoods(goods_rcd);

    dialogAddGood->exec();

    if(dialogAddGood->getState())
    {
        ui->tableWidgetGoods->setRowCount(ui->tableWidgetGoods->rowCount() + 1);

        Good t_good = dialogAddGood->getGood();
        t_good.flag = GoodAdded;
        goodsOrder.push_back(t_good);

        QTableWidgetItem *itemRcd       = new QTableWidgetItem();
        QTableWidgetItem *itemName      = new QTableWidgetItem();
        QTableWidgetItem *itemSum      = new QTableWidgetItem();
        QTableWidgetItem *itemSumPDV      = new QTableWidgetItem();
        QTableWidgetItem *itemCount      = new QTableWidgetItem();

        itemRcd->setText(dialogAddGood->getGood().rcd);
        itemName->setText(dialogAddGood->getGood().name);
        itemSum->setText(QString::number(dialogAddGood->getGood().sum));
        itemSumPDV->setText(QString::number(dialogAddGood->getGood().sum * 1.2));
        itemCount->setText(QString::number(dialogAddGood->getGood().count));

        itemRcd->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemSum->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemSumPDV->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemCount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 0, itemRcd);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 1, itemName);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 2, itemSum);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 3, itemSumPDV);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 4, itemCount);

        ui->doubleSpinBoxSum->setValue(ui->doubleSpinBoxSum->value() + dialogAddGood->getGood().sum);

        for(int i = 0; i < goods_rcd.count(); i++)
        {
            if(goods_rcd[i].id == dialogAddGood->getGood().id)
            {
                goods_rcd[i].countHave = dialogAddGood->getGood().count;
                break;
            }
        }
    }

    delete dialogAddGood;
}

void DialogOrder::on_actionEditGoodCount_triggered()
{
    DialogAddGoodInOrder *dialogGood = new DialogAddGoodInOrder();

    dialogGood->setGoods(goods_rcd);

    dialogGood->setId(goodsOrder[ui->tableWidgetGoods->currentRow()].id);
    dialogGood->setCount(goodsOrder[ui->tableWidgetGoods->currentRow()].count);

    dialogGood->exec();

    if(dialogGood->getState())
    {
        ui->tableWidgetGoods->setRowCount(ui->tableWidgetGoods->rowCount() + 1);

        Good t_good = dialogGood->getGood();
        if(goodsOrder[ui->tableWidgetGoods->currentRow()].flag == GoodNothing)
            t_good.flag = GoodModify;
        else
            t_good.flag = GoodAdded;

        for(int i = 0; i < goods_rcd.count(); i++)
        {
            if(goods_rcd[i].id == dialogGood->getGood().id)
            {
                goods_rcd[i].countHave += goodsOrder[ui->tableWidgetGoods->currentRow()].count;
                goods_rcd[i].countHave -= dialogGood->getGood().count;
                break;
            }
        }

        ui->doubleSpinBoxSum->setValue(ui->doubleSpinBoxSum->value() - goodsOrder[ui->tableWidgetGoods->currentRow()].sum);
        goodsOrder[ui->tableWidgetGoods->currentRow()] = dialogGood->getGood();
        ui->doubleSpinBoxSum->setValue(ui->doubleSpinBoxSum->value() + goodsOrder[ui->tableWidgetGoods->currentRow()].sum);

        ui->tableWidgetGoods->item(ui->tableWidgetGoods->currentRow(), 0)->setText(dialogGood->getGood().rcd);
        ui->tableWidgetGoods->item(ui->tableWidgetGoods->currentRow(), 1)->setText(dialogGood->getGood().name);
        ui->tableWidgetGoods->item(ui->tableWidgetGoods->currentRow(), 2)->setText(QString::number(dialogGood->getGood().sum));
        ui->tableWidgetGoods->item(ui->tableWidgetGoods->currentRow(), 3)->setText(QString::number(dialogGood->getGood().sum * 1.2));
        ui->tableWidgetGoods->item(ui->tableWidgetGoods->currentRow(), 4)->setText(QString::number(dialogGood->getGood().count));
    }

    delete dialogGood;
}

void DialogOrder::on_actionRemoveGood_triggered()
{
    if(goodsOrder[ui->tableWidgetGoods->currentRow()].flag == GoodNothing || goodsOrder[ui->tableWidgetGoods->currentRow()].flag == GoodModify)
        goodsOrder[ui->tableWidgetGoods->currentRow()].flag = GoodRemove;
    else
    {
        ui->tableWidgetGoods->removeRow(ui->tableWidgetGoods->currentRow());
        goodsOrder.remove(ui->tableWidgetGoods->currentRow(), 1);
    }
}

int DialogOrder::getState() const
{
    return state;
}
