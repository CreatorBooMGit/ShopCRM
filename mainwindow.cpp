#include "DialogClient.h"
#include "DialogEmployee.h"
#include "DialogGood.h"
#include "DialogOrder.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QSqlError>
#include <QDate>
#include <QInputDialog>
#include <QSqlResult>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("31.131.23.152");
    database.setPort(3306);
    database.setDatabaseName("shopcrm");
    database.setUserName("shopcrm");
    database.setPassword("123456");
    database.open();

    query = new QSqlQuery(database);

    updateOrders();
    updateGoods();
    updateCategories();
    updateClients();
    updateEmployees();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox messageAbout;
    messageAbout.setIcon(QMessageBox::Information);
    messageAbout.setWindowTitle(tr("Информация про программу"));
    messageAbout.setText(tr("CRM система магазина\n"
                            "Дипломная работа\n"
                            "Студента группы 471\n"
                            "Додукало Д.О."));
    messageAbout.exec();
}

void MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl(QApplication::applicationDirPath() + "/help/index.htm"));
}


void MainWindow::on_actionAddOrder_triggered()
{
    DialogOrder *dialogOrder = new DialogOrder();

    query->prepare("SELECT clients_rcd, clients_surname, clients_name, clients_patronymic "
                   "FROM clients");
    query->exec();

    while(query->next())
        dialogOrder->addClient(query->value("clients_rcd").toInt(), QString("%1 %2 %3")
                               .arg(query->value("clients_surname").toString())
                               .arg(query->value("clients_name").toString())
                               .arg(query->value("clients_patronymic").toString()));

    query->prepare("SELECT employees_idemployee, employees_surname, employees_name, employees_patronymic "
                   "FROM employees");
    query->exec();

    while(query->next())
        dialogOrder->addManager(query->value("employees_idemployee").toInt(), QString("%1 %2 %3")
                                .arg(query->value("employees_surname").toString())
                                .arg(query->value("employees_name").toString())
                                .arg(query->value("employees_patronymic").toString()));

    query->prepare("SELECT status_id, status_name "
                   "FROM status");
    query->exec();

    while(query->next())
        dialogOrder->addStatus(query->value("status_id").toInt(), query->value("status_name").toString());

    query->prepare("SELECT goods_idgood, goods_rcd, goods_name, goods_price, goods_count "
                   "FROM goods ");
    query->exec();

    while(query->next())
        dialogOrder->addGood(query->value("goods_idgood").toInt(),
                             query->value("goods_rcd").toString(),
                             query->value("goods_name").toString(),
                             query->value("goods_price").toDouble(),
                             query->value("goods_count").toDouble());

    dialogOrder->exec();

    if(dialogOrder->getState() == 1)
    {
        query->prepare("INSERT INTO orders (orders_rcd, orders_date, orders_time, orders_client, orders_manager, orders_sum, orders_status ) "
                       "VALUES (:orders_rcd, :orders_date, :orders_time, :orders_client, :orders_manager, :orders_sum, :orders_status)");
        query->bindValue(":orders_rcd", dialogOrder->getRcd());
        query->bindValue(":orders_date", dialogOrder->getDate());
        query->bindValue(":orders_time", dialogOrder->getTime());
        query->bindValue(":orders_client", dialogOrder->getClient());
        query->bindValue(":orders_manager", dialogOrder->getManager());
        query->bindValue(":orders_sum", dialogOrder->getSum());
        query->bindValue(":orders_status", dialogOrder->getStatus());

        query->exec();

        int t_lastIns = query->lastInsertId().toInt();

        for(int i = 0; i < dialogOrder->getGoodsOrder().count(); i++)
        {
            query->prepare("INSERT INTO ordergoods (ordergoods_order, ordergoods_good, ordergoods_count ) "
                           "VALUES (:ordergoods_order, :ordergoods_good, :ordergoods_count)");
            query->bindValue(":ordergoods_order", t_lastIns);
            query->bindValue(":ordergoods_good", dialogOrder->getGoodsOrder()[i].id);
            query->bindValue(":ordergoods_count", dialogOrder->getGoodsOrder()[i].count);

            query->exec();

            query->prepare("UPDATE goods "
                           "SET goods_count = goods_count - :goods_count "
                           "WHERE goods_idgood = :goods_idgood");
            query->bindValue(":goods_count", dialogOrder->getGoodsOrder()[i].count);
            query->bindValue(":goods_idgood", dialogOrder->getGoodsOrder()[i].id);

            query->exec();
        }

        updateOrders();
    }

    delete dialogOrder;
}

