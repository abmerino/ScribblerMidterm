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
    tabWidget = new QTabWidget(centralWidget); //initialize tabWidget initially hidden on right-side

    // sample tab
    QWidget *sampleTab = new QWidget();
    QLabel *sampleLabel = new QLabel("Sample Tab Conent");
    QVBoxLayout *sampleTabLayout = new QVBoxLayout(sampleTab);
    sampleTabLayout->addWidget(sampleLabel);
    sampleTab->setLayout(sampleTabLayout);
    tabWidget->addTab(sampleTab, "Tab 1");
    //////////
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

    QAction *startCap = new QAction("&Start Capture", this);
    connect(startCap, &QAction::triggered, this, &MainWindow::startCapture);
    startCap->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    captureMenu->addAction(startCap);

    QAction *endCap = new QAction("&End Capture", this);
    connect(endCap, &QAction::triggered, scribbler, &Scribbler::endCapture);
    endCap->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_E);
    captureMenu->addAction(endCap);

    menuBar()->addMenu(captureMenu);


    //View Menu:
    QMenu *viewMenu = new QMenu("&View");

    QAction *startls = new QAction("&New Line Segment", this);
    connect(startls, &QAction::triggered, this, &MainWindow::lineSegments);
    startls->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_L);
    viewMenu->addAction(startls);

    QAction *startDots = new QAction("&New Dot Segment", this);
    connect(startDots, &QAction::triggered, this, &MainWindow::dotsOnly);
    startDots->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_D);
    viewMenu->addAction(startDots);

    menuBar()->addMenu(viewMenu);
}

MainWindow::~MainWindow() {
    QSettings settings("AB Systems", "Scribbler1");
    settings.setValue("lastDir", lastDir);
}

void MainWindow::openFileSlot() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select File", lastDir, "All File (*)");
    if (fileName.isEmpty()) return;

    lastDir = QFileInfo(fileName).absolutePath();

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Error", "Unable to open file");
        return;
    }

    QDataStream in(&file);
    QList<MouseEvent> events;
    in >> events; //setter and getter methods for events in Scribbler class

    scribbler->setMouseEvents(events);
    scribbler->readEvents();
}

void MainWindow::saveFileSlot() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", lastDir, "All Files (*)");
    if (fileName.isEmpty()) return;

    lastDir = QFileInfo(fileName).absolutePath();

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, "Error", "Cannot save file");
        return;
    }

    QDataStream out(&file);
    out << scribbler->getMouseEvents();
    file.close();
}

void MainWindow::resetFileSlot() {
    scribbler->reset();
    tabWidget->clear();
    tabWidget->hide();
}

void MainWindow::startCapture() {
    scribbler->startCapture();
    tabWidget->hide();
}

void MainWindow::endCapture(const QList<MouseEvent> &events) {
    QTableWidget *table = new QTableWidget(events.size(), 3, this);
    table->setHorizontalHeaderLabels(QStringList() << "Action" << "Position X" << "Position Y");


    for (int i = 0; i<events.size(); ++i) {
        const MouseEvent &evt = events[i];

        QTableWidgetItem *actionItem = new QTableWidgetItem();
        switch (evt.action) {
            case MouseEvent::Press: actionItem->setText("Press"); break;
            case MouseEvent::Move: actionItem->setText("Move"); break;
            case MouseEvent::Release: actionItem->setText("Release"); break;
        }
        table->setItem(i, 0, actionItem);

        // x column
        QTableWidgetItem *rItem = new QTableWidgetItem(QString::number(evt.pos.x()));
        table->setItem(i, 1, rItem);

        QTableWidgetItem *cItem = new QTableWidgetItem(QString::number(evt.pos.y()));
        table->setItem(i, 2, cItem);
    }

    QString tabName = QString("Capture %1").arg(tabWidget->count() + 1);
    tabWidget->addTab(table, tabName);
    tabWidget->show();
}

void MainWindow::lineSegments() {
    scribbler->setViewMode(Scribbler::ViewMode::LineSegments);
}

void MainWindow::dotsOnly() {
    scribbler->setViewMode(Scribbler::ViewMode::DotsOnly);
}
