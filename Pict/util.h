#ifndef UTIL_H
#define UTIL_H

#include "opencv2/opencv.hpp"
#include <QString>
#include <QImage>
#include <QScrollBar>

using namespace cv;

class MainWindow;

bool loadFile(MainWindow *window, QString &fileName); // Charge l'image
void setMyImage(MainWindow *window, QImage &newImage); // assigne l'image entant qu'attribut
void scaleImage(MainWindow *window, double factor); // Change la taille de l'image en fonction d'un facteur
void flipImage(Mat &cvImage, int orientation);
void rotateRightImage(Mat &cvImage);
void rotateLeftImage(Mat &cvImage);
void invertImage(Mat &cvImage);
void blurImage(Mat &cvImage);
void ditherImage(Mat &cvImage);

#endif // UTIL_H