void MainWindow::on_actionEditOrder_triggered()
{

}

void MainWindow::on_actionRemoveOrder_triggered()
{
    if(ui->tableWidgetOrders->currentRow() < 0)
        return;

    query->prepare("DELETE FROM orders "
                   "WHERE orders_idorder = :orders_idorder");
    query->bindValue(":orders_idorder", orders_rcd[ui->tableWidgetOrders->currentRow()]);

    query->exec();

    updateOrders();
}

void MainWindow::on_actionAddGood_triggered()
{
    DialogGood *dialogGood = new DialogGood();

    query->prepare("SELECT categories_idcategory, categories_name "
                   "FROM categories");
    query->exec();

    while(query->next())
        dialogGood->addCategory(query->value("categories_idcategory").toInt(),
                                query->value("categories_name").toString());

    dialogGood->exec();

    if(dialogGood->getState() == 1)
    {
        query->prepare("INSERT INTO goods (goods_rcd, goods_category, goods_name, goods_price, goods_count) "
                       "VALUES (:goods_rcd, :goods_category, :goods_name, :goods_price, :goods_count)");
        query->bindValue(":goods_rcd", dialogGood->getRcd());
        query->bindValue(":goods_category", dialogGood->getCategory());
        query->bindValue(":goods_name", dialogGood->getName());
        query->bindValue(":goods_price", dialogGood->getPrice());
        query->bindValue(":goods_count", dialogGood->getCount());

        query->exec();

        updateGoods();
    }

    delete dialogGood;
}

void MainWindow::on_actionEditGood_triggered()
{
    DialogGood *dialogGood = new DialogGood();

    query->prepare("SELECT categories_idcategory, categories_name "
                   "FROM categories");
    query->exec();

    while(query->next())
        dialogGood->addCategory(query->value("categories_idcategory").toInt(),
                                query->value("categories_name").toString());

    query->prepare("SELECT goods_idgood, goods_rcd, goods_category, goods_name, goods_price, goods_count "
                   "FROM goods "
                   "WHERE goods_idgood = :goods_idgood");
    query->bindValue(":goods_idgood", goods_rcd[ui->tableWidgetGoods->currentRow()]);
    query->exec();
    query->next();

    dialogGood->setRcd(query->value("goods_rcd").toString());
    dialogGood->setCategory(query->value("goods_category").toInt());
    dialogGood->setName(query->value("goods_name").toString());
    dialogGood->setPrice(query->value("goods_price").toDouble());
    dialogGood->setCount(query->value("goods_count").toDouble());

    dialogGood->exec();

    if(dialogGood->getState() == 1)
    {
        query->prepare("UPDATE goods "
                       "SET goods_rcd = :goods_rcd, goods_category = :goods_category, goods_name = :goods_name, goods_price = :goods_price, goods_count = :goods_count "
                       "WHERE goods_idgood = :goods_idgood");
        query->bindValue(":goods_idgood", goods_rcd[ui->tableWidgetGoods->currentRow()]);
        query->bindValue(":goods_rcd", dialogGood->getRcd());
        query->bindValue(":goods_category", dialogGood->getCategory());
        query->bindValue(":goods_name", dialogGood->getName());
        query->bindValue(":goods_price", dialogGood->getPrice());
        query->bindValue(":goods_count", dialogGood->getCount());

        query->exec();

        updateGoods();
    }

    delete dialogGood;
}

