#ifndef DIALOGORDER_H
#define DIALOGORDER_H

#include <QDate>
#include <QDialog>

#include "DialogAddGoodInOrder.h"

struct Person {
    int id;
    QString FIO;
};

struct Status {
    int id;
    QString name;
};

namespace Ui {
    class DialogOrder;
}

class DialogOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrder(QWidget *parent = 0);
    ~DialogOrder();

    void    setRcd(const QString &value);
    void    setDate(const QDate &value);
    void    setTime(const QTime &value);
    void    setClient(int value);
    void    setManager(int value);
    void    setSum(double value);
    void    setStatus(int value);

    QString getRcd() const;
    QDate   getDate() const;
    QTime   getTime() const;
    int     getClient() const;
    int     getManager() const;
    double  getSum() const;
    int     getStatus() const;

    QVector<Good> getGoodsOrder() const;

    void addClient(int _id, QString _FIO);
    void addManager(int _id, QString _FIO);
    void addStatus(int _id, QString _name);
    void addGood(int _id, QString _rcd, QString _name, double _price, double _countHave);

    int getState() const;

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_doubleSpinBoxSum_valueChanged(double arg1);
    void on_tableWidgetGoods_customContextMenuRequested(const QPoint &pos);
    void on_actionAddGood_triggered();
    void on_actionEditGoodCount_triggered();
    void on_actionRemoveGood_triggered();

private:
    Ui::DialogOrder *ui;

    int     state = 0;

    QString rcd;
    QDate   date;
    QTime   time;
    int     client;
    int     manager;
    double  sum;
    int     status;

    QVector<Person>     clients_rcd;
    QVector<Person>     managers_rcd;
    QVector<Status>     status_rcd;
    QVector<Good>       goods_rcd;

    QVector<Good>     goodsOrder;
};

#endif // DIALOGORDER_H
