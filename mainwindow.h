#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionHelp_triggered();
    void on_actionRemoveEmployee_triggered();
    void on_actionAddOrder_triggered();
    void on_actionEditOrder_triggered();
    void on_actionRemoveOrder_triggered();
    void on_actionAddGood_triggered();
    void on_actionEditGood_triggered();
    void on_actionRemoveGood_triggered();
    void on_actionAddCategory_triggered();
    void on_actionEditCategory_triggered();
    void on_actionRemoveCategory_triggered();
    void on_actionAddClient_triggered();
    void on_actionEditClient_triggered();
    void on_actionRemoveClient_triggered();
    void on_actionAddEmployee_triggered();
    void on_actionEditEmployee_triggered();
    void on_actionLock_triggered();
    void on_actionExit_triggered();

    void on_tableWidgetOrders_customContextMenuRequested(const QPoint &pos);

    void on_tableWidgetCategories_customContextMenuRequested(const QPoint &pos);

    void on_tableWidgetGoods_customContextMenuRequested(const QPoint &pos);

    void on_tableWidgetClients_customContextMenuRequested(const QPoint &pos);

    void on_tableWidgetEmployees_customContextMenuRequested(const QPoint &pos);

    void on_actionUpdateOrders_triggered();

    void on_actionUpdateGoods_triggered();

    void on_actionUpdateCategories_triggered();

    void on_actionUpdateClients_triggered();

    void on_actionUpdateEmployees_triggered();

private:
    Ui::MainWindow *ui;

    QSqlDatabase database;
    QSqlQuery *query;

    void updateOrders();
    void updateCategories();
    void updateGoods();
    void updateClients();
    void updateEmployees();

    QVector<int> orders_rcd;
    QVector<int> categories_rcd;
    QVector<int> goods_rcd;
    QVector<int> clients_rcd;
    QVector<int> emplyoees_rcd;
};

#endif // MAINWINDOW_H
