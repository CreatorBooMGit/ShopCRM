#ifndef DIALOGGOOD_H
#define DIALOGGOOD_H

#include <QDialog>

namespace Ui {
    class DialogGood;
}

class DialogGood : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGood(QWidget *parent = 0);
    ~DialogGood();

    void addCategory(int rcd, QString name);

    void setRcd(const QString &value);
    void setCategory(int value);
    void setName(const QString &value);
    void setPrice(const double &value);
    void setCount(int value);

    QString getRcd() const;
    int getCategory() const;
    QString getName() const;
    double getPrice() const;
    int getCount() const;

    int getState() const;

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

    void on_doubleSpinBoxPrice_valueChanged(double arg1);

private:
    Ui::DialogGood *ui;

    int state = 0;

    QString rcd;
    int     category;
    QString name;
    double   price;
    int     count;

    QVector<int> category_rcd;
};

#endif // DIALOGGOOD_H
