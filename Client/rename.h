#ifndef RENAME_H
#define RENAME_H

#include <QDialog>

namespace Ui {
class Rename;
}

class Rename : public QDialog
{
    Q_OBJECT

public:
    explicit Rename(QWidget *parent = 0);
    QString value();
    ~Rename();
    void accept();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::Rename *ui;
    QString name;
};

#endif // RENAME_H