void MainWindow::on_actionRemoveGood_triggered()
{
    if(ui->tableWidgetGoods->currentRow() < 0)
        return;

    query->prepare("DELETE FROM goods "
                   "WHERE goods_idgood = :goods_idgood");
    query->bindValue(":goods_idgood", goods_rcd[ui->tableWidgetGoods->currentRow()]);

    query->exec();

    updateOrders();
    updateGoods();
}

void MainWindow::on_actionAddCategory_triggered()
{
    QString n_category = QInputDialog::getText(this, tr("Добавить категорию"), "Введите название новой категории");

    if(n_category.isEmpty()) return;

    query->prepare("INSERT INTO categories (categories_name) "
                   "VALUES (:categories_name)");
    query->bindValue(":categories_name", n_category);

    query->exec();

    updateCategories();
}

void MainWindow::on_actionEditCategory_triggered()
{
    QString n_category = QInputDialog::getText(this, tr("Добавить категорию"), tr("Введите название новой категории"), QLineEdit::Normal, ui->tableWidgetCategories->currentItem()->text());

    if(n_category.isEmpty()) return;

    query->prepare("UPDATE categories "
                   "SET categories_name = :categories_name "
                   "WHERE categories_idcategory = :categories_idcategory");
    query->bindValue(":categories_idcategory", categories_rcd[ui->tableWidgetCategories->currentRow()]);
    query->bindValue(":categories_name", n_category);

    query->exec();
    qDebug() << query->boundValues();
    qDebug() << query->lastError().text();

    updateCategories();
}

void MainWindow::on_actionRemoveCategory_triggered()
{
    if(ui->tableWidgetCategories->currentRow() < 0)
        return;

    query->prepare("DELETE FROM categories "
                   "WHERE categories_idcategory = :categories_idcategory");
    query->bindValue(":categories_idcategory", categories_rcd[ui->tableWidgetCategories->currentRow()]);

    query->exec();

    updateCategories();
    updateGoods();
}

void MainWindow::on_actionAddClient_triggered()
{
    DialogClient *dialogClient = new DialogClient();

    query->prepare("SELECT employees_idemployee, employees_surname, employees_name, employees_patronymic "
                   "FROM employees");
    query->exec();

    while(query->next())
        dialogClient->addManager(query->value("employees_idemployee").toInt(),
                                 query->value("employees_surname").toString(),
                                 query->value("employees_name").toString(),
                                 query->value("employees_patronymic").toString());

    dialogClient->exec();

    if(dialogClient->getState() == 1)
    {
        query->prepare("INSERT INTO clients (clients_surname, clients_name, clients_patronymic, clients_phone, clients_email, clients_manager) "
                       "VALUES (:surname, :name, :patronymic, :phone, :email, :manager)");
        query->bindValue(":surname", dialogClient->getSurname());
        query->bindValue(":name", dialogClient->getName());
        query->bindValue(":patronymic", dialogClient->getPatronymic());
        query->bindValue(":phone", dialogClient->getPhone());
        query->bindValue(":email", dialogClient->getEmail());
        query->bindValue(":manager", dialogClient->getManagerId());

        query->exec();

        updateClients();
    }

    delete dialogClient;
}

