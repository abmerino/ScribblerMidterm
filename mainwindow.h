#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QGraphicsScene>

class Scribbler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *centralWidget;

    Scribbler *scribbler;
    QTabWidget *tabWidget;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFileSlot();
    void saveFileSlot();
    void resetFileSlot();

    void startCapture();
    void endCapture();

    void lineSegmentsOnly();
    void dotsOnly();
};
#endif // MAINWINDOW_H
