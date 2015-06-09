#ifndef QTIMAGERESIZER_H
#define QTIMAGERESIZER_H

#include <QMainWindow>

namespace Ui {
class QtImageResizer;
}

class QtImageResizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtImageResizer(QWidget *parent = 0);
    ~QtImageResizer();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::QtImageResizer *ui;
};

#endif // QTIMAGERESIZER_H