void MainWindow::on_actionEditClient_triggered()
{
    DialogClient *dialogClient = new DialogClient();

    query->prepare("SELECT employees_idemployee, employees_surname, employees_name, employees_patronymic "
                   "FROM employees");
    query->exec();

    while(query->next())
        dialogClient->addManager(query->value("employees_idemployee").toInt(),
                                 query->value("employees_surname").toString(),
                                 query->value("employees_name").toString(),
                                 query->value("employees_patronymic").toString());

    query->prepare("SELECT clients_surname, clients_name, clients_patronymic, clients_phone, clients_email, clients_manager "
                   "FROM clients "
                   "WHERE clients_rcd = :clients_rcd");
    query->bindValue(":clients_rcd", clients_rcd[ui->tableWidgetClients->currentRow()]);
    query->exec();
    query->next();

    dialogClient->setSurname(query->value("clients_surname").toString());
    dialogClient->setName(query->value("clients_name").toString());
    dialogClient->setPatronymic(query->value("clients_patronymic").toString());
    dialogClient->setPhone(query->value("clients_phone").toString());
    dialogClient->setEmail(query->value("clients_email").toString());
    dialogClient->setManagerId(query->value("clients_manager").toInt());

    dialogClient->exec();

    if(dialogClient->getState() == 1)
    {
        query->prepare("UPDATE clients "
                       "SET clients_surname = :clients_surname, clients_name = :clients_name, clients_patronymic = :clients_patronymic, clients_phone = :clients_phone, clients_email = :clients_email, clients_manager = :clients_manager "
                       "WHERE clients_rcd = :clients_rcd");
        query->bindValue(":clients_rcd", clients_rcd[ui->tableWidgetClients->currentRow()]);
        query->bindValue(":clients_surname", dialogClient->getSurname());
        query->bindValue(":clients_name", dialogClient->getName());
        query->bindValue(":clients_patronymic", dialogClient->getPatronymic());
        query->bindValue(":clients_phone", dialogClient->getPhone());
        query->bindValue(":clients_email", dialogClient->getEmail());
        query->bindValue(":clients_manager", dialogClient->getManagerId());

        query->exec();

        updateClients();
    }

    delete dialogClient;
}

void MainWindow::on_actionRemoveClient_triggered()
{
    if(ui->tableWidgetClients->currentRow() < 0)
        return;

    query->prepare("DELETE FROM clients "
                   "WHERE clients_rcd = :clients_rcd");
    query->bindValue(":clients_rcd", clients_rcd[ui->tableWidgetClients->currentRow()]);

    query->exec();

    updateOrders();
    updateClients();
}

void MainWindow::on_actionAddEmployee_triggered()
{
    DialogEmployee *dialogEmployee = new DialogEmployee();

    dialogEmployee->exec();

    if(dialogEmployee->getState() == 1)
    {
        query->prepare("INSERT INTO employees (employees_surname, employees_name, employees_patronymic, employees_birthday, employees_username, employees_password) "
                       "VALUES (:surname, :name, :patronymic, :birthday, :login, :password)");
        query->bindValue(":surname", dialogEmployee->getSurname());
        query->bindValue(":name", dialogEmployee->getName());
        query->bindValue(":patronymic", dialogEmployee->getPatronymic());
        query->bindValue(":birthday", dialogEmployee->getBirthday().toString(""));
        query->bindValue(":login", dialogEmployee->getLogin());
        query->bindValue(":password", dialogEmployee->getPassword());

        query->exec();

        updateEmployees();
    }

    delete dialogEmployee;
}

void MainWindow::on_actionEditEmployee_triggered()
{
    DialogEmployee *dialogEmployee = new DialogEmployee();

    query->prepare("SELECT employees_surname, employees_name, employees_patronymic, employees_birthday, employees_username "
                   "FROM employees "
                   "WHERE employees_idemployee = :employees_idemployee");
    query->bindValue(":employees_idemployee", emplyoees_rcd[ui->tableWidgetEmployees->currentRow()]);
    query->exec();
    query->next();

    dialogEmployee->setSurname(query->value("employees_surname").toString());
    dialogEmployee->setName(query->value("employees_name").toString());
    dialogEmployee->setPatronymic(query->value("employees_patronymic").toString());
    dialogEmployee->setBirthday(query->value("employees_birthday").toDate());
    dialogEmployee->setLogin(query->value("employees_username").toString());

    dialogEmployee->exec();

    if(dialogEmployee->getState() == 1)
    {
        query->prepare("UPDATE employees "
                       "SET employees_surname = :employees_surname, employees_name = :employees_name, employees_patronymic = :employees_patronymic, "
                       "employees_birthday = :employees_birthday, employees_username = :employees_username, employees_password = 0employees_password: "
                       "WHERE employees_idemployee = :employees_idemployee");
        query->bindValue(":employees_idemployee", emplyoees_rcd[ui->tableWidgetEmployees->currentRow()]);
        query->bindValue(":surname", dialogEmployee->getSurname());
        query->bindValue(":name", dialogEmployee->getName());
        query->bindValue(":patronymic", dialogEmployee->getPatronymic());
        query->bindValue(":birthday", dialogEmployee->getBirthday().toString("yyyy-MM-dd"));
        query->bindValue(":login", dialogEmployee->getLogin());
        query->bindValue(":password", dialogEmployee->getPassword());

        query->exec();

        updateEmployees();
    }

    delete dialogEmployee;
}

