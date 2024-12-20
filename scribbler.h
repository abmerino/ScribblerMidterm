#ifndef SCRIBBLER_H
#define SCRIBBLER_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <QMouseEvent>
#include <QDataStream>
#include <QPointF>

class MouseEvent {
public:
    enum Action {
        Press,
        Move,
        Release
    };

    int action;
    QPointF pos;
    quint64 time;
    QGraphicsItem *graphicsItem = nullptr;


    MouseEvent() : action(Press), pos(0,0), time(0) {}
    MouseEvent(Action a, QPointF p) : action(a), pos(p) {}
    MouseEvent(int _action, QPointF _pos, quint64 _time);

    friend QDataStream &operator<<(QDataStream &out, const MouseEvent &evt);
    friend QDataStream &operator>>(QDataStream &out, MouseEvent &evt);
};
Q_DECLARE_METATYPE(MouseEvent)

class Scribbler : public QGraphicsView
{
    Q_OBJECT

public:
    enum class ViewMode{
        LineSegments,
        DotsOnly
    };
    //explicit?
    Scribbler(QWidget *parent = nullptr);

    // getter and setter for mouse events
    const QList<MouseEvent>& getMouseEvents() const;
    void setMouseEvents(const QList<MouseEvent>& newEvents);

    // capture control methods
    void startCapture();
    void endCapture();
    void readEvents();
    QGraphicsItemGroup* createCaptureGroup(QList<MouseEvent> &events);

    // view mode
    void setViewMode(ViewMode mode);

    // file operations
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);

public slots:
    void reset();
    void onSaveTriggered();
    void onLoadTriggered();

signals:
    void captureEnded(QList<MouseEvent> &events);

protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void paintEvent(QPaintEvent *event) override;

private:
    // drawing properties
    QGraphicsScene scene;
    double lineWidth;
    QPointF lastPoint;
    ViewMode viewMode;
    QElapsedTimer timer;

    QList<QList<MouseEvent>> captures; //stores all captures
    QList<MouseEvent> events; //stores the active capture
};

#endif // SCRIBBLER_H
