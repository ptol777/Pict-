#include "mytoolbar.h"
#include "mainwindow.h"

MyToolBar::MyToolBar(QWidget *parent)
    :QToolBar(parent){
    setIconSize(QSize(35,35));

    QIcon openIcon = QIcon("/home/erde/Dropbox/Développement/Pict/add.svg"); // cahrge l'icone
    openAct = new QAction(openIcon, tr("Open"), this); // Initialise l'action liée à cette fenetre
    QObject::connect(openAct, &QAction::triggered, this, &MainWindow::open); //connecte le signal de cette action au slot de la fenetre
    addAction(openAct); // Ajoute l'action à la barre d'outils

    QIcon saveIcon = QIcon("/home/erde/Dropbox/Développement/Pict/save.png");
    saveAct = new QAction(saveIcon, tr("Save"), this);
    QObject::connect(saveAct, &QAction::triggered, this, &MainWindow::saveAs);
    saveAct->setEnabled(false); // désactive les fonctionnalités tant que l'image n'est pas chargée
    addAction(saveAct);

    addSeparator();

    QIcon zoomInIcon = QIcon("/home/erde/Dropbox/Développement/Pict/zoom_in.svg");
    zoomInAct = new QAction(zoomInIcon, tr("zoom In"), this);
    QObject::connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
    zoomInAct->setEnabled(false);
    addAction(zoomInAct);

    QIcon zoomOutIcon = QIcon("/home/erde/Dropbox/Développement/Pict/zoom_out.svg");
    zoomOutAct = new QAction(zoomOutIcon, tr("zoom Out"), this);
    QObject::connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
    zoomOutAct->setEnabled(false);
    addAction(zoomOutAct);

    addSeparator();

    QIcon flipIcon = QIcon("/home/erde/Dropbox/Développement/Pict/flip.ico");
    flipAct = new QAction(flipIcon, tr("Flip"), this);
    QObject::connect(flipAct, &QAction::triggered, this, &MainWindow::flip);
    flipAct->setEnabled(false);
    addAction(flipAct);

    QIcon rotateRightIcon = QIcon("/home/erde/Dropbox/Développement/Pict/rotate_right.svg");
    rotateRightAct = new QAction(rotateRightIcon, tr("Rotate Right"), this);
    QObject::connect(rotateRightAct, &QAction::triggered, this, &MainWindow::rotateRight);
    rotateRightAct->setEnabled(false);
    addAction(rotateRightAct);

    QIcon rotateLeftIcon = QIcon("/home/erde/Dropbox/Développement/Pict/rotate_left.svg");
    rotateLeftAct = new QAction(rotateLeftIcon, tr("Rotate Left"), this);
    QObject::connect(rotateLeftAct, &QAction::triggered, this, &MainWindow::rotateLeft);
    rotateLeftAct->setEnabled(false);
    addAction(rotateLeftAct);

}
