#ifndef PROCESS_H
#define PROCESS_H

void setImage(QImage &newImage);
bool saveImage(QString &fileName);
void adjustScrollBar(QScrollBar *scrollBar, double factor);
void scaleImage(double factor);
void updateImage(); // Met à jour les traitements d openCV
cv::Mat getCvImage();

#endif // PROCESS_H
