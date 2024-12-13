#include "mainwindow.h"
#include "scribbler.h"
#include <QtWidgets>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // initialize QSettings to load last used directory
    QSettings settings ("AB Systems", "Scribbler1");
    lastDir = settings.value("lastDir", QDir::homePath()).toString();

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

    connect(scribbler, &Scribbler::captureEnded, this, &MainWindow::endCapture);

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
    //updates capture opacity for tab switching
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateCaptureOpacity);

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

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Error", "Unable to open file");
        return;
    }

    // update lastDir to the directory of the loaded file
    lastDir = QFileInfo(fileName).absolutePath();

    QDataStream in(&file);
    QList<MouseEvent> events;
    in >> events; //setter and getter methods for events in Scribbler class

    scribbler->setMouseEvents(events);
    scribbler->readEvents();
}

void MainWindow::saveFileSlot() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", lastDir, "All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, "Error", "Cannot save file");
        return;
    }

    // update lastDir to the directory of the saved file
    lastDir = QFileInfo(fileName).absolutePath();

    QDataStream out(&file);
    out << scribbler->getMouseEvents();
}

void MainWindow::resetFileSlot() {
    scribbler->reset();
    tabWidget->clear();
    tabWidget->hide();
}

void MainWindow::startCapture() {
    scribbler->startCapture();
    tabWidget->hide(); //hides tabs during capture
}

void MainWindow::endCapture(QList<MouseEvent> &events) {
    //qDebug() << "End Capture triggered--Event size:" << events.size();
    if (events.isEmpty()) {
        QMessageBox::warning(this, "No Data", "No mouse events captured");
        return;
    }

    //create new capture group and add to the graphics scene
    QGraphicsItemGroup *group = scribbler->createCaptureGroup(events);
    captureGroups.append(group);

    //create table
    QTableWidget *table = new QTableWidget(events.size(), 6, this);
    //table->setHorizontalHeaderLabels(QStringList() << "Action Type" << "X Position" << "Y Position" << "Distance" << "Speed" << "Angle");
    table->setHorizontalHeaderLabels({"Action", "X Position", "Y Position", "Distance", "Speed", "Angle"});

    for (int i = 0; i<events.size(); ++i) {
        const MouseEvent &evt = events[i];

        // QTableWidgetItem *actionItem = new QTableWidgetItem();
        // actionItem->setText(evt.action == MouseEvent::Press ? "Press" : evt.action == MouseEvent::Move ? "Move" : "Release");

        // table->setItem(i, 0, actionItem);

        // // x column
        // QTableWidgetItem *xItem = new QTableWidgetItem(QString::number(evt.pos.x()));
        // table->setItem(i, 1, xItem);

        // // y column
        // QTableWidgetItem *yItem = new QTableWidgetItem(QString::number(evt.pos.y()));
        // table->setItem(i, 2, yItem);

        table->setItem(i, 0, new QTableWidgetItem(evt.action == MouseEvent::Press ? "Press" : evt.action == MouseEvent::Move ? "Move" : "Release"));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(evt.pos.x())));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(evt.pos.y())));

        if (i > 0) {
            // calculate distance
            double distance = QLineF(events[i-1].pos, evt.pos).length();
            table->setItem(i, 3, new QTableWidgetItem(QString::number(distance)));

            // calculate speed
            double timeDifference = evt.time - events[i-1].time;
            double speed = timeDifference > 0 ? distance / timeDifference : 0;
            table->setItem(i, 4, new QTableWidgetItem(QString::number(speed)));

            // calculate angle
            if (i > 1) {
                QLineF line1(events[i-2].pos, events[i -1].pos);
                QLineF line2(events[i-1].pos, evt.pos);
                double angle = std::abs(line1.angleTo(line2));
                table->setItem(i, 5, new QTableWidgetItem(QString::number(angle)));
            }
        }
    }

    // numbered tab names, incremented for each capture
    QString tabName = QString("Capture %1").arg(tabWidget->count() + 1);
    tabWidget->addTab(table, tabName);
    tabWidget->show();

    //connect table selection to opacity adjustment
    connect(table->selectionModel(), &QItemSelectionModel::selectionChanged, [=, &events](const QItemSelection &selected) {
        for (const MouseEvent &evt : events) {
            //set all to 25% opacity
            if (evt.graphicsItem) {
                evt.graphicsItem->setOpacity(0.25);
            }
        }
        //highlight selected items (100% opacity)
        for (const QModelIndex &index : selected.indexes()) {
            if (index.isValid() && index.row() < events.size()) {
                const MouseEvent &evt = events[index.row()];
                if (evt.graphicsItem) {
                    evt.graphicsItem->setOpacity(1.0);
                }
            }
        }
    });
    // connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateCaptureOpacity);
}

void MainWindow::updateCaptureOpacity(int index) {
    for (int i = 0; i < captureGroups.size(); ++i) {
        //captureGroups[i]->setOpacity(i == index ? 1.0 : 0.25);
        QGraphicsItemGroup *group = captureGroups[i];
        if (group) {
            group->setOpacity(i == index ? 1.0 : 0.25);
        }
    }
}

void MainWindow::lineSegments() {
    scribbler->setViewMode(Scribbler::ViewMode::LineSegments);
}

void MainWindow::dotsOnly() {
    scribbler->setViewMode(Scribbler::ViewMode::DotsOnly);
}
