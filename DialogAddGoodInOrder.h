#ifndef DIALOGADDGOODINORDER_H
#define DIALOGADDGOODINORDER_H

#include <QDialog>

namespace Ui {
    class DialogAddGoodInOrder;
}

struct Good {
    int     id;
    QString rcd;
    QString name;
    double  price;
    double  sum;
    double  count;
    double  countHave;
};

class DialogAddGoodInOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddGoodInOrder(QWidget *parent = 0);
    ~DialogAddGoodInOrder();

    int getState() const;
    Good getGood() const;

    void setGoods(const QVector<Good> &value);

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
    void on_doubleSpinBoxCount_valueChanged(double arg1);
    void on_doubleSpinBoxSum_valueChanged(double arg1);
    void on_doubleSpinBoxPrice_valueChanged(double arg1);
    void on_toolButtonRcd_clicked();
    void on_toolButtonName_clicked();

private:
    Ui::DialogAddGoodInOrder *ui;

    int state = 0;
    Good good;

    QVector<Good> goods;

    //

    int current = 0;
};

#endif // DIALOGADDGOODINORDER_H
