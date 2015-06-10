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
    QString allSources = "";
    foreach (const QString &str, listSources) {
        allSources += str;
    }
    QMessageBox::information(
            this,
            tr("An information message."),
            tr(
                    "Quellverzeichnis: " + allSources.toUtf8() + "\n" +
                    "Zielverzeichnis: " + ui->lineEditTarget->text().toUtf8() + "\n" +
                    "\n" +
                    "Unterverzeichnis Gross: " + ui->lineEditDirectoryLarge->text().toUtf8() + "\n" +
                    "Breite Gross: " + ui->lineEditWidthLarge->text().toUtf8() + "\n" +
                    "Höhe Gross: " + ui->lineEditHeightLarge->text().toUtf8() + "\n" +
                    "\n" +
                    "Unterverzeichnis Klein: " + ui->lineEditDirectorySmall->text().toUtf8() + "\n" +
                    "Breite Klein: " + ui->lineEditWidthSmall->text().toUtf8() + "\n" +
                    "Höhe Klein: " + ui->lineEditHeightSmall->text().toUtf8() + "\n"

            )

    );
    foreach (const QString &sourceDirString, listSources) {
        QDir sourceDir(sourceDirString.toUtf8());
        if (!sourceDir.exists()){
          qCritical("Your Source Directory has to exist!");
        }
        qCritical("Process Source Directory");
        QStringList nameFilter;
        nameFilter << "*.png" << "*.jpg" << "*.gif";
        QFileInfoList list = sourceDir.entryInfoList( nameFilter, QDir::Files );

        foreach (QFileInfo f, list){
            if (f.isFile()){
                QImage* image = new QImage();

                if(image->load(f.absoluteFilePath().toUtf8())) {
                    QImage img = image->scaled(ui->lineEditWidthLarge->text().toInt(), ui->lineEditHeightLarge->text().toInt(), Qt::KeepAspectRatio );
                    QDir targetDir(ui->lineEditTarget->text().toUtf8() + "/" + sourceDir.dirName() + "/" + ui->lineEditDirectoryLarge->text().toUtf8() + "/");
                    if (!targetDir.exists()) {
                        qCritical("Create directory: " + targetDir.absolutePath().toUtf8());
                        targetDir.mkpath(".");
                    }
                    img.save(ui->lineEditTarget->text().toUtf8() + "/" + sourceDir.dirName() + "/" + ui->lineEditDirectoryLarge->text().toUtf8() + "/" + f.fileName().toUtf8());

                    QImage imgSmall = image->scaled(ui->lineEditWidthSmall->text().toInt(), ui->lineEditHeightSmall->text().toInt(), Qt::KeepAspectRatio );
                    QDir targetDirSmall(ui->lineEditTarget->text().toUtf8() + "/" + sourceDir.dirName() + "/" + ui->lineEditDirectorySmall->text().toUtf8() + "/");
                    if (!targetDirSmall.exists()) {
                        qCritical("Create directory: " + targetDirSmall.absolutePath().toUtf8());
                        targetDirSmall.mkpath(".");
                    }
                    imgSmall.save(ui->lineEditTarget->text().toUtf8() + "/" + sourceDir.dirName() + "/" + ui->lineEditDirectorySmall->text().toUtf8() + "/" + f.fileName().toUtf8());


                } else {
                    qCritical("Failed to load image " + f.absoluteFilePath().toUtf8());
                }
            }
        }





    }

}

void QtImageResizer::on_buttonTarget_clicked()
{
    ui->lineEditTarget->setText(QFileDialog::getExistingDirectory(this, tr("Zielverzeichnis wählen"),
                                                ui->lineEditTarget->text().toUtf8(),
                                                QFileDialog::ShowDirsOnly));

}
