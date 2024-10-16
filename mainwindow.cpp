#include "mainwindow.h"
#include "scribbler.h"
#include <QtWidgets>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //create new widget on the heap
    centralWidget = new QWidget(this);

    //set new widget as central widget
    setCentralWidget(centralWidget);

    //create horizontal layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    //create scribbler class and set to central widget
    scribbler = new Scribbler();
    scribbler->setObjectName("scribbler");
    mainLayout->addWidget(scribbler, 1);

    //right-side layout for QTabWidget
    QVBoxLayout *rightLayout = new QVBoxLayout;
    //initialize tabWidget initially hidden on right-side
    tabWidget = new QTabWidget(centralWidget);
    //tabWidget->setTabBarAutoHide(true);
    rightLayout->addWidget(tabWidget);

    QWidget *rightWidget = new QWidget(centralWidget);
    rightWidget->setLayout(rightLayout);
    mainLayout->addWidget(rightWidget, 1);

    //create Menu bar
    //File Menu:
    QMenu *fileMenu = new QMenu("&File");

    QAction *openFileAct = new QAction("&Open File", this);
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFileSlot);
    openFileAct->setShortcut(Qt::CTRL| Qt::Key_O);
    fileMenu->addAction(openFileAct);

    QAction *saveFileAct = new QAction("&Save File", this);
    connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFileSlot);
    saveFileAct->setShortcut(Qt::CTRL | Qt::Key_S);
    fileMenu->addAction(saveFileAct);

    QAction *resetScribbleAct = new QAction("&Reset Scribbler", this);
    connect(resetScribbleAct, &QAction::triggered, this, &MainWindow::resetFileSlot);
    resetScribbleAct->setShortcut(Qt::CTRL | Qt::Key_R);
    fileMenu->addAction(resetScribbleAct);

    menuBar()->addMenu(fileMenu);

    //Capture Menu:
    QMenu *captureMenu = new QMenu("&Capture");

    QAction *startCap = new QAction("&Start", this);
    connect(startCap, &QAction::triggered, this, &MainWindow::startCapture);
    //startCap->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    captureMenu->addAction(startCap);

    QAction *endCap = new QAction("&End", this);
    connect(endCap, &QAction::triggered, this, &MainWindow::endCapture);
    //endCap->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_E);
    captureMenu->addAction(endCap);

    menuBar()->addMenu(captureMenu);

    //View Menu:

}

MainWindow::~MainWindow() {}

void MainWindow::openFileSlot() {

}

void MainWindow::saveFileSlot() {

}

void MainWindow::resetFileSlot() {

}
