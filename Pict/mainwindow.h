#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/opencv.hpp"
#include "util.h"

#include <QMainWindow>
#include <QAction>
#include <QApplication>
#include <QToolBar>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageWriter>
#include <QDockWidget>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMetaMethod>
#include <QMenu>
#include <QMenuBar>
#include <QToolButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots: //Slots représentant chaque fonctionnalité du programme
    void open();
    void saveAs();
    void about();
    void zoomIn();
    void zoomOut();
    void flip();
    void flipHorizontal();
    void rotateRight();
    void rotateLeft();
    void threshold();
    void invert();
    void blur();
    void setDefault();
    void dither();
    void optimizeContrast();
    void detectEdges();

private:
    QToolBar *myToolBar; // Barre d'outils traitement basique
    QToolBar *processToolBar; // Barre d'outil traitement avancé
    QImage image;
    QLabel *imageLabel; // espace pour l'image chargée
    QScrollArea *scrollArea; // Espace couvert par le scroll
    double scaleFactor; // change la taille de l'image
    bool grayScale; // permet de savoir si l'image est une nuance de gris
    QString fileName;
    cv::Mat cvImage; // version de l'image par openCV

    QAction *openAct; // Actions correspondant au slots
    QAction *saveAct;
    QAction *aboutAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *flipAct;
    QAction *flipHorizontalAct;
    QAction *rotateRightAct;
    QAction *rotateLeftAct;
    QAction *thresholdAct;
    QAction *invertAct;
    QAction *blurAct;
    QAction *setDefaultAct;
    QAction *ditherAct;
    QAction *optimizeContrastAct;

    void createBasicActions(); // Initialise la barre d'outils et les actions
    void createAdvancedActions(); // outils avancés
    void updateImage(); // Met à jour les traitements d openCV

public:
    MainWindow(QWidget *parent = 0);
    cv::Mat getCvImage();
    void setCvImage(cv::Mat image);
    double getScaleFactor();
    void setScaleFactor(double scaleFactor);
    void increaseScaleFactor(double scaleFactor);
    QLabel* getImageLabel();
    QScrollArea* getScrollArea();
    void setImage(QImage image); // Prend l'image en propriété
    QImage getImage();
    void updateActions(); //Active les actions lorsque l'image est chargée
    QAction* getZoomInAct();
    QAction* getZoomOutAct();

};

#endif // MAINWINDOW_H
