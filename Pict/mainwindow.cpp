#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){

    ///Initialisation
    imageLabel= new QLabel();
    scrollArea = new QScrollArea();
    scaleFactor = 1; // permet de conaitre l'ajustement de la taille de l'image
    grayScale = false; // identifie si l'image est une nuance de gris
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // permet de changer la taille de l'image
    imageLabel->setScaledContents(true); // agrandit l'image en meme tant que le QLabel (pour le zoom)

    ///Création du scroll
    scrollArea->setBackgroundRole(QPalette::Dark); // fond grisé pour l'esapce non occupée par l'image
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false); // affiché uniquement lorsqu'une image est chargée
    setCentralWidget(scrollArea);

    ///Créaion des actions
    createBasicActions();
    createAdvancedActions();

    ///Gestion taille de la fenetre
    QSize optimalSize = QGuiApplication::primaryScreen()->availableSize() * 3 / 5; // taille optimale de la fenetre, 3/5 de l'écran
    resize(optimalSize); // donne taille par defaut de la fenetre
    setMinimumSize(optimalSize); // taille minimale de la fenetre
}

void MainWindow::createBasicActions(){
    ///Initialisation
    myToolBar = addToolBar(tr("Basic tools"));
    myToolBar->setIconSize(QSize(35,35));

    ///Ajout des actions à la barre d'outils et barre de menu avec icones, raccourci
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    //Ouvrir
    openAct = myToolBar->addAction(QIcon(":/myresources/add.svg"), tr("Open"), this, &MainWindow::open); // Ajout de l'action à la barre d'outils,
    //Association de l'icone et de la methode associée
    openAct->setShortcut(QKeySequence::Open); // ajout raccourci
    fileMenu->addAction(openAct); // Ajout à la barre de menu

    //Sauvegarder
    saveAct = myToolBar->addAction(QIcon(":/myresources/save.png"), tr("Save"), this, &MainWindow::saveAs);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setEnabled(false);
    fileMenu->addAction(saveAct);

    //Quitter
    exitAct = fileMenu->addAction(tr("Exit"), this, &MainWindow::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    myToolBar->addSeparator(); // séparateur pour aggréger les fonctionnalités

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    //Zoom avant
    zoomInAct = myToolBar->addAction(QIcon(":/myresources/zoom_in.svg"), tr("Zoom In"), this, &MainWindow::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    editMenu->addAction(zoomInAct);

    //Zoom arrière
    zoomOutAct = myToolBar->addAction(QIcon(":/myresources/zoom_out.svg"), tr("Zoom Out"), this, &MainWindow::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    editMenu->addAction(zoomOutAct);

    myToolBar->addSeparator();

    //Flip mirroir vertical
    flipAct = myToolBar->addAction(QIcon(":/myresources/flip.ico"), tr("Flip Vertical"), this, &MainWindow::flip);
    flipAct->setEnabled(false);
    editMenu->addAction(flipAct);

    //Flip mirroir horizontal
    flipHorizontalAct = myToolBar->addAction(QIcon(":/myresources/flip_horizontal.png"), tr("Flip Horizontal"), this,
                                             &MainWindow::flipHorizontal);
    flipHorizontalAct->setEnabled(false);
    editMenu->addAction(flipHorizontalAct);

    //Rotation droite
    rotateRightAct = myToolBar->addAction(QIcon(":/myresources/rotate_right.svg"), tr("Rotate Right"), this,
                                          &MainWindow::rotateRight);
    rotateRightAct->setEnabled(false);
    editMenu->addAction(rotateRightAct);

    //Rotation gauche
    rotateLeftAct = myToolBar->addAction(QIcon(":/myresources/rotate_left.svg"), tr("Rotate Left"), this,
                                         &MainWindow::rotateLeft);
    rotateLeftAct->setEnabled(false);
    editMenu->addAction(rotateLeftAct);

    QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutMenu->addAction(tr("&About this app"), this, &MainWindow::about);
    aboutMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);

    myToolBar->addSeparator();

    //Retour propriétés par défaut de l'image
    setDefaultAct = myToolBar->addAction(QIcon(":/myresources/undo.ico"), tr("Set to default"), this,
                                         &MainWindow::setDefault);
    setDefaultAct->setEnabled(false);
    editMenu->addAction(setDefaultAct);
}

void MainWindow::createAdvancedActions(){
    ///Initialisation
    processToolBar = new QToolBar(tr("Advanced tools"));
    addToolBar(Qt::LeftToolBarArea, processToolBar);
    processToolBar->setIconSize(QSize(35,35));

    ///Création et ajout des actions
    //thresold en déterminant automatiquement le seuil
    thresholdAct = processToolBar->addAction(QIcon(":/myresources/threshold.png"), tr("Threshold"), this,
                                         &MainWindow::threshold);
    thresholdAct->setEnabled(false);

    //Inversion de couleur
    invertAct = processToolBar->addAction(QIcon(":/myresources/inverted.png"), tr("Invert colors"), this,
                                         &MainWindow::invert);
    invertAct->setEnabled(false);

    //Blur
    blurAct = processToolBar->addAction(QIcon(":/myresources/blur.png"), tr("Blur"), this,
                                         &MainWindow::blur);
    blurAct->setEnabled(false);

    //Dithering
    ditherAct = processToolBar->addAction(QIcon(":/myresources/dither.png"), tr("Dither"), this,
                                         &MainWindow::dither);
    ditherAct->setEnabled(false);

    //Optimisation de contraste
    optimizeContrastAct = processToolBar->addAction(QIcon(":/myresources/opt_contrast.png"), tr("Optimize Contrast"), this,
                                         &MainWindow::optimizeContrast);
    optimizeContrastAct->setEnabled(false);
}

void MainWindow::open(){
    QFileDialog dialog(this, tr("Open File"));
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg)")); // seules ces extensions seront affichées lors de la sélection

    if(dialog.exec()){ // bloque toutes les autres fenetres de l'application jusqu'à erreur ou sélection
        fileName = dialog.selectedFiles().first(); // Charge la première image sélectionnée
        loadFile(this, fileName);
    }

}

void MainWindow::setCvImage(cv::Mat image){
    cvImage = image;
}

double MainWindow::getScaleFactor(){
    return scaleFactor;
}
void MainWindow::setScaleFactor(double scaleFactor){
    this->scaleFactor = scaleFactor;
}

void MainWindow::increaseScaleFactor(double scaleFactor){
    this->scaleFactor *= scaleFactor;
}

QLabel* MainWindow::getImageLabel(){
    return imageLabel;
}

QScrollArea* MainWindow::getScrollArea(){
    return scrollArea;
}

void MainWindow::setImage(QImage image){
    this->image = image;
}

QImage MainWindow::getImage(){
    return image;
}

QAction* MainWindow::getZoomInAct(){
    return zoomInAct;
}

QAction* MainWindow::getZoomOutAct(){
    return zoomOutAct;
}

void MainWindow::updateActions(){
    if(!image.isNull()){
        saveAct->setEnabled(true);
        zoomInAct->setEnabled(true);
        zoomOutAct->setEnabled(true);
        flipAct->setEnabled(true);
        flipHorizontalAct->setEnabled(true);
        rotateRightAct->setEnabled(true);
        rotateLeftAct->setEnabled(true);
        setDefaultAct->setEnabled(true);
        thresholdAct->setEnabled(true);
        invertAct->setEnabled(true);
        blurAct->setEnabled(true);
        ditherAct->setEnabled(true);
        optimizeContrastAct->setEnabled(true);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Pict"),
            tr("<p>This is a simple GUI application combining Qt and"
               " openCV for basic image processing</p>"));
}

void MainWindow::zoomIn(){
    scaleImage(this, 1.25);
}

void MainWindow::zoomOut(){
    scaleImage(this, 0.8);
}

void MainWindow::saveAs(){
    QFileDialog dialog(this, tr("Save Image As"));
    dialog.setAcceptMode(QFileDialog::AcceptSave); // pour la sauvegarde d'image
    dialog.setDefaultSuffix("jpg");

    if(dialog.exec()){
        fileName = dialog.selectedFiles().first();

        QImageWriter writer(fileName);
        if (!writer.write(image)) { // affiche une erreur en cas d'échec
            QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),tr("Cannot write %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        }else{
            QString message = tr("Saved \"%1\"").arg(QDir::toNativeSeparators(fileName));
            statusBar()->showMessage(message); // Affiche la résussite sur le status bar
        }
    }
}

void MainWindow::updateImage(){
    if(grayScale){
        image = QImage((unsigned char*)cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_Grayscale8);
    }else{
        QImage temp((unsigned char*)cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888);
        image = temp.rgbSwapped().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::flip(){
    flipImage(cvImage, 1);
    updateImage();
}

void MainWindow::flipHorizontal(){
    flipImage(cvImage, -1);
    updateImage();
}

void MainWindow::rotateRight(){
    rotateRightImage(cvImage);
    updateImage();
}

void MainWindow::rotateLeft(){
    rotateLeftImage(cvImage);
    updateImage();
}

void MainWindow::threshold(){
    if(grayScale == false){ // vérifie d'abord si l'image est une nuance d gris
        cv::cvtColor(cvImage, cvImage, COLOR_BGR2GRAY);
    }
    cv::threshold(cvImage, cvImage, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    grayScale = true;
    thresholdAct->setEnabled(false);
    ditherAct->setEnabled(false);
    optimizeContrastAct->setEnabled(false);
    updateImage();
}

void MainWindow::invert(){
    invertImage(cvImage);
    updateImage();
}

void MainWindow::blur(){
    blurImage(cvImage);
    updateImage();
}

void MainWindow::setDefault(){
    cvImage = cv::imread(fileName.toStdString());
    grayScale = false;
    thresholdAct->setEnabled(true); // l'un n'a plus d'effet apres l'autre
    ditherAct->setEnabled(true);
    optimizeContrastAct->setEnabled(true);
    updateImage();
}

void MainWindow::dither(){
    if(grayScale == false){
        cv::cvtColor(cvImage, cvImage, COLOR_BGR2GRAY);
    }
    ditherImage(cvImage);
    grayScale = true;
    thresholdAct->setEnabled(false); // l'un n'a plus d'effet apres l'autre
    ditherAct->setEnabled(false);
    optimizeContrastAct->setEnabled(false);
    updateImage();
}

void MainWindow::optimizeContrast(){
    if(grayScale == false){
        cv::cvtColor(cvImage, cvImage, COLOR_BGR2GRAY);
    }
    cv::equalizeHist(cvImage, cvImage);
    optimizeContrastAct->setEnabled(false);
    grayScale = true;
    updateImage();
}

void MainWindow::detectEdges(){
    cv::GaussianBlur( cvImage, cvImage, Size(3,3), 0, 0, BORDER_DEFAULT );
    if(grayScale == false){
        cvtColor(cvImage, cvImage, COLOR_BGR2GRAY);
    }
    Scharr(cvImage, cvImage, CV_SCHARR, 1, 0);
    grayScale = true;
    updateImage();
}