void MainWindow::on_actionRemoveEmployee_triggered()
{
    if(ui->tableWidgetEmployees->currentRow() < 0)
        return;

    query->prepare("DELETE FROM employees "
                   "WHERE employees_idemployee = :employees_idemployee");
    query->bindValue(":employees_idemployee", emplyoees_rcd[ui->tableWidgetEmployees->currentRow()]);

    query->exec();

    updateOrders();
    updateClients();
    updateEmployees();
}

void MainWindow::on_actionLock_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::updateOrders()
{
    orders_rcd.clear();
    ui->tableWidgetOrders->clearContents();
    ui->tableWidgetOrders->setRowCount(0);

    query->prepare("SELECT orders_idorder, orders_rcd, orders_date, orders_time, "
                   "clients_surname, clients_name, clients_patronymic, "
                   "employees_surname, employees_name, employees_patronymic,"
                   "orders_sum, status_name "
                   "FROM orders "
                   "LEFT JOIN clients ON clients_rcd = orders_client "
                   "LEFT JOIN employees ON employees_idemployee = orders_manager "
                   "LEFT JOIN status ON status_id = orders_status");

    qDebug() << query->exec();
    qDebug() << query->lastError().text();

    while(query->next())
    {
        ui->tableWidgetOrders->setRowCount(ui->tableWidgetOrders->rowCount() + 1);

        orders_rcd.push_back(query->value("orders_idorder").toInt());

        QTableWidgetItem *itemRcd       = new QTableWidgetItem();
        QTableWidgetItem *itemData      = new QTableWidgetItem();
        QTableWidgetItem *itemTime      = new QTableWidgetItem();
        QTableWidgetItem *itemClient    = new QTableWidgetItem();
        QTableWidgetItem *itemManager   = new QTableWidgetItem();
        QTableWidgetItem *itemSum       = new QTableWidgetItem();
        QTableWidgetItem *itemSumPDV       = new QTableWidgetItem();
        QTableWidgetItem *itemStatus    = new QTableWidgetItem();

        itemRcd->setText(query->value("orders_rcd").toString());
        itemData->setText(query->value("orders_date").toDate().toString("dd.MM.yyyy"));
        itemTime->setText(query->value("orders_time").toTime().toString("hh:mm:ss"));
        itemClient->setText(QString("%1 %2 %3")
                            .arg(query->value("clients_surname").toString())
                            .arg(query->value("clients_name").toString())
                            .arg(query->value("clients_patronymic").toString()));
        itemManager->setText(QString("%1 %2 %3")
                             .arg(query->value("employees_surname").toString())
                             .arg(query->value("employees_name").toString())
                             .arg(query->value("employees_patronymic").toString()));
        itemSum->setText(query->value("orders_sum").toString());
        itemSumPDV->setText(QString::number(query->value("orders_sum").toDouble() * 1.2));
        itemStatus->setText(query->value("status_name").toString());

        itemRcd->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemData->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemTime->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemClient->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemManager->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemSum->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemSumPDV->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemStatus->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 0, itemRcd);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 1, itemData);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 2, itemTime);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 3, itemClient);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 4, itemManager);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 5, itemSum);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 6, itemSumPDV);
        ui->tableWidgetOrders->setItem(ui->tableWidgetOrders->rowCount() - 1, 7, itemStatus);
    }
}

