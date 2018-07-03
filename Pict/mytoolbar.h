#ifndef MYTOOLBAR_H
#define MYTOOLBAR_H

#include <QAction>
#include <QSize>
#include <QToolBar>


class MyToolBar : public QToolBar{

    Q_OBJECT

public:
    MyToolBar(QWidget *parent = 0);

private:
    QAction *openAct;
    QAction *saveAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *flipAct;
    QAction *rotateRightAct;
    QAction *rotateLeftAct;
};

#endif // MYTOOLBAR_H
