#ifndef QTIMAGERESIZER_H
#define QTIMAGERESIZER_H

#include <QMainWindow>
#include <QStringListModel>

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
    void on_buttonSourceAdd_clicked();

    void on_buttonSourceRemove_clicked();

    void on_buttonResize_clicked();

    void on_buttonTarget_clicked();

private:
    Ui::QtImageResizer *ui;
    QStringList listSources;
    QStringListModel modelSources;
};

#endif // QTIMAGERESIZER_H
