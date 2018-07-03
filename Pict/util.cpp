#include "util.h"
#include "mainwindow.h"

bool loadFile(MainWindow *window, QString &fileName){ // retourne un boolean indiquant la reussite ou le echec du chargement
    QImageReader reader(fileName);
    reader.setAutoTransform(true); // applique les méta données associés à l'image
    QImage newImage = reader.read();

    if (newImage.isNull()) { // le chemin est incorrect et aucune image n'est chargée
        QMessageBox::warning(window, QGuiApplication::applicationDisplayName(),window->tr("Unable to load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString())); // affiche une erreur
        // le chemin est affiché suivant le séparateur approprié
        return false;
    }

    setMyImage(window, newImage);
    window->setWindowFilePath(fileName); // récupère le chemin de l'image chargée

    window->setCvImage(cv::imread(fileName.toStdString())); // récupère la version utilisée par opencv);

    QString message = window->tr("Image \"%1\", %2x%3").arg(QDir::toNativeSeparators(fileName)).arg(window->getImage().width()).arg(window->getImage().height());
    window->statusBar()->showMessage(message); // Affiche en bas le chemin et la taille de l'image chargée

    return true;
}

void setMyImage(MainWindow *window, QImage &newImage){
    window->setImage(newImage);
    window->getImageLabel()->setPixmap(QPixmap::fromImage(window->getImage())); //QPixmap est plus approprié pour afficher l'image, QImage est pour le chargement
    window->setScaleFactor(1.0);

    window->getScrollArea()->setVisible(true); // remarquable par le fond gris
    window->updateActions();
    window->getImageLabel()->resize(window->getImageLabel()->pixmap()->size()); // configure une taille raisonnable du label et donc de l'image contenue
}

void flipImage(Mat &cvImage, int orientation){
    cv::Mat tempImage;
    tempImage = cvImage;
    cv::flip(tempImage, cvImage, orientation); // on ne peut flipper avec la meme image comme sortie d ou la temp
}

void rotateRightImage(Mat &cvImage){
    cv::Point2i center(cvImage.cols/2, cvImage.rows/2); // Centre de rotation
    cv::Mat rot = cv::getRotationMatrix2D(center, 90, -1);
    cv::warpAffine(cvImage, cvImage, rot, cvImage.size(), cv::INTER_NEAREST);
}

void rotateLeftImage(Mat &cvImage){
    cv::Point2i center(cvImage.cols/2, cvImage.rows/2); // Centre de rotation
    cv::Mat rot = cv::getRotationMatrix2D(center, 90, 1);
    cv::warpAffine(cvImage, cvImage, rot, cvImage.size(), cv::INTER_NEAREST);

}

void invertImage(Mat &cvImage){
    cv::Mat tempImage;
    tempImage = cvImage;
    cv::bitwise_not(tempImage, cvImage);
}

void blurImage(Mat &cvImage){
    cv::Mat tempImage;
    tempImage = cvImage;
    cv::GaussianBlur(tempImage, cvImage, cv::Size(11, 11), 0);
}

uint8_t saturated_add(uint8_t val1, int8_t val2){
  int16_t val1_int = val1;
  int16_t val2_int = val2;
  int16_t tmp = val1_int + val2_int;

  if(tmp > 255){
    return 255;
  }
  else if(tmp < 0){
    return 0;
  }
  else{
    return tmp;
  }
}

void ditherImage(Mat &cvImage){
    Mat dithImg;
    dithImg = cvImage;

    //cvtColor(cvImage, dithImg, CV_BGR2GRAY);
    int imgWidth = dithImg.cols;
    int imgHeight = dithImg.rows;
    int err;
    int8_t a,b,c,d;
    for(int i=0; i<imgHeight; i++){
        for(int j=0; j<imgWidth; j++){
            if(dithImg.at<uint8_t>(i,j) > 127){
                err = dithImg.at<uint8_t>(i,j) - 255;
                dithImg.at<uint8_t>(i,j) = 255;
            }
            else{
                err = dithImg.at<uint8_t>(i,j) - 0;
                dithImg.at<uint8_t>(i,j) = 0;
            }
            a = (err * 7) / 16;
            b = (err * 1) / 16;
            c = (err * 5) / 16;
            d = (err * 3) / 16;
            if((i != (imgHeight-1)) && (j != 0) && (j != (imgWidth - 1))){
                dithImg.at<uint8_t>(i+0,j+1) = saturated_add(dithImg.at<uint8_t>(i+0,j+1),a);
                dithImg.at<uint8_t>(i+1,j+1) = saturated_add(dithImg.at<uint8_t>(i+1,j+1),b);
                dithImg.at<uint8_t>(i+1,j+0) = saturated_add(dithImg.at<uint8_t>(i+1,j+0),c);
                dithImg.at<uint8_t>(i+1,j-1) = saturated_add(dithImg.at<uint8_t>(i+1,j-1),d);
          }
        }
      }
    cvImage = dithImg;
}

void scaleImage(MainWindow *window, double factor){ // Ajuste la taille de l'image suivant factor
    Q_ASSERT(window->getImageLabel()->pixmap());
    window->increaseScaleFactor(factor);
    window->getImageLabel()->resize(window->getScaleFactor() * window->getImageLabel()->pixmap()->size()); // Ajustement

    window->getScrollArea()->horizontalScrollBar()
            ->setValue(int(factor * window->getScrollArea()->horizontalScrollBar()->value())); // ajuste en fonction de la taille de l'image
    window->getScrollArea()->verticalScrollBar()
            ->setValue(int(factor * window->getScrollArea()->verticalScrollBar()->value())); // ajuste en fonction de la taille de l'image

    window->getZoomInAct()->setEnabled(window->getScaleFactor() < 2.5); // désactive les zoom pour les tailles max et min, 250% et 2.5 %
    window->getZoomOutAct()->setEnabled(window->getScaleFactor() > 0.25);
}
