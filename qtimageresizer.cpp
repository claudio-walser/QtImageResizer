#include "qtimageresizer.h"
#include "ui_qtimageresizer.h"
#include <QFileDialog>
#include <QMessageBox>

QtImageResizer::QtImageResizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtImageResizer)
{
    ui->setupUi(this);
}

QtImageResizer::~QtImageResizer()
{
    delete ui;
}

void QtImageResizer::on_pushButton_2_clicked()
{
    ui->lineEdit_4->setText(QFileDialog::getExistingDirectory(this, tr("Quellverzeichnis wählen"),
                                                ui->lineEdit_4->text().toUtf8(),
                                                QFileDialog::ShowDirsOnly));
}

void QtImageResizer::on_pushButton_3_clicked()
{
    ui->lineEdit_3->setText(QFileDialog::getExistingDirectory(this, tr("Zielverzeichnis wählen"),
                                                ui->lineEdit_3->text().toUtf8(),
                                                QFileDialog::ShowDirsOnly));
}

void QtImageResizer::on_pushButton_clicked()
{
    QMessageBox::information(
            this,
            tr("An information message."),
            tr(
                    "Quellverzeichnis: " + ui->lineEdit_4->text().toUtf8() + "\n" +
                    "Zielverzeichnis: " + ui->lineEdit_3->text().toUtf8() + "\n" +
                    "Breite: " + ui->lineEdit->text().toUtf8() + "\n" +
                    "Höhe: " + ui->lineEdit_2->text().toUtf8() + "\n" +
                    "Proportional: " + (ui->checkBox->checkState() ? "Ja" : "Nein") + "\n" +
                    QString("Qualität: %1").arg(ui->spinBox->value()).toUtf8()

            )

    );

    QDir sourceDir(ui->lineEdit_4->text().toUtf8());
    if (!sourceDir.exists()){
      qCritical("Your Source Directory has to exist!");
    }

    qCritical("So far so good");

    QStringList nameFilter;
    nameFilter << "*.png" << "*.jpg" << "*.gif";
    QFileInfoList list = sourceDir.entryInfoList( nameFilter, QDir::Files );

    foreach (QFileInfo f, list){
        if (f.isFile()){
            QImage* image = new QImage();

            if(image->load(f.absoluteFilePath().toUtf8())) {
                QImage img = image->scaled(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt(), Qt::KeepAspectRatio );

                img.save(ui->lineEdit_3->text().toUtf8() + "/" + f.fileName().toUtf8());
            } else {
                qCritical("Failed to load image " + f.absoluteFilePath().toUtf8());
            }
        }
    }

}
