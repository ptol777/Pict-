#include "process.h"

bool MainWindow::loadFile(QString &fileName){ // retourne un boolean indiquant la reussite ou le echec du chargement
    QImageReader reader(fileName);
    reader.setAutoTransform(true); // applique les méta données associés à l'image
    QImage newImage = reader.read();

    if (newImage.isNull()) { // le chemin est incorrect et aucune image n'est chargée
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),tr("Unable to load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString())); // affiche une erreur
        // le chemin est affiché suivant le séparateur approprié
        return false;
    }

    setImage(newImage);
    setWindowFilePath(fileName); // récupère le chemin de l'image chargée

    cvImage = cv::imread(fileName.toStdString()); // récupère la version utilisée par opencv

    QString message = tr("Image \"%1\", %2x%3").arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height());
    statusBar()->showMessage(message); // Affiche en bas le chemin et la taille de l'image chargée

    return true;
}

void MainWindow::setImage(QImage &newImage){
    image = newImage;
    imageLabel->setPixmap(QPixmap::fromImage(image)); //QPixmap est plus approprié pour afficher l'image, QImage est pour le chargement
    scaleFactor = 1.0;

    scrollArea->setVisible(true); // remarquable par le fond gris
    updateActions();
    createDock();
    imageLabel->resize(imageLabel->pixmap()->size()); // configure une taille raisonnable du label et donc de l'image contenue
}
