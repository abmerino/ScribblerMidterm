#ifndef SCRIBBLER_H
#define SCRIBBLER_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QMouseEvent>
#include <QDataStream>
#include <QPointF>

class MouseEvent {
public:
    enum {
        Press,
        Move,
        Release
    };

    int action;
    QPointF pos;
    quint64 time;

    MouseEvent() : action(Press), pos(0,0), time(0) {}
    MouseEvent(int _action, QPointF _pos, quint64 _time);

    friend QDataStream &operator<<(QDataStream &out, const MouseEvent &evt);
    friend QDataStream &operator>>(QDataStream &out, MouseEvent &evt);
};

class Scribbler : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene scene;
    double lineWidth;
    QPointF lastPoint;
    QList<MouseEvent> events;

public:
    enum class ViewMode{
        LineSegments,
        DotsOnly
    };

    Scribbler(QWidget *parent = nullptr);

    // getter and setter for mouse events
    const QList<MouseEvent>& getMouseEvents() const;
    void setMouseEvents(const QList<MouseEvent>& newEvents);
    void startCapture();
    void endCapture();
    void readEvents();
    void setViewMode(ViewMode mode);
    ViewMode viewMode;
    // tds changes
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);
    void displayError(const QString& title, const QString& message);

public slots:
    void reset();
    // tds changes
    void onSaveTriggered();
    void onLoadTriggered();

signals:
    void captureEnded(const QList<MouseEvent> &events);

protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void paintEvent(QPaintEvent *event) override;

};

#endif // SCRIBBLER_H