void MainWindow::updateCategories()
{
    categories_rcd.clear();
    ui->tableWidgetCategories->clearContents();
    ui->tableWidgetCategories->setRowCount(0);

    query->prepare("SELECT categories_idcategory, categories_name "
                   "FROM categories ");

    qDebug() << query->exec();

    while(query->next())
    {
        ui->tableWidgetCategories->setRowCount(ui->tableWidgetCategories->rowCount() + 1);

        categories_rcd.push_back(query->value("categories_idcategory").toInt());

        QTableWidgetItem *itemName  = new QTableWidgetItem();

        itemName->setText(query->value("categories_name").toString());

        itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetCategories->setItem(ui->tableWidgetCategories->rowCount() - 1, 0, itemName);
    }
}

void MainWindow::updateGoods()
{
    goods_rcd.clear();
    ui->tableWidgetGoods->clearContents();
    ui->tableWidgetGoods->setRowCount(0);

    query->prepare("SELECT goods_idgood, goods_rcd, categories_name, goods_name, goods_price, goods_count "
                   "FROM goods "
                   "LEFT JOIN categories ON categories_idcategory = goods_category");

    qDebug() << query->exec();

    while(query->next())
    {
        ui->tableWidgetGoods->setRowCount(ui->tableWidgetGoods->rowCount() + 1);

        goods_rcd.push_back(query->value("goods_idgood").toInt());

        QTableWidgetItem *itemRCD           = new QTableWidgetItem();
        QTableWidgetItem *itemCategoryName  = new QTableWidgetItem();
        QTableWidgetItem *itemName          = new QTableWidgetItem();
        QTableWidgetItem *itemPrice         = new QTableWidgetItem();
        QTableWidgetItem *itemPricePDV      = new QTableWidgetItem();
        QTableWidgetItem *itemCount         = new QTableWidgetItem();

        itemRCD->setText(query->value("goods_rcd").toString());
        itemCategoryName->setText(query->value("categories_name").toString());
        itemName->setText(query->value("goods_name").toString());
        itemPrice->setText(query->value("goods_price").toString());
        itemPricePDV->setText(QString::number(query->value("goods_price").toDouble() * 1.2));
        itemCount->setText(query->value("goods_count").toString());

        itemRCD->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemCategoryName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemPrice->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemPricePDV->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemCount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 0, itemRCD);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 1, itemCategoryName);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 2, itemName);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 3, itemPrice);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 4, itemPricePDV);
        ui->tableWidgetGoods->setItem(ui->tableWidgetGoods->rowCount() - 1, 5, itemCount);
    }
}

void MainWindow::updateClients()
{
    clients_rcd.clear();
    ui->tableWidgetClients->clearContents();
    ui->tableWidgetClients->setRowCount(0);

    query->prepare("SELECT clients_rcd, clients_surname, clients_name, clients_patronymic, clients_phone, clients_email, employees_surname, employees_name, employees_patronymic "
                   "FROM clients "
                   "LEFT JOIN employees ON clients_manager = employees_idemployee");

    query->exec();

    while(query->next())
    {
        ui->tableWidgetClients->setRowCount(ui->tableWidgetClients->rowCount() + 1);

        clients_rcd.push_back(query->value("clients_rcd").toInt());

        QTableWidgetItem *itemFIO       = new QTableWidgetItem();
        QTableWidgetItem *itemPhone          = new QTableWidgetItem();
        QTableWidgetItem *itemEmail    = new QTableWidgetItem();
        QTableWidgetItem *itemManager       = new QTableWidgetItem();

        itemFIO->setText(QString("%1 %2 %3")
                         .arg(query->value("clients_surname").toString())
                         .arg(query->value("clients_name").toString())
                         .arg(query->value("clients_patronymic").toString()));
        itemPhone->setText(query->value("clients_phone").toString());
        itemEmail->setText(query->value("clients_email").toString());
        itemManager->setText(QString("%1 %2 %3")
                             .arg(query->value("employees_surname").toString())
                             .arg(query->value("employees_name").toString())
                             .arg(query->value("employees_patronymic").toString()));

        itemFIO->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemPhone->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemEmail->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemManager->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetClients->setItem(ui->tableWidgetClients->rowCount() - 1, 0, itemFIO);
        ui->tableWidgetClients->setItem(ui->tableWidgetClients->rowCount() - 1, 1, itemPhone);
        ui->tableWidgetClients->setItem(ui->tableWidgetClients->rowCount() - 1, 2, itemEmail);
        ui->tableWidgetClients->setItem(ui->tableWidgetClients->rowCount() - 1, 3, itemManager);
    }
}

