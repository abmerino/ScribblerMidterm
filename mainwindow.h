#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scribbler.h"
#include <QMainWindow>
#include <QtWidgets>
#include <QGraphicsScene>

class Scribbler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *centralWidget;
    QString lastDir;

    Scribbler *scribbler;
    QTabWidget *tabWidget;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //add QGraphics Ellipse pointer and
    // QList<QList<MouseEvents>>
    QList<QGraphicsItemGroup*> captureGroups;

public slots:
    void openFileSlot();
    void saveFileSlot();
    void resetFileSlot();

    void startCapture();
    void endCapture(QList<MouseEvent> &events); //slot to handle end signal
    void updateCaptureOpacity(int index);

    void lineSegments();
    void dotsOnly();
};
#endif // MAINWINDOW_H
