#include "qtimageresizer.h"
#include "ui_qtimageresizer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

QtImageResizer::QtImageResizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtImageResizer)
{
    ui->setupUi(this);

    // setup listModel and listView
    modelSources.setStringList(listSources);
    ui->listViewSources->setModel(&modelSources);
}

QtImageResizer::~QtImageResizer()
{
    delete ui;
}

void QtImageResizer::on_buttonSourceAdd_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Quellverzeichnis wählen"),
                                                "", // home directory to start from
                                                QFileDialog::ShowDirsOnly);
    if (!listSources.contains(directory)) {
        listSources.append(directory);
        //modelSources.submit(); //boy, it would just make perfectly sense to me :(
        modelSources.setStringList(listSources); // a hack but works at least
    } else {
        qCritical(directory.toUtf8() + " - Already known as a source!");
    }
}

void QtImageResizer::on_buttonSourceRemove_clicked()
{
    listSources.removeAt(ui->listViewSources->currentIndex().row());
    modelSources.setStringList(listSources);
    qCritical("ui->listView->currentIndex().row()");
}

void QtImageResizer::on_buttonResize_clicked()
{
    QMessageBox::information(
        this,
        tr("An information message."),
        tr("Does nothing yet")
    );

}