void MainWindow::updateEmployees()
{
    emplyoees_rcd.clear();
    ui->tableWidgetEmployees->clearContents();
    ui->tableWidgetEmployees->setRowCount(0);

    query->prepare("SELECT employees_idemployee, employees_surname, employees_name, employees_patronymic, employees_birthday, employees_username "
                   "FROM employees");

    query->exec();

    while(query->next())
    {
        ui->tableWidgetEmployees->setRowCount(ui->tableWidgetEmployees->rowCount() + 1);

        emplyoees_rcd.push_back(query->value("employees_idemployee").toInt());

        QTableWidgetItem *itemSurname       = new QTableWidgetItem();
        QTableWidgetItem *itemName          = new QTableWidgetItem();
        QTableWidgetItem *itemPatronymic    = new QTableWidgetItem();
        QTableWidgetItem *itemBirthday       = new QTableWidgetItem();
        QTableWidgetItem *itemUsername       = new QTableWidgetItem();

        itemSurname->setText(query->value("employees_surname").toString());
        itemName->setText(query->value("employees_name").toString());
        itemPatronymic->setText(query->value("employees_patronymic").toString());
        itemBirthday->setText(query->value("employees_birthday").toDate().toString("dd.MM.yyyy"));
        itemUsername->setText(query->value("employees_username").toString());

        itemSurname->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemPatronymic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemBirthday->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        itemUsername->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetEmployees->setItem(ui->tableWidgetEmployees->rowCount() - 1, 0, itemSurname);
        ui->tableWidgetEmployees->setItem(ui->tableWidgetEmployees->rowCount() - 1, 1, itemName);
        ui->tableWidgetEmployees->setItem(ui->tableWidgetEmployees->rowCount() - 1, 2, itemPatronymic);
        ui->tableWidgetEmployees->setItem(ui->tableWidgetEmployees->rowCount() - 1, 3, itemBirthday);
        ui->tableWidgetEmployees->setItem(ui->tableWidgetEmployees->rowCount() - 1, 4, itemUsername);
    }
}

void MainWindow::on_tableWidgetOrders_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddOrder);
    contextTableMenu.addAction(ui->actionEditOrder);
    contextTableMenu.addAction(ui->actionRemoveOrder);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateOrders);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_tableWidgetCategories_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddCategory);
    contextTableMenu.addAction(ui->actionEditCategory);
    contextTableMenu.addAction(ui->actionRemoveCategory);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateCategories);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_tableWidgetGoods_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddGood);
    contextTableMenu.addAction(ui->actionEditGood);
    contextTableMenu.addAction(ui->actionRemoveGood);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateGoods);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_tableWidgetClients_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddClient);
    contextTableMenu.addAction(ui->actionEditClient);
    contextTableMenu.addAction(ui->actionRemoveClient);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateClients);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_tableWidgetEmployees_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddEmployee);
    contextTableMenu.addAction(ui->actionEditEmployee);
    contextTableMenu.addAction(ui->actionRemoveEmployee);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateEmployees);

    QPoint globalPos = ui->tableWidgetGoods->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_actionUpdateOrders_triggered()
{
    updateOrders();
}

void MainWindow::on_actionUpdateGoods_triggered()
{
    updateGoods();
}

void MainWindow::on_actionUpdateCategories_triggered()
{
    updateCategories();
}

void MainWindow::on_actionUpdateClients_triggered()
{
    updateClients();
}

void MainWindow::on_actionUpdateEmployees_triggered()
{
    updateEmployees();
}
