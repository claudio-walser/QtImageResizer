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
    QString directory = QFileDialog::getExistingDirectory(
        this,
        tr("Quellverzeichnis wählen"),
        "", // home directory to start from
        QFileDialog::ShowDirsOnly
    );
    if (!listSources.contains(directory)) {
        listSources.append(directory);
        //modelSources.submit(); //boy, it would just make perfectly sense to me :(
        modelSources.setStringList(listSources); // a hack but works at least
    }
}

void QtImageResizer::on_buttonSourceRemove_clicked()
{
    listSources.removeAt(ui->listViewSources->currentIndex().row());
    modelSources.setStringList(listSources);
}

void QtImageResizer::on_buttonResize_clicked()
{
    bool hasError = false;
    QString message;
    // do some checks first
    if (listSources.length() == 0) {
        hasError = true;
        message += "\n- Es muss mindestens ein Quellverzeichnis angegeben werden.";
    }

    if (ui->lineEditTarget->text().length() == 0) {
        hasError = true;
        message += "\n- Es muss ein Zielverzeichnis angegeben werden.";
    }

    if (
        ui->lineEditDirectoryLarge->text().length() == 0 ||
        ui->lineEditHeightLarge->text().length() == 0 ||
        ui->lineEditWidthLarge->text().length() == 0
    ) {
        hasError = true;
        message += "\n- Die Angaben für 'Skalierung - Gross' müssen eingetragen werden";
    }

    if (
        ui->lineEditDirectorySmall->text().length() == 0 ||
        ui->lineEditHeightSmall->text().length() == 0 ||
        ui->lineEditWidthSmall->text().length() == 0
    ) {
        hasError = true;
        message += "\n- Die Angaben für 'Skalierung - Klein' müssen eingetragen werden";
    }

    if (hasError == true) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Fehler in der Eingabe");
        msgBox.setText(message);
        msgBox.exec();

        return;
    }


    // start real processing now
    QString allSources;

    // loop sources list
    foreach (const QString &sourceDirString, listSources) {
        QDir sourceDir(sourceDirString.toUtf8());
        if (!sourceDir.exists()){
          qCritical("Your Source Directory has to exist!");
        }
        // append all sources for info displaying later
        allSources += sourceDirString;

        // read all images
        QStringList nameFilter;
        nameFilter << "*.png" << "*.jpg" << "*.gif";
        QFileInfoList list = sourceDir.entryInfoList( nameFilter, QDir::Files );

        foreach (QFileInfo f, list){
            // target directory for large images
            QDir targetDir(ui->lineEditTarget->text().toUtf8() +
                           "/" + sourceDir.dirName() +
                           "/" + ui->lineEditDirectoryLarge->text().toUtf8() +
                           "/");
            // create if not exists
            if (!targetDir.exists()) {
                targetDir.mkpath(".");
            }

            // target directory for small images
            QDir targetDirSmall(ui->lineEditTarget->text().toUtf8() +
                                "/" + sourceDir.dirName() +
                                "/" + ui->lineEditDirectorySmall->text().toUtf8() +
                                "/");
            // create if not exists
            if (!targetDirSmall.exists()) {
                targetDirSmall.mkpath(".");
            }

            // if source file exists
            if (f.isFile()){
                QImage image;
                // if image loadable
                if(image.load(f.absoluteFilePath().toUtf8())) {
                    // large scale image
                    QImage img = image.scaled(
                        ui->lineEditWidthLarge->text().toInt(),
                        ui->lineEditHeightLarge->text().toInt(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    );



                    img.save(targetDir.absolutePath().toUtf8() + "/" + f.fileName().toUtf8());

                    // small scale image
                    QImage imgSmall = image.scaled(
                        ui->lineEditWidthSmall->text().toInt(),
                        ui->lineEditHeightSmall->text().toInt(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    );
                    imgSmall.save(targetDirSmall.absolutePath().toUtf8() + "/" + f.fileName().toUtf8());
                } else {
                    qCritical("Failed to load image " + f.absoluteFilePath().toUtf8());
                }
            }
        }
    }

    // display information
    QMessageBox::information(
        this,
        tr("Bilder fertig skaliert"),
        tr(
            "Bilder erfolgreich nach " + ui->lineEditTarget->text().toUtf8() + " skaliert" +
            "\n" +
            "\n" +
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
}

void QtImageResizer::on_buttonTarget_clicked()
{
    ui->lineEditTarget->setText(QFileDialog::getExistingDirectory(
        this,
        tr("Zielverzeichnis wählen"),
        ui->lineEditTarget->text().toUtf8(),
        QFileDialog::ShowDirsOnly)
    );
}
